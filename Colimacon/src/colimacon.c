/**
 *	Victor Oudin
 *	11/04/2013
 *
 *	colimacon.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "colimacon.h"

/**
 *	filled the outside ring of an sub array with four loops that successively course the top line,
 *	the right column, the bottom line and the left column, at the position pos_x (abscissa)
 *	and pos_y (ordinate).
 *
 *	parameters :
 *	-array		an array of arrays.
 *	-rows		size of line.
 *	-columns	size of column.
 *	-sub_rows	size of line of the sub array.
 *	-sub_columns	size of column of the sub array.
 *	-pos_x		abscissa of orignne of the sub array.
 *	-pos_y		ordinate of orignne of the sub array.
 *	-value		first value to insert in the sub array.
 *
 *	return : returns the first value for the next ring.
 */
int ring(int **array, int rows, int columns, int sub_rows, int sub_columns, int pos_x, int pos_y, int value){

	if(sub_rows<2 || sub_columns<2) return 0;

	int i,j;
	int count = 0;

	for (j=pos_x; j<sub_columns+pos_x && value<=rows*columns; j++){		/* insert of top line */
		(*array)[pos_y * columns + j] = value+count;
		count++;
	}
	for (i=pos_y+1; i<sub_rows+pos_y && value<=rows*columns; i++){		/* insert of rigth column */
		(*array)[i * columns + sub_columns+pos_x-1] = value+count;
		count++;
	}
	
	for (j=(sub_columns+pos_x-2); j>=pos_x && value<=rows*columns; j--){	/* insert of bottom line */
		(*array)[(sub_rows+pos_y-1) * columns + j] = value+count;
		count++;
	}
	
	for (i=(sub_rows+pos_y-2); i>pos_y && value<=rows*columns; i--){	/* insert of left column */
		(*array)[i * columns + pos_x] = value+count;
		count++;
	}
	return value+count;
}

/**
 *	spirally filled an sub array by recursion at the position pos_x (abscissa)
 *	and pos_y (ordinate).
 *
 *	parameters :
 *	-array		an array of arrays.
 *	-rows		size of line.
 *	-columns	size of column.
 *	-sub_rows	size of line of the sub array.
 *	-sub_columns	size of column of the sub array.
 *	-pos_x		abscissa of orignne of the sub array.
 *	-pos_y		ordinate of orignne of the sub array.
 *	-value		first value to insert in the sub array.
 *
 *	return : returns the first value for the next ring.
 */
int recColimacon(int **array, int rows, int columns, int sub_rows, int sub_columns, int pos_x, int pos_y, int value){

	/* filled 1 square */
	if(sub_rows==1 && sub_columns==1 && value<=rows*columns){
		(*array)[pos_y * columns + pos_x] = value;
		return 1;
	}

	/* filled 1 line */
	if(sub_rows==1 && value<=rows*columns){
		(*array)[pos_y * columns + pos_x] = value;
		if(recColimacon(array,rows,columns,sub_rows,(sub_columns-1),(pos_x+1),pos_y,(value+1))==0){
			perror("recColimacon");
			return 0;
		}
		return 1;
	}

	/* filled 1 column */
	if(sub_columns==1 && value<=rows*columns){
		(*array)[pos_y * columns + pos_x] = value;
		if(recColimacon(array,rows,columns,(sub_rows-1),sub_columns,pos_x,(pos_y+1),(value+1))==0){
			perror("recColimacon");
			return 0;
		}
		return 1;
	}

	/* filled the outside ring of the array */
	int res=ring(array,rows,columns,sub_rows,sub_columns,pos_x,pos_y,value);

	/* if the array size is 2 rows or 2 columns, no need to make recursive call */
	if(sub_rows==2 || sub_columns==2) return 1;

	/* recursive call for array bigger than 2 rows and 2 columns */
	if(recColimacon(array,rows,columns,(sub_rows-2),(sub_columns-2),(pos_x+1),(pos_y+1),res)==0){
		perror("recColimacon");
		return 0;
	}

	return 1;
}

int colimacon(int **array, unsigned int rows, unsigned int columns){

	/* memory allocation */
	if((*array = malloc(rows*columns*sizeof(int))) == NULL){
		perror("Allocation Failed");
		return 0;
	}

	/* filling the array of arrays by recursion */
	if((recColimacon(array,rows,columns,rows,columns,0,0,1))==0){
		perror("recColimacon");
		return 0;
	}
	
	return 1;
}

