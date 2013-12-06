/*
 *	Victor Oudin
 *	11/19/2013
 *
 *	fork.h
 */

#ifndef FORK_H
#define FORK_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>

/* Measuring the time of creation and waiting for the termination of "n" forks type process in microsecond */
struct timespec timeCreateTermFork(int n);

/* Measurement the time of "n" context switching between 2 forks in microsecond */
/* with semaphores */
struct timespec timeContextSwitchForkSema(int n);

/* with pipes */
struct timespec timeContextSwitchForkPipe(int n);


#endif /* FORK_H */
