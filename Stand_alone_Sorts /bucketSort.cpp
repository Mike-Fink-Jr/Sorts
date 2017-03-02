#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define swap( x, y ) { int t=x; x=y; y=t; }


void bucketSort(int[], int);		// 100000 buckets, using insertion
void insertSort(int[], int);		// insertion sort

//  Bucket Sort
//  Strategy:  divide the data into many buckets
//             and sort the buckets, then combine results
//             The insertion sort is chosen for its simplicity
void bucketSort(int array[], int size)
{
	int *buckets[10000];		// many buckets!
	int bucketSize[10000];		// the size of each bucket
	int fillPos;			// for filling the array
	int which;				// which bucket to use
	int divisor;			// how to pick which bucket

	divisor = 1 + RAND_MAX / 10000;	// round up to stay in bounds

	for (int i = 0; i<10000; i++)
	{
		buckets[i] = new int[size / 1000];	// allocate outside the stack
		bucketSize[i] = 0;		// buckets are empty
	}

	for (int i = 0; i<size; i++)
	{
		which = array[i] / divisor;
		buckets[which][bucketSize[which]] 	// pick a bucket
			= array[i];			//    to toss it in
		bucketSize[which]++;			// one more
	}

	//  sort the buckets
	for (int i = 0; i<10000; i++)
		insertSort(buckets[i], bucketSize[i]);

	//  now put them back into the array
	fillPos = 0;
	for (which = 0; which < 10000; which++)
		for (int i = 0; i < bucketSize[which]; i++, fillPos++)
			array[fillPos] = buckets[which][i];

	for (int i = 0; i<10000; i++)
		delete buckets[i];		// deallocate when done
}
