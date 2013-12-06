/*
 *	Victor Oudin
 *	11/19/2013
 *
 *	resolution.c
 */

#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* calculation resolution of gettimeofday */
int resolutionGettimeofday(){
	struct timeval t,t2,tmp;

	gettimeofday(&t, NULL);
	
	gettimeofday(&t2, NULL);

	while ((t2.tv_sec - t.tv_sec == 0) && (t2.tv_usec - t.tv_usec == 0)){
		t.tv_sec = t2.tv_sec;
		t.tv_usec = t2.tv_usec;
		gettimeofday(&t2, NULL);		
	}
	if ((t2.tv_usec-t.tv_usec)<0){
		tmp.tv_sec = (t2.tv_sec-t.tv_sec-1);
		tmp.tv_usec = (1000000+t2.tv_usec-t.tv_usec);
	}else{
		tmp.tv_sec = (t2.tv_sec-t.tv_sec);
		tmp.tv_usec = (t2.tv_usec-t.tv_usec);
	}
	return (tmp.tv_sec*1000000+tmp.tv_usec);
}

/* calculation resolution of time */
int resolutionTime(){
	time_t t,t2;

	t = time(NULL);
	
	t2 = time(NULL);

	while ((t2 - t == 0)){
		t = t2;
		t2 = time(NULL);
	}
	return (t2 - t);
}

/* calculation resolution of clock_gettime */
int resolutionClockgettime(){
	struct timespec t,t2,tmp;

	clock_gettime(CLOCK_REALTIME, &t);

	clock_gettime(CLOCK_REALTIME, &t2);

	while ((t2.tv_sec - t.tv_sec == 0) && (t2.tv_nsec - t.tv_nsec == 0)){
		t.tv_sec = t2.tv_sec;
		t.tv_nsec = t2.tv_nsec;
		clock_gettime(CLOCK_REALTIME, &t2);
	}
	if ((t2.tv_nsec-t.tv_nsec)<0){
		tmp.tv_sec = (t2.tv_sec-t.tv_sec-1);
		tmp.tv_nsec = (1000000000+t2.tv_nsec-t.tv_nsec);
	}else{
		tmp.tv_sec = (t2.tv_sec-t.tv_sec);
		tmp.tv_nsec = (t2.tv_nsec-t.tv_nsec);
	}
	return (tmp.tv_sec*1000000000+tmp.tv_nsec);
}

int main(int argc, const char * argv[]){
        
	int i, n;

        /* registration of arguments */
	if(argc!=2){ perror("Invalid number of arguments, enter 1 integer"); return 0; }
	else{
		n = atoi(argv[1]);
	}
	
	double moy=0;

	/* make the average "n" resolution */
	for(i=0; i<n; i++){
		moy+=resolutionClockgettime();
	}
	printf("average resolution of %d clock_gettime : %f nanoseconds\n", n, (moy/n));

	moy=0;
	for(i=0; i<n; i++){
		moy+=resolutionGettimeofday();
	} 
	printf("average resolution of %d gettimeofday : %f microseconds\n", n, (moy/n));

	n=10;
	moy=0;
	for(i=0; i<n; i++){
		moy+=resolutionTime();
	} 
	printf("average resolution of %d time : %f seconds\n", n, (moy/n));

	return 0;
}
