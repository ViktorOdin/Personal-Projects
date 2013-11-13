/**
 *	Victor Oudin
 *	11/04/2013
 *
 *	colimacon.h
 *	Tp4
 */

#ifndef COLIMACON_H
#define COLIMACON_H

/**
 *	spirally filled an array of arrays with four loops that successively course the top line,
 *	the right column, the bottom line and the left column.
 *
 *	parameters :
 *	-array		an array of arrays.
 *	-rows		size of line.
 *	-columns	size of column.
 *
 *	return : returns 1 for the success of the function.
 */
int colimacon(int **array, unsigned int rows, unsigned int columns);

#endif
