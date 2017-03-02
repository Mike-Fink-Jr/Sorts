#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define swap( x, y ) { int t=x; x=y; y=t; }


void quickSortIter(int[], int);	// iterative quick sort
void quickSortPartition(int[], int, int, int &);
void push(int[], int[], int&, int, int);
void pop(int[], int[], int &, int &, int &);
void insertSort(int[], int);

#define swap( x, y ) { int t=x; x=y; y=t; }


void quickSortIter(int array[], int size)
{
	int top, bottom;			// bounds of current subarray
	int pivotPos;			// separation between partitions

	int *topStack = new int[size / 2];	// some array stacks
	int *botStack = new int[size / 2];	//   for a to do list
	int depth = 0;			// initially empty
	bool naiveQuick;

	if (size < 200000)  	naiveQuick = true;
	else  					naiveQuick = false;
		

	push(botStack, topStack, depth, 0, size - 1);

	while (depth > 0)		// while there is work to do
	{
		pop(botStack, topStack, depth, bottom, top);
		if (top < bottom + 5)	// small areays should use some other sort
			insertSort(array + bottom, top - bottom + 1);
		else
		{
			// try to get a good choice of median
			if (!naiveQuick)
			{
				sortThree(array[(top + bottom) / 2], array[bottom], array[top]);
				// 'naive' just picks the first one -- bad choice when sorted
			}
			quickSortPartition(array, bottom, top, pivotPos);

			if (pivotPos > bottom)
				push(botStack, topStack, depth, bottom, pivotPos - 1);
			if (pivotPos < top)
				push(botStack, topStack, depth, pivotPos + 1, top);
		}
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


inline void sortThree(int& a, int& b, int& c)
{
	if (a>b)  swap(a, b);
	if (a>c)  swap(a, c);
	if (b>c)  swap(b, c);
	//   if ((a<b) || (b<c)) cout << "sortThree failed!" << endl;
}



//  real short array-based stack, for iterative Quick sort to use
void push(int botStack[], int topStack[], int &depth, int bottom, int top)
{
	topStack[depth] = top;
	botStack[depth] = bottom;
	depth++;
}

void pop(int botStack[], int topStack[], int &depth, int &bottom, int &top)
{
	depth--;
	top = topStack[depth];
	bottom = botStack[depth];
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

