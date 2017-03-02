#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define swap( x, y ) { int t=x; x=y; y=t; }


void bubbleSort(int[], int);		// bubble sort



//  Bubble Sort
//  Strategy: look for two adjacent elements out of order
//            and swap them to put them in order
//            repeat as necessary
void bubbleSort(int array[], int size)
{
	bool sorted = false;	// not yet sorted
	while (!sorted)
	{
		sorted = true;		// assume it is sorted this time
		for (int i = 1; i<size; i++)
			if (array[i] < array[i - 1])
			{
			swap(array[i], array[i - 1]);
			sorted = false;	// nope, not sorted this time
			}
	}
	// for all i: 1 <= i < size, array[i-1] < array[i]
}
