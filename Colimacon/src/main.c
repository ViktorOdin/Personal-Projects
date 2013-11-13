/**
 *	Victor Oudin
 *	11/04/2013
 *
 *	main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "colimacon.h"

/**
 *	test that an array was spirally filled, with four loops that successively course the top line,
 *	the right column, the bottom line and the left column.
 *
 *	parameters :
 *	-array		an array.
 *	-rows		size of line.
 *	-columns	size of column.
 *
 *	return : returns 1 for the success of the function.
 */
int testColimacon(int **array, unsigned int rows, unsigned int columns){

	int i,j;
	int value = 1;
	int MAX_VALUE= rows * columns;
	int top_line = 0;
	int bottom_line = rows - 1;
	int left_column = 0;
	int right_column = columns - 1;

	while(value <= MAX_VALUE){
		
		for (j=left_column; j<=right_column && value<=MAX_VALUE; j++)
			if((*array)[top_line * columns + j] != value++) return 0;

		for (i=top_line+1; i<=bottom_line && value<=MAX_VALUE; i++)
			if((*array)[i * columns + right_column] != value++) return 0;

		for (j=right_column-1; j>=left_column && value<=MAX_VALUE; j--)
			if((*array)[bottom_line * columns + j] != value++) return 0;

		for (i=bottom_line-1; i>top_line && value<=MAX_VALUE; i--)
			if((*array)[i * columns + left_column] != value++) return 0;

		top_line++; bottom_line--; left_column++; right_column--;
	}

	return 1;
}

/**
 *	main
 */
int main(int argc, const char * argv[]){

	unsigned int rows,columns;
	
	/* registration of arguments */
	if(argc<2){ perror("Invalid number of arguments, enter 1 or 2 integer"); return 0; }	
	else if(argc==2){
		rows = atoi(argv[1]);
		columns = atoi(argv[1]);
	}	
	else if(argc==3){
		rows = atoi(argv[1]);
		columns = atoi(argv[2]);
	}
	else{ perror("Invalid number of arguments, enter 1 or 2 integer"); return 0; }

	if (rows < 1 || columns < 1) {
		perror("invalid arguments, enter arguments superior to 0");
		return 0;
	}
	
	int res;
	int *array;

	/* allocated and filling the array of arrays */
	if((res = colimacon(&array, rows, columns)) == 0){
		perror("");
	    	return 0;
	}

	unsigned int i,j;

	/* printing the array of arrays*/
	for (i = 0 ; i < rows; i++) {
		for (j = 0; j < columns; j++) {
			printf("%d\t", array[i*columns + j]);
		}
		printf("\n");
	}

	res = testColimacon(&array, rows, columns);
	if(res == 0){
		printf("Bad filled of Array, not Colimacon\n");
	} 

	/* free allocated memory */
	free(array);
    
	return 0;
}

