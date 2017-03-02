#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;


void mergeSort(int[], int);		// merge sort
void mergeSortRecurse(int[], int, int[], int[]);

#define swap( x, y ) { int t=x; x=y; y=t; }



//  Merge Sort
//  Strategy:  divide the list in half
//             sort each half
//             merge two sorted arrays together
//  In this implementation, the merging space is allocated in advance
//  so that each recursive call would not need to make new memory
void mergeSortRecurse(int array[], int size, int listA[], int listB[])
{
	int halfSize = size / 2,
		otherHalf = size - halfSize;
	int i, j, out;

	if (size > 1)		// we have work to do
	{
		// sort the two halves (listA and listB are workspace)
		mergeSortRecurse(array, halfSize, listA, listB);
		mergeSortRecurse(array + halfSize, otherHalf, listA, listB);

		// now copy our sorted halves
		for (i = 0; i < halfSize; i++)
			listA[i] = array[i];
		for (j = 0; i < size; i++, j++)
			listB[j] = array[i];

		// copy the front elements of each list
		i = j = out = 0;
		while (i < halfSize && j < otherHalf)
			if (listA[i] < listB[j])
				array[out++] = listA[i++];
			else
				array[out++] = listB[j++];

		// copy the leftovers
		while (i < halfSize)
			array[out++] = listA[i++];
		while (j < otherHalf)
			array[out++] = listB[j++];
	}
}

void mergeSort(int array[], int size)
{
	int *listA = new int[1 + size / 2];	// +1 in case size is odd
	int *listB = new int[1 + size / 2];

	mergeSortRecurse(array, size, listA, listB);

	delete[] listA;
	delete[] listB;
}
