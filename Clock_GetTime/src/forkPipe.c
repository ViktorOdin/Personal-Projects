/*
 *	Victor Oudin
 *	11/19/2013
 *
 *	forkPipe.c
 */

#include "fork.h"

/* Measurement the time of "n" context switching with pipes between 2 forks in microsecond */
struct timespec timeContextSwitchForkPipe(int n){

	/* Variables */
	struct timespec start, stop, time;	/* beginning, end, and time of measurement */
	pid_t pid;				/* Son's pid */
	int i;					/* loop iterator */
	int son[2], father[2];			/* Son's and Father's pipes */
	char * buf;				/* buffer for pipes */

	/* Initialization of Son's and Father's pipes */
	if(pipe(son)==-1){ 
		perror("creation son's pipe failed");
		exit(EXIT_FAILURE);
	}
	if(pipe(father)==-1){ 
		perror("creation father's pipe failed");
		exit(EXIT_FAILURE);
	}

	/* Writing in Son's pipe for the father begins */
	if(write(son[1], "1", 1) == -1){
		perror("writing father's pipe failed\n");
		exit(EXIT_FAILURE);
	}

	/* Creation of Son process */
	if((pid = fork()) == -1){
		perror("fork failed");
		exit(EXIT_FAILURE);
	}

	if(pid == 0){			/* Son process */
		close(father[1]);	/* close writing side of Father's pipe */
		close(son[0]);		/* close reading side of Son's pipe */
	}
	else{				/* Father process */
		close(son[1]);		/* close writing side of Son's pipe */
		close(father[0]);	/* close reading side of Father's pipe */
	}

	/* Start the measurement */
	if(clock_gettime(CLOCK_REALTIME, &start) == -1){
		perror("clock_gettime failed");
		exit(EXIT_FAILURE);
	}

	/* "n" context switching between Son and Father */
	for(i=0; i<n; i++){
		if(pid == 0){						/* Son process */
			if(read(father[0], &buf, 1) == -1){		/* read at Father's pipe */
				perror("reading father's pipe failed\n");
				exit(EXIT_FAILURE);
			}
			if(write(son[1], &buf, 1) == -1){		/* write in Son's pipe */
				perror("writing son's pipe failed\n");
				exit(EXIT_FAILURE);
			}
		}
		else{		/* Father process */
			if(read(son[0], &buf, 1) == -1){		/* read at Son's pipe */
				perror("reading son's pipe failed\n");
				exit(EXIT_FAILURE);
			}
			if(write(father[1], &buf, 1) == -1){		/* write in Father's pipe */
				perror("writing father's pipe failed\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	/* Stop the measurement */
	if(clock_gettime(CLOCK_REALTIME, &stop) == -1){
		perror("clock_gettime failed");
		exit(EXIT_FAILURE);
	}

	if(pid == 0){		/* termination of Son process */
		exit(0);
	}

	if ((stop.tv_nsec-start.tv_nsec)<0){
		time.tv_sec = (stop.tv_sec-start.tv_sec-1);
		time.tv_nsec = (1000000000+stop.tv_nsec-start.tv_nsec);
	}else{
		time.tv_sec = (stop.tv_sec-start.tv_sec);
		time.tv_nsec = (stop.tv_nsec-start.tv_nsec);
	}

	printf("%d switch of context of fork with pipes = %ld seconds %ld microseconds\n", n, time.tv_sec, (time.tv_nsec/1000));

	return time;
}
