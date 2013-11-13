/**
 *	Victor Oudin
 *	11/04/2013
 *
 *	test.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "colimacon.h"

int timeColimacon(unsigned int rows, unsigned int columns, int OPTION_PRINT){
	int *array;
	time_t start, stop;
	
	start=time(NULL);

	if(colimacon(&array, rows, columns) == 0){
		perror("Error");
	    	return 0;
	}
		
	stop=time(NULL);

	if(OPTION_PRINT==1){
		unsigned int i,j;
		for (i = 0 ; i < rows; i++) {
			for (j = 0; j < columns; j++) {
				printf("%d\t", array[i*columns + j]);
			}
			printf("\n");
		}
	}

	free(array);

	return (stop-start);	
}


/**
 *	main
 */
int main(){

	unsigned int rows,columns;
	int res;

	rows=1; columns=1;
	res = timeColimacon(rows,columns,1);
	printf("Temps réel d'exécution pour colimaçon %d %d : %dsec\n",rows,columns,res); 

	rows=1; columns=10;
	res = timeColimacon(rows,columns,1);
	printf("Temps réel d'exécution pour colimaçon %d %d : %dsec\n",rows,columns,res); 

	rows=10; columns=1;
	res = timeColimacon(rows,columns,1);
	printf("Temps réel d'exécution pour colimaçon %d %d : %dsec\n",rows,columns,res); 

	rows=9; columns=9;
	res = timeColimacon(rows,columns,1);
	printf("Temps réel d'exécution pour colimaçon %d %d : %dsec\n",rows,columns,res); 

	rows=9; columns=10;
	res = timeColimacon(rows,columns,1);
	printf("Temps réel d'exécution pour colimaçon %d %d : %dsec\n",rows,columns,res); 

	rows=10; columns=9;
	res = timeColimacon(rows,columns,1);
	printf("Temps réel d'exécution pour colimaçon %d %d : %dsec\n",rows,columns,res); 

	rows=10; columns=10;
	res = timeColimacon(rows,columns,1);
	printf("Temps réel d'exécution pour colimaçon %d %d : %dsec\n",rows,columns,res); 

	rows=100; columns=100;
	res = timeColimacon(rows,columns,0);
	printf("Temps réel d'exécution pour colimaçon %d %d : %dsec\n",rows,columns,res); 

	rows=1000; columns=1000;
	res = timeColimacon(rows,columns,0);
	printf("Temps réel d'exécution pour colimaçon %d %d : %dsec\n",rows,columns,res);

	rows=10000; columns=10000;
	res = timeColimacon(rows,columns,0);
	printf("Temps réel d'exécution pour colimaçon %d %d : %dsec\n",rows,columns,res);

	return 0;
}

