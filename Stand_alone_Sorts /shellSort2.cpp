#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define swap( x, y ) { int t=x; x=y; y=t; }



void shellSort2(int[], int);		// shell sort, strides 2^p 3^q
int powMinus[25], twoThree[300], twoSize;


void shellSort2(int array[], int size)
{
	shellSort(array, size, twoThree, twoSize);
}

// Shellsort
// Strategy:  divide data into large number of small interleaved arrays
// 		 (which are very fast for insertion sort)
// 	      repeat with fewer larger interleaved arrays
// 	         (insertion sort is fast when mostly sorted)
void shellSort(int array[], int size, int strides[], int phases)
{
	int stride;
	int newValue;
	int pos;


	phases--;			// start at end of array of strides
	while (strides[phases] > size / 2)
		phases--;		// skip those with little or no work to do
	while (phases >= 0)		// then visit the rest of these
	{
		stride = strides[phases];
		for (int stripe = 0; stripe < stride; stripe++)
			for (int newPos = stripe + stride; newPos < size; newPos += stride)
			{
			newValue = array[newPos];
			for (pos = newPos; pos >= stride && newValue < array[pos - stride]; pos -= stride)
			{
				array[pos] = array[pos - stride];
			}
			array[pos] = newValue;
			}
		phases--;		// next phase (fewer, larger arrays)
	}
}