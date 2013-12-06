/*
 *	Victor Oudin
 *	16/19/2013
 *
 *	forkSema.c
 */

#include "fork.h"

/* 2 semaphore structure for synchronization */
struct doubleSemaphore{
	sem_t father;
	sem_t son;
} doubleSemaphore;

/* Measurement the time of "n" context switching with semaphores between 2 forks in microsecond */
struct timespec timeContextSwitchForkSema(int n){

	/* Variables */
	struct timespec start, stop, time;	/* beginning, end, and time of measurement */
	pid_t pid;				/* Son's pid */
	int i;					/* loop iterator */
	int fd;					/* file descriptor */
	struct doubleSemaphore * ptr;		/* structure pointer of two semaphore */

	/* Creation file for semaphore */
	if((fd = open(".semaphore", O_RDWR | O_CREAT, 0660)) == -1){
		perror("creation file for semaphore failed\n");
		exit(EXIT_FAILURE);
	}
	/* Writing semaphore structure in file */
	if((write(fd, &doubleSemaphore, sizeof(doubleSemaphore))) == -1){
		perror("writing semaphore structure in file failed\n");
		exit(EXIT_FAILURE);
	}
	/* Mapping and creation of semaphore structure pointer */
	if((ptr = (struct doubleSemaphore *) mmap( NULL, sizeof(struct doubleSemaphore), 
					PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == ((void *) -1)){
		perror("creation of mapping semaphore structure failed\n");
		exit(EXIT_FAILURE);
	}
	/* Close file */
	if(close(fd) == -1)
		perror("close file failed\n");
	/* Initializing semaphore structure */
	if((sem_init(&ptr->father, 1, 0) != 0) || (sem_init(&ptr->son, 1, 1) != 0)){
		perror("sem_init failed");
		exit(EXIT_FAILURE);
	}
	/* Creation of Son process */
	if((pid = fork()) == -1){
		perror("fork failed");
		exit(EXIT_FAILURE);
	}

	/* Start the measurement */
	if(clock_gettime(CLOCK_REALTIME, &start) == -1){
		perror("clock_gettime failed");
		exit(EXIT_FAILURE);
	}
	/* "n" context switching between Son and Father */
	for(i=0; i<n; i++){
		if(pid == 0){					/* Son process */
			if(sem_wait(&ptr->father) == -1){	/* wait Father */
				perror("sem_wait 1 failed");
				exit(EXIT_FAILURE);
			}
			if(sem_post(&ptr->son) == -1){		/* post Son */
				perror("sem_post 2 failed");
				exit(EXIT_FAILURE);
			}
		}
		else{						/* Father process */
			if(sem_wait(&ptr->son) == -1){		/* wait Son */
				perror("sem_wait 2 failed");
				exit(EXIT_FAILURE);
			}
			if(sem_post(&ptr->father) == -1){	/* post Father */
				perror("sem_post 1 failed");
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

	printf("%d switch of context of fork with semaphores = %ld seconds %ld microseconds\n", n, time.tv_sec, (time.tv_nsec/1000));

	/* Destroy semaphores and errase semaphore file */
	if((sem_destroy(&ptr->father) == -1) || (sem_destroy(&ptr->son) == -1) || (remove(".semaphore") == -1))
		perror("sem_destroy or remove failed");

	return time;
}
