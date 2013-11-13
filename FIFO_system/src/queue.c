/*
 *	queue.c
 *
 *	Victor Oudin & Azibi Mounia
 *
 */ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/uio.h>
#include "queue.h"

typedef struct queue queue;
struct queue
{
  unsigned int size_buf; // size of queue
  int nbr_msg;           // number of messages in queue
  long occupied;         // occupied space in queue
  size_t * msg_size;     // table of size of messages in queue
  void * buf;            // queue
  
  pthread_mutex_t mutex;
  pthread_cond_t full;
  pthread_cond_t empty;

};

struct queue *queue_new(size_t size){
  int rc;
  queue *q=malloc(sizeof(struct queue));
  if(q==NULL){
    free(q);
    return NULL;
  }

  q->size_buf= size;
  q->nbr_msg=0;
  q->occupied=0;

  q->msg_size= malloc(size*sizeof(size_t));
  if(q->msg_size==NULL){
    free(q->msg_size);
    free(q);
    return NULL;
  }

  q->buf= calloc(size,size); // if we do malloc, it's necessary to do memset.
  if(q->buf==NULL){
    free(q->buf);
    free(q->msg_size);
    free(q);
    return NULL;
  }
  //memset(q->buf,'\0',size);

  rc = pthread_mutex_init(&q->mutex,NULL);
  if(rc<0){
    free(q->msg_size);
    free(q->buf);
    free(q);
    return NULL;
  }

  rc = pthread_cond_init(&q->full,NULL);
  if(rc<0){
    free(q->msg_size);
    free(q->buf);
    free(q);
    return NULL;
  }

  rc = pthread_cond_init(&q->empty,NULL);
  if(rc<0){
    free(q->msg_size);
    free(q->buf);
    free(q);
    return NULL;
  }
  return q;
}


void queue_destroy(struct queue *queue){
  pthread_mutex_destroy(&queue->mutex);
  pthread_cond_destroy(&queue->empty);
  pthread_cond_destroy(&queue->full);
  free(queue->msg_size);
  free(queue->buf);
  return free(queue);
}

int enqueue(struct queue *queue, const void *buf, size_t size, int blocking){

  // locking of the mutex
  pthread_mutex_lock(&queue->mutex);

  // test of arguments
  if(queue==NULL || buf==NULL || size<=0 ){
    errno=EINVAL;
    pthread_mutex_unlock(&queue->mutex);	
    return -1;
  }
  

  if(blocking!=0){ // blocking mode
    while((queue->size_buf-(size_t)queue->occupied) < size){
      pthread_cond_wait(&(queue->full),&(queue->mutex));
    } 
  }
  if( (queue->size_buf-(size_t)queue->occupied) < size ){ // unblocking mode
    errno=EAGAIN;
    pthread_mutex_unlock(&queue->mutex);	
    return -1;
  }
  memmove(queue->buf+(queue->occupied),buf,size);
  queue->msg_size[queue->nbr_msg]=size;
  queue->nbr_msg++;
  queue->occupied += (long) size;
  pthread_mutex_unlock(&queue->mutex); // unlocking of the mutex
  
  if(blocking!=0)
    pthread_cond_broadcast(&queue->empty); // not empty any more
  
  return 1;
}

ssize_t dequeue(struct queue *queue, void *buf, size_t bufsize, int blocking){

  // locking of the mutex
  pthread_mutex_lock(&queue->mutex);

  // test of arguments
  if(queue==NULL || buf==NULL || bufsize<=0 ){
    errno=EINVAL;
    pthread_mutex_unlock(&queue->mutex);	
    return -1;
  }
  
  if(blocking!=0){ // blocking mode
    while(queue->nbr_msg==0 || queue->occupied==0 || queue->msg_size[0]==0 ){
      pthread_cond_wait(&(queue->empty),&(queue->mutex));
    }
  }
  
  if( queue->nbr_msg==0 || queue->occupied==0 || queue->msg_size[0]==0 ){ // unblocking mode
    errno=EAGAIN;
    pthread_mutex_unlock(&queue->mutex);	
    return -1;
  }
  if( bufsize<queue->msg_size[0] ){ // test size of arrival
    pthread_mutex_unlock(&queue->mutex);
    errno=EMSGSIZE;	
    return -1;
  }
  memmove(buf,queue->buf,queue->msg_size[0]);
  memmove(queue->buf,queue->buf+queue->msg_size[0],queue->size_buf-queue->msg_size[0]);
  queue->occupied -= queue->msg_size[0];
  size_t size_dequeue=queue->msg_size[0];
  memmove(queue->msg_size,queue->msg_size+1,queue->size_buf-1);
  queue->nbr_msg--;
  pthread_mutex_unlock(&queue->mutex); // unlocking of the mutex
  
  if(blocking!=0)
    pthread_cond_broadcast(&queue->full); // not full any more

  return size_dequeue;
}

int enqueue_multiple(struct queue *queue, const struct iovec *iov, int iovcnt, int blocking){

  // locking of the mutex
  pthread_mutex_lock(&queue->mutex);

  // test of arguments
  if(queue==NULL || iov==NULL || iovcnt<0 ){
    errno=EINVAL;
    pthread_mutex_unlock(&queue->mutex);	
    return -1;
  }
  
  int i=0;
  while(i<iovcnt){
    if(blocking!=0){ // blocking mode
      while((queue->size_buf-(size_t)queue->occupied) < iov[i].iov_len && i==0){
	pthread_cond_wait(&(queue->full),&(queue->mutex));
      } 
    }
    if( (queue->size_buf-(size_t)queue->occupied) < iov[i].iov_len && i==0 ){ // unblocking mode
      errno=EAGAIN;
      pthread_mutex_unlock(&queue->mutex);	
      return -1;
    }
    if( (queue->size_buf-(size_t)queue->occupied) < iov[i].iov_len && i>0 ){ // all mode 
      if(blocking!=0)
	pthread_cond_broadcast(&queue->empty); // not empty any more
      pthread_mutex_unlock(&queue->mutex); // unlocking of the mutex
      return i; // not all messages enqueue
    }
    memmove(queue->buf+(queue->occupied),iov[i].iov_base,iov[i].iov_len);
    queue->msg_size[queue->nbr_msg]=iov[i].iov_len;
    queue->nbr_msg++;
    queue->occupied += (long)iov[i].iov_len;
    i++;


  }
  pthread_mutex_unlock(&queue->mutex); // unlocking of the mutex
  
  if(blocking!=0)
    pthread_cond_broadcast(&queue->empty); // not empty any more
  return i; // all messages enqueue
}

int dequeue_multiple(struct queue *queue, struct iovec *iov, int iovcnt, int blocking){

  // locking of the mutex
  pthread_mutex_lock(&queue->mutex);

  // test of arguments
  if(queue==NULL || iov==NULL || iovcnt<0 ){
    errno=EINVAL;
    pthread_mutex_unlock(&queue->mutex);	
    return -1;
  }
  
  int i=0;
  while(i<iovcnt){
    
    if(blocking!=0){ // blocking mode
      while((queue->nbr_msg==0 || queue->occupied==0 || queue->msg_size[0]==0) && i==0 ){
	pthread_cond_wait(&(queue->empty),&(queue->mutex));
      }
    }
    if( (queue->nbr_msg==0 || queue->occupied==0 || queue->msg_size[0]==0) && i==0 ){ // unblocking mode
      errno=EAGAIN;
      pthread_mutex_unlock(&queue->mutex);	
      return -1;
    }
    if( (queue->nbr_msg==0 || queue->occupied==0 || queue->msg_size[0]==0) && i>0 ){ // all mode
      pthread_mutex_unlock(&queue->mutex); // unlocking of the mutex
      if(blocking!=0)
	pthread_cond_broadcast(&queue->full); // not full any more	
      return i; // not all messages dequeue
    }
    if( iov[i].iov_len<queue->msg_size[0]  ){
      errno=EMSGSIZE;
      if(blocking!=0 && i>0)
	pthread_cond_broadcast(&queue->full); // not full any more
      pthread_mutex_unlock(&queue->mutex); // unlocking of the mutex
      return i;
    }
    memmove(iov[i].iov_base,queue->buf,queue->msg_size[0]);  
    iov[i].iov_len=queue->msg_size[0];
    memmove(queue->buf,queue->buf+queue->msg_size[0],queue->size_buf-queue->msg_size[0]);
    queue->occupied -= queue->msg_size[0];
    memmove(queue->msg_size,queue->msg_size+1,queue->size_buf-1);
    queue->nbr_msg--;
    i++;

  }
  pthread_mutex_unlock(&queue->mutex); // unlocking of the mutex
  
  if(blocking!=0)
    pthread_cond_broadcast(&queue->full); // not full any more
  return i; // all messages dequeue
}


int enqueue_multiple_dynamic(struct queue *queue, const struct iovec *iov, int iovcnt){

  // locking of the mutex
  pthread_mutex_lock(&queue->mutex);

  // test of arguments
  if(queue==NULL || iov==NULL || iovcnt<0 ){
    errno=EINVAL;
    pthread_mutex_unlock(&queue->mutex);	
    return -1;
  }

  size_t tmp_size_buf;
  size_t * ptr_realloc_msg_size;
  void * ptr_realloc_buf;

  int i=0;
  while(i<iovcnt){ // no mode in dynamic enqueue
    if( (queue->size_buf-(size_t)queue->occupied) < iov[i].iov_len ){ //  not enough size, realloc size buffer
      tmp_size_buf=queue->size_buf*2;
      ptr_realloc_msg_size = realloc(queue->msg_size,tmp_size_buf*sizeof(size_t));
      ptr_realloc_buf= realloc(queue->buf,tmp_size_buf);
      if(ptr_realloc_msg_size==NULL || ptr_realloc_buf==NULL){
	printf("realloc failed\n");
	free(ptr_realloc_msg_size);
	free(ptr_realloc_buf);
      }
      else{
	queue->msg_size=ptr_realloc_msg_size;
	queue->buf=ptr_realloc_buf;
	memset(queue->buf+queue->size_buf,'\0',queue->size_buf);
	queue->size_buf= queue->size_buf*2;
      }
    } 
    else{ // enough size, enqueue normally
      memmove(queue->buf+(queue->occupied),iov[i].iov_base,iov[i].iov_len);
      queue->msg_size[queue->nbr_msg]=iov[i].iov_len;
      queue->nbr_msg++;
      queue->occupied += (long)iov[i].iov_len;
      i++;
    }


  }
  pthread_mutex_unlock(&queue->mutex); // unlocking of the mutex
  
  //  if(blocking!=0) // necessary for dequeue, becauce not dynamic
  pthread_cond_broadcast(&queue->empty); // not empty any more

  return iovcnt; // all messages enqueue
}
