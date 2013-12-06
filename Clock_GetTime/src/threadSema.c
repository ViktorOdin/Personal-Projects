/*
 *	Victor Oudin
 *	11/19/2013
 *
 *	threadSema.c
 */

#include "thread.h"

/* 2 semaphore structure for synchronization with number of switch */
struct doubleSemaWithNum{
	sem_t sem1;
	sem_t sem2;
	int number;
} doubleSemaWithNum;

/* Thread switch context*/
void* threadSwitchContextSema(void *p){

	/* Variables */
	struct doubleSemaWithNum * ptr;			/* structure pointer of two semaphore */
	ptr= (struct doubleSemaWithNum *) p;	
	int i;						/* loop iterator */
	/* "number" context switching between the 2 threads */
	for(i=0; i < (ptr->number); i++){
		if(sem_wait(&ptr->sem2) == -1){		/* wait sem2 */
			perror("sem_wait 2 failed");
			return ((void *) -1);
		}
		if(sem_post(&ptr->sem1) == -1){		/* post sem1 */
			perror("sem_post 1 failed");
			return ((void *) -1);
		}
	}

	pthread_exit(0);
}

/* Measurement the time of "n" context switching with semaphore between 2 threads in microsecond */
struct timespec timeContextSwitchThreadSema(int n){

	/* Variables */
	struct timespec start, stop, time;	/* beginning, end, and time of measurement */
	int i;					/* loop iterator */
	pthread_t thread;			/* pthread handle for create a thread */
	int fd;					/* file descriptor */
	struct doubleSemaWithNum * ptr;		/* structure pointer of two semaphore */

	/* Creation file for semaphore */
	if((fd = open(".semaphore", O_RDWR | O_CREAT, 0660)) == -1){
		perror("creation file for semaphore failed\n");
		exit(EXIT_FAILURE);
	}
	/* Writing semaphore structure in file */
	if((write(fd, &doubleSemaWithNum, sizeof(doubleSemaWithNum))) == -1){
		perror("writing semaphore structure in file failed\n");
		exit(EXIT_FAILURE);
	}
	/* Mapping and creation of semaphore structure pointer */
	if((ptr = (struct doubleSemaWithNum *) mmap( NULL, sizeof(struct doubleSemaWithNum), 
					PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == ((void *) -1)){
		perror("creation of mapping semaphore structure failed\n");
		exit(EXIT_FAILURE);
	}
	/* Close file */
	if(close(fd) == -1)
		perror("close file failed\n");
	/* Initializing semaphore structure */
	if((sem_init(&ptr->sem1, 1, 0) != 0) || (sem_init(&ptr->sem2, 1, 1) != 0)){
		perror("sem_init failed");
		exit(EXIT_FAILURE);
	}
	ptr->number=n;
	/* Create thread */
	if(pthread_create(&thread, NULL, threadSwitchContextSema, (void *) ptr) != 0){
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
		if(sem_wait(&ptr->sem1) == -1){		/* wait sem1 */
			perror("sem_wait 1 failed");
			exit(EXIT_FAILURE);
		}
		if(sem_post(&ptr->sem2) == -1){		/* post sem2 */
			perror("sem_post 2 failed");
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

	printf("%d switch of context of threads with semaphore = %ld seconds %ld microseconds\n", n, time.tv_sec,(time.tv_nsec/1000));

	/* Destroy semaphores and errase semaphore file */
	if((sem_destroy(&ptr->sem1) == -1) || (sem_destroy(&ptr->sem2) == -1) || (remove(".semaphore") == -1))
		perror("sem_destroy or remove failed");
	
	return time;
}
