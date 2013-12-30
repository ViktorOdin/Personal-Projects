/*
 *	Victor Oudin
 *	21/12/2013
 *
 *	main.c
 */

#define _GNU_SOURCE

#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

static int inversion;
pthread_barrier_t barriere;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*** HIGH thread ***/
void* threadA(){
	/*** set HIGH priority ***/
	struct sched_param p;
	p.sched_priority=9;
	pthread_setschedparam (pthread_self(), SCHED_FIFO, &p);
	/*** wait barrier ***/
	pthread_barrier_wait(&barriere);
	sleep(1);		/*** begin 1 seconds after release ***/ //2nd to run 
	printf("thread A\nA tente de prendre le mutex\n");
	/*** lock mutex ***/
	pthread_mutex_lock(&mutex);
	printf("A lock\n");

	/*** unlock mutex ***/
	printf("A unlock\n");
	pthread_mutex_unlock(&mutex);

	printf("fin A: ");
	if(inversion == 1)	/*** noticing priority inversion ***/
		printf(" \e[41m\e[1;37minversion de priorité détectée\e[0m\n");
	else			/*** not noticing priority inversion ***/
		printf(" \e[44m\e[1;37mpas d'inversion de priorité\e[0m\n");
	pthread_exit(NULL);
}
/*** MEDIUM thread ***/
void* threadB(){
	/*** set MEDIUM priority ***/
	struct sched_param p;
	p.sched_priority=6;
	pthread_setschedparam (pthread_self(), SCHED_FIFO, &p);
	/*** wait barrier ***/
	pthread_barrier_wait(&barriere);
	sleep(2);	/*** begin 2 seconds after release ***/ //3rd tu run
	printf("thread B\n");
	
	int i=0;
	while(i<999999){
		i++;
	}
	/*** set inversion ***/
	inversion = 1;

	printf("fin B\n");
	pthread_exit(NULL);
}
/*** LOW thread ***/
void* threadC(){
	/*** set LOW priority ***/
	struct sched_param p;
	p.sched_priority=3;
	pthread_setschedparam (pthread_self(), SCHED_FIFO, &p);
	/*** wait barrier ***/
	pthread_barrier_wait(&barriere);		//1st to run
	printf("thread C\n");
	/*** lock mutex ***/
	pthread_mutex_lock(&mutex);
	printf("C lock\n");
	int i=0;
	while(i<999999999){
		i++;
	}
	/*** unlock mutex ***/
	printf("C unlock\n");
	pthread_mutex_unlock(&mutex);	

	printf("fin C\n");
	pthread_exit(NULL);
}

int main(int argc, const char * argv[]){

	cpu_set_t cpu_set;
	pthread_t A;
	pthread_t B;
	pthread_t C;
	pthread_mutexattr_t m_attr;
	static int heritage;

	if(argc == 2)
		heritage = atoi(argv[1]);
	if(argc > 2){
		printf("Invalid number of arguments,\n");
		printf("n'entrez rien ou 0 pour montrer l'inversion de priorité,\n");
		perror("entrez 1 pour montrer l'héritage de priorité.\n");
		exit(EXIT_FAILURE); 
	}
	
	if(pthread_mutexattr_init(&m_attr) != 0){
		perror("pthread_mutexattr_init failed\n");
		exit(EXIT_FAILURE);
	}
	if(heritage){	/*** héritage de priorité ***/
		#ifdef _POSIX_THREAD_PRIO_INHERIT
			printf("_POSIX_THREAD_PRIO_INHERIT supported\n");
		#else
			printf("_POSIX_THREAD_PRIO_INHERIT not supported\n");
			exit(EXIT_FAILURE);
		#endif
		if(pthread_mutexattr_setprotocol(&m_attr, PTHREAD_PRIO_INHERIT) != 0){
			perror("pthread_mutexattr_setprotocol failed\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/*** pthread init barrier & mutex ***/
	if(pthread_barrier_init(&barriere, NULL, 3) != 0){
		perror("pthread_barrier_init failed\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_mutex_init(&mutex, &m_attr) !=0){
		perror("pthread_mutex_init failed\n");
		exit(EXIT_FAILURE);
	}

	/*** Fixer l'execution sur un seul coeur ***/
	CPU_ZERO(&cpu_set);
	CPU_SET(0, &cpu_set);
	if(sched_setaffinity(0, sizeof(cpu_set), &cpu_set) != 0) {
		perror("sched_setaffinity");
		exit(EXIT_FAILURE);
	}

	/*** pthread_create ***/
	if(pthread_create(&C, NULL, threadC, NULL) != 0){ /*** LOW thread ***/
		perror("pthread_create C failed\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&A, NULL, threadA, NULL) != 0){ /*** HIGH thread ***/
		perror("pthread_create A failed\n");
		exit(EXIT_FAILURE);
	}
	if(pthread_create(&B, NULL, threadB, NULL) != 0){ /*** MEDIUM thread ***/
		perror("pthread_create B failed\n");
		exit(EXIT_FAILURE);
	}
	/*** pthread_join ***/
	if((pthread_join(A, NULL) != 0)||(pthread_join(B, NULL) != 0)||(pthread_join(C, NULL) != 0)){
		perror("pthread_join failed");
		exit(EXIT_FAILURE);
	}

	return 0;
}
