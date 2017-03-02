#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define swap( x, y ) { int t=x; x=y; y=t; }



//  Insertion Sort
//  Strategy:  build a list of sorted data by examining 
//             each new unsorted value in turn
void insertSort(int array[], int size)
{
	int newValue;		// new value to insert
	int pos;			// find a place to put it

	for (int newPos = 1; newPos < size; newPos++)
	{
		// for all i,j: 0 <= i <= j < newPos, array[i] <= array[j]
		newValue = array[newPos];
		for (pos = newPos; pos > 0 && newValue < array[pos - 1]; pos--)
		{
			array[pos] = array[pos - 1];
		}
		// for all i,j: 0 <= i <= j <= pos, 
		// 	array[i] <= array[j] <= newValue
		// for all i,j: pos+1 <= i <= j <= newPos, 
		// 	newValue < array[i] <= array[j]
		// if loop repeated, newvalue < array[pos] == array[pos+1]
		array[pos] = newValue;
		// for all i,j: 0 <= i <= j <= newPos, array[i] <= array[j]
	}
}