/*
 *	Victor Oudin
 *	11/19/2013
 *
 *	threadPipe.c
 */

#include "thread.h"

/* 2 pipes structure for synchronization with number of switch */
struct doublePipe{
	int pipe1[2];
	int pipe2[2];
	int number;
} doublePipe;

/* Thread switch context*/
void* threadSwitchContextPipe(void *p){

	/* Variables */
	struct doublePipe * ptr;		/* structure pointer of two pipes */
	ptr= (struct doublePipe *) p;	
	int i;					/* loop iterator */
	char * buf;				/* buffer for pipes */

	/* "number" context switching between the 2 threads */
	for(i=0; i < (ptr->number); i++){
		if(read(ptr->pipe1[0], &buf, 1) == -1){		/* read at pipe1 */
			perror("reading pipe1 failed\n");
			return ((void *) -1);
		}
		if(write(ptr->pipe2[1], &buf, 1) == -1){	/* write in pipe2 */
			perror("writing pipe2 failed\n");
			return ((void *) -1);
		}
	}

	pthread_exit(0);
}

/* Measurement the time of "n" context switching with pipes between 2 threads in microsecond */
struct timespec timeContextSwitchThreadPipe(int n){

	/* Variables */
	struct timespec start, stop, time;	/* beginning, end, and time of measurement */
	int i;					/* loop iterator */
	pthread_t thread;			/* pthread handle for create a thread */
	struct doublePipe * ptr;		/* structure pointer of two pipes */
	char * buf;				/* buffer for pipes */

	/* Allocate memory for pipes structure */
	if((ptr = (struct doublePipe *)malloc(sizeof(struct doublePipe))) == NULL){
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}

	/* Initialization of pipes */
	if(pipe(ptr->pipe1)==-1){ 
		perror("creating pipe1 failed");
		exit(EXIT_FAILURE);
	}
	if(pipe(ptr->pipe2)==-1){ 
		perror("creating pipe2 failed");
		exit(EXIT_FAILURE);
	}
	ptr->number=n;	

	/* Writing in pipe2 for the pipe1 begins */
	if(write(ptr->pipe2[1], "1", 1) == -1){
		perror("writing pipe2 failed\n");
		exit(EXIT_FAILURE);
	}
	
	/* Create thread */
	if(pthread_create(&thread, NULL, threadSwitchContextPipe, (void *) ptr) != 0){
		perror("pthread_create failed");
		exit(EXIT_FAILURE);
	}

	/* Start the measurement */
	if(clock_gettime(CLOCK_REALTIME, &start) == -1){
		perror("clock_gettime failed");
		exit(EXIT_FAILURE);
	}

	/* "n" context switching between the 2 threads */
	for(i=0; i < n; i++){
		if(read(ptr->pipe2[0], &buf, 1) == -1){		/* read at pipe2 */
			perror("reading pipe2 failed\n");
			exit(EXIT_FAILURE);
		}
		if(write(ptr->pipe1[1], &buf, 1) == -1){	/* write in pipe1 */
			perror("writing pipe1 failed\n");
			exit(EXIT_FAILURE);
		}
	}

	/* Stop the measurement */
	if(clock_gettime(CLOCK_REALTIME, &stop) == -1){
		perror("clock_gettime failed");
		exit(EXIT_FAILURE);
	}
	/* Wait for the thread to terminate */
	if(pthread_join(thread, NULL) != 0){
		perror("pthread_join failed");
		exit(EXIT_FAILURE);
	}

	if ((stop.tv_nsec-start.tv_nsec)<0){
		time.tv_sec = (stop.tv_sec-start.tv_sec-1);
		time.tv_nsec = (1000000000+stop.tv_nsec-start.tv_nsec);
	}else{
		time.tv_sec = (stop.tv_sec-start.tv_sec);
		time.tv_nsec = (stop.tv_nsec-start.tv_nsec);
	}

	printf("%d switch of context of threads with pipes = %ld seconds %ld microseconds\n", n, time.tv_sec,(time.tv_nsec/1000));
	
	return time;
}
