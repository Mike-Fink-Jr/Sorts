#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define swap( x, y ) { int t=x; x=y; y=t; }




//  Selection Sort
//  Strategy:  Identify what should be first (the smallest)
//             then what should be second (next smallest), etc.
void selectSort(int array[], int size)
{
	int minPos;			// location of a smallest value
	for (int sorted = 0; sorted < size; sorted++)
	{
		// for all i,j: 0 <= i < sorted and i <= j < size, array[i] <= array[j]
		// array[sorted] <= array[sorted]
		minPos = sorted;		// start with the first
		for (int next = sorted + 1; next < size; next++)
		{
			// for all j: sorted <= j < next, array[minPos] <= array[j]
			if (array[minPos] > array[next])
				minPos = next;
			// for all j: sorted <= j <= next, array[minPos] <= array[j]
		}
		// for all j: sorted <= j < size, array[minPos] <= array[j]
		swap(array[sorted], array[minPos]);
		// for all j: sorted <= j < size, array[sorted] <= array[j]
		// for all i,j: 0 <= i <= sorted and i <= j < size, array[i] <= array[j]
	}
	// for all i,j: 0 <= i <= j < size, array[i] <= array[j]
}