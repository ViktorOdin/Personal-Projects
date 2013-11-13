/*
 *	queue-test.c
 *
 *	Victor Oudin & Azibi Mounia
 *
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/uio.h>
#include <pthread.h>
#include <sched.h>
#include "queue.h"


/* Déclaration d'une fonction définie plus tard.  Le C aime bien ça. */

static void* thread_main(void *q);

/* On va pas se fatiguer, on arrête dès qu'il y a une erreur. */

static void fatal(const char *message){
  perror(message);
  exit(1);
}

/* Le même, mais qui prend un code explicite plutôt que consulter errno. */

static void fatal_code(const char *message, int code){
  fprintf(stderr, "%s: %s\n", message,
	  code >= 0 && code < sys_nerr ? sys_errlist[code] : "Unkown error");
}

/* Ben ouais, ça n'existe pas en C. */

static int min(int i, int j){
  return i <= j ? i : j;
}

int main(){
  int i, rc;
  struct queue *queue;
  pthread_t thread;

  queue = queue_new(17);
  if(queue == NULL)
    fatal("new_queue");

  /* Crée un thread qui exécute thread_main(queue). */
  rc = pthread_create(&thread, NULL, thread_main, (void*)queue);
  if(rc != 0)
    fatal_code("pthread_create", rc);

  
  printf("Test non-bloquant.\n");
  for(i = 0; i < 100; i++) {
  again:
    rc = enqueue(queue, &i, sizeof(i), 0);
    if(rc < 0) {
      if(errno == EAGAIN) {
	sched_yield();
	goto again;
      }
      fatal("enqueue");
    }
  }
  sleep(1);


  printf("Test bloquant.\n");
  for(i = 0; i < 100; i++) {
    rc = enqueue(queue, &i, sizeof(i), 1);
    if(rc < 0)
      fatal("enqueue");
  }
  sleep(1);


  printf("Test multiple.\n");
  i = 0;
  while(i < 100) {
    int j = i + 1, k = i + 2, l = i + 3;
    struct iovec iov[4];
    iov[0].iov_base = &i; iov[0].iov_len = sizeof(i);
    iov[1].iov_base = &j; iov[1].iov_len = sizeof(j);
    iov[2].iov_base = &k; iov[2].iov_len = sizeof(k);
    iov[3].iov_base = &l; iov[3].iov_len = sizeof(l);
    rc = enqueue_multiple(queue, iov, min(97 - i, 4), 1);
    if(rc < 0)
      fatal("enqueue_multiple");
    if(rc==0)
      break;
    assert(rc > 0);
       
    i += rc;
  }

  rc = pthread_join(thread, NULL);
  if(rc != 0)
    fatal_code("pthread_join", rc);

  queue_destroy(queue);


  return 0;
}

/* La fonction exécutée par le thread. */

static void* thread_main(void *q){
  struct queue *queue = (struct queue *)q;
  int i;

  for(i = 0; i < 100; i++) {
    int val, rc;
  again2:
    rc = dequeue(queue, &val, sizeof(val), 0);
    if(rc < 0) {
      if(errno == EAGAIN) {
	sched_yield();
	goto again2;
      }
      fatal("dequeue");
    }
    assert(rc == sizeof(val) && val == i);
  }
  printf("Test non-bloquant terminé.\n");


  for(i = 0; i < 100; i++) {
    int val, rc;
    rc = dequeue(queue, &val, sizeof(val), 1);
    if(rc < 0)
      fatal("dequeue");
    assert(rc == sizeof(val) && val == i);
  }
  printf("Test bloquant terminé.\n");

  
  i = 0;
  while(i < 100) {
    int rc, j = i + 1, k = i + 2, l = i + 3;
    struct iovec iov[4];
    iov[0].iov_base = &i; iov[0].iov_len = sizeof(i);
    iov[1].iov_base = &j; iov[1].iov_len = sizeof(j);
    iov[2].iov_base = &k; iov[2].iov_len = sizeof(k);
    iov[3].iov_base = &l; iov[3].iov_len = sizeof(l);
    rc = dequeue_multiple(queue, iov, min(4, 97 - i), 1);
    if(rc < 0)
      fatal("dequeue_multiple");
    if(rc==0){
      break;
    }
    assert(rc > 0);
    i += rc;
  }
  printf("Test multiple terminé.\n");
  

  pthread_exit(NULL);
}
