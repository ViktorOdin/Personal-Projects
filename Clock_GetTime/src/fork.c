/*
 *	Victor Oudin
 *	11/19/2013
 *
 *	fork.c
 */

#include "fork.h"

/* Measuring the time of creation and waiting for the termination of "n" forks type process in microsecond */
struct timespec timeCreateTermFork(int n){

	/* Variables */
	struct timespec start, stop, time;	/* beginning, end, and measure of the time */
	pid_t pid;				/* Son's pid */
	int i;					/* loop iterator */

	/* Start the measurement */
	if(clock_gettime(CLOCK_REALTIME, &start) == -1){
		perror("clock_gettime failed");
		exit(EXIT_FAILURE);
	}

	/* Creation of "n" process */
	for(i = 0; i<n; i++){
		if((pid = fork()) == -1){	/* creation of Son process */
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		if(pid == 0) { exit(0); }	/* termination of Son process */
		else{				/* Father process waiting for Son Process to terminate */
			if(wait(NULL) == -1){
				perror("wait failed");
				exit(EXIT_FAILURE);
			}
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

	printf("create and wait for termination %d forks = %ld seconds %ld microseconds\n", n, time.tv_sec,(time.tv_nsec/1000));

	return time;
}

