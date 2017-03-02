#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define swap( x, y ) { int t=x; x=y; y=t; }

void radixSort(int array[], int size)


//  Radix Sort
//  Strategy:  sort data just by its digits
//             starting with the rightmost digit
//             and ending with the leftmost
void radixSort(int array[], int size)
{
	int *buckets[10];			// organize on base 10 digits
	int bucketSize[10];			// the size of each collection
	int fillPos;			// for refilling the array
	int digit;				// single digit (for subscript)

	for (int i = 0; i<10; i++)
		buckets[i] = new int[size];	// allocate outside the stack

	int digitValue = 1;
	for (int j = 0; j<10; j++)		// integer holds 10-digit numbers
	{
		//  array is sorted according to rightmost digits (< digitValue)
		for (int i = 0; i<10; i++)
			bucketSize[i] = 0;		// empty buckets
		for (int i = 0; i<size; i++)
		{
			digit = (array[i] / digitValue) % 10;	// extract a digit
			buckets[digit][bucketSize[digit]] 	// pick a bucket
				= array[i];				//    to toss it in
			bucketSize[digit]++;			// one more
		}

		//  now put them back into the array
		fillPos = 0;
		for (digit = 0; digit < 10; digit++)
			for (int i = 0; i < bucketSize[digit]; i++, fillPos++)
				array[fillPos] = buckets[digit][i];

		//  array is sorted according to rightmost digits (<= digitValue)
		digitValue *= 10;		// examine next digit
	}

	for (int i = 0; i<10; i++)
		delete[] buckets[i];
}
