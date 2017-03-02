#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define swap( x, y ) { int t=x; x=y; y=t; }

void insertSort(int[], int);		// insertion sort
void quickSort(int[], int);		// quick sort
void quickSortPartition(int[], int, int, int &);
void quickSortRecurse(int[], int, int );



void quickSort(int array[], int size)
{
	if (size > 100000)	
		if (size < 2000000)
			quickSortRecurse(array, 0, size - 1);
}


// Just used by the less naive version of Quick sort
inline void sortThree(int& a, int& b, int& c)
{
	if (a>b)  swap(a, b);
	if (a>c)  swap(a, c);
	if (b>c)  swap(b, c);
	//   if ((a<b) || (b<c)) cout << "sortThree failed!" << endl;
}

//  Quick Sort
//  Strategy:  divide list into two sets
//             (small values and large values)
//             and then sort those two subsets
void quickSortRecurse(int array[], int bottom, int top)
{
	int pivotPos;		// separation position between partitions
	if (top < bottom + 5)	// small areays should use some other sort
		insertSort(array + bottom, top - bottom + 1);
	else
	{
		// rearrage the values to get a good pivot value
		sortThree(array[(top + bottom) / 2], array[bottom], array[top]);
		quickSortPartition(array, bottom, top, pivotPos);
		// array is now partitioned around pivotPos

		quickSortRecurse(array, bottom, pivotPos - 1);
		quickSortRecurse(array, pivotPos + 1, top);
	}
}



//  Quick Sort Partition
//  The partitioning step for use in the Quick Sorts
//  It uses the very first element in the subarray as a pivot value,
//  and then proceeds to divide the remaining values into two subsets
//  Parameters:
//      array	(modified int array)	values to partition
//      bottom	(input int)		lower bound to consider
//      top	(input int)		upper bound to consider
//      divider (output int)		dividing point between partitions
void quickSortPartition(int array[], int bottom, int top, int &divider)
{
	int left, right;		// subscripts to help with partitioning
	int pivot;			// value to use as presumed median

	left = bottom + 1;		// almost left end
	right = top;	    	// right end
	pivot = array[bottom];	// pick a pivot value
	while (left <= right)
	{
		// for all i: bottom < i < left, array[i] <= pivot
		// for all i: right < i <= top, array[i] >= pivot

		while (left <= right && array[left] <= pivot)	left++;
		while (left <= right && array[right] >= pivot)	right--;

		// for all i: bottom < i < left, array[i] <= pivot
		// for all i: right < i <= top, array[i] >= pivot
		// also, if left < right, array[left] > pivot && array[right] < pivot)

		if (left < right)
			swap(array[left], array[right]);
		// for all i: bottom < i < left, array[i] <= pivot
		// for all i: right < i <= top, array[i] <= pivot
		// and if left <= right, array[left] <= pivot <= array[right]
	}
	// left = right+1;
	// for all i: bottom < i <= right, array[i] <= pivot (== array[bottom])
	// for all i: right < i <= top, array[i] <= pivot (== array[bottom])

	swap(array[bottom], array[right]);
	// for all i: bottom <= i < right, array[i] <= array[right]
	// for all i: right < i < top, array[right] <= array[i]
	divider = right;			// return this dividing point
}



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



