/*
 *	Victor Oudin
 *	11/19/2013
 *
 *	main.c
 */

#include "fork.h"
#include "thread.h"

int main(int argc, const char * argv[]){
        
	int n;
	struct timespec time;
	long int res;

        /* registration of arguments */
	if(argc!=2){ perror("Invalid number of arguments, enter 1 integer"); return 0; }
	else{
		n = atoi(argv[1]);
	}

	time = timeCreateTermFork(n);
	res = (time.tv_sec*1000000 + time.tv_nsec/1000)/n;
	printf("average time = %ld microseconds\n", res);

	printf("===================================================================================\n");
	time = timeCreateTermThread(n);
	res = (time.tv_sec*1000000 + time.tv_nsec/1000)/n;
	printf("average time = %ld microseconds\n", res);

	n=n*10;

	printf("===================================================================================\n");
	time = timeContextSwitchForkSema(n);
	res = (time.tv_sec*1000000 + time.tv_nsec/1000)/n;
	printf("average time = %ld microseconds\n", res);

	printf("===================================================================================\n");
	time = timeContextSwitchThreadSema(n);
	res = (time.tv_sec*1000000 + time.tv_nsec/1000)/n;
	printf("average time = %ld microseconds\n", res);

	printf("===================================================================================\n");
	time = timeContextSwitchForkPipe(n);
	res = (time.tv_sec*1000000 + time.tv_nsec/1000)/n;
	printf("average time = %ld microseconds\n", res);

	printf("===================================================================================\n");
	time = timeContextSwitchThreadPipe(n);
	res = (time.tv_sec*1000000 + time.tv_nsec/1000)/n;
	printf("average time = %ld microseconds\n", res);

	return 0;
}

