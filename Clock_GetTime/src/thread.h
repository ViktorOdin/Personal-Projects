/*
 *	Victor Oudin
 *	11/19/2013
 *
 *	thread.h
 */

#ifndef THREAD_H
#define THREAD_H

#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>

/* Measuring the time of creation and waiting for the termination of "n" threads in microsecond */
struct timespec timeCreateTermThread(int n);

/* Measurement the time of "n" context switching between 2 threads in microsecond */
/* with semaphores */
struct timespec timeContextSwitchThreadSema(int n);

/* with pipes */
struct timespec timeContextSwitchThreadPipe(int n);


#endif /* THREAD_H */
