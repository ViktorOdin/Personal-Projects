/*
 *	Victor Oudin
 *	11/19/2013
 *
 *	thread.c
 */

#include "thread.h"

/* Return function for thread */
void* justReturn(void* t){
	return t;
}

/* Measuring the time of creation and waiting for the termination of "n" threads in microsecond */
struct timespec timeCreateTermThread(int n){

	/* Variables */
	struct timespec start, stop, time;	/* beginning, end, and time of measurement */
	pthread_t thread;			/* identifiant of thread */
	int i;					/* loop iterator */

	/* Start the measurement */
	if(clock_gettime(CLOCK_REALTIME, &start) == -1){
		perror("clock_gettime failed");
		exit(EXIT_FAILURE);
	}
    
	/* Creation of "n" thread */
	for(i = 0; i<n; i++){
		if(pthread_create(&thread, NULL, justReturn, NULL) != 0){	/* create a thread */
			perror("pthread_create failed");
			exit(EXIT_FAILURE);
		}
		if(pthread_join(thread, NULL) != 0){				/* wait for the thread to terminate */
			perror("pthread_join failed");
			exit(EXIT_FAILURE);
		}
	}

	/* Stop the measurement */
	if(clock_gettime(CLOCK_REALTIME, &stop) == -1){
		perror("clock_gettime failed");
		exit(EXIT_FAILURE);
	}

	if ((stop.tv_nsec-start.tv_nsec)<0){
		time.tv_sec = (stop.tv_sec-start.tv_sec-1);
		time.tv_nsec = (1000000000+stop.tv_nsec-start.tv_nsec);
	}else{
		time.tv_sec = (stop.tv_sec-start.tv_sec);
		time.tv_nsec = (stop.tv_nsec-start.tv_nsec);
	}

	printf("create and wait for termination %d threads = %ld seconds %ld microseconds\n", n, time.tv_sec,(time.tv_nsec/1000));
	return time;
}

