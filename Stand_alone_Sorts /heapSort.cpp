#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
#define swap( x, y ) { int t=x; x=y; y=t; }


void heapSort(int[], int);		// heap sort
void heapInsert(int[], int &, int);
void heapRemove(int[], int &, int &);

//  Heap Sort
//  Strategy:  use the front of the array as a max heap
//             insert values into the heap from beginning to end
//             then extract to fill array from end to beginning
//             the largest value will be removed first to end of the array
//  NOTE: Since the array data uses 0 as the first subscript
//        and the heap uses 1 as its first subscript
//        we will pass an adjusted pointer to the heap for the offset
//  Also NOTE: Both heapInsert and heapRemove modify they value of
//  	'heapSize', so the given invariants to reflect changes!
void heapSort(int array[], int size)
{
	int heapSize = 1;		// pretend heap has 1 sorted value
	int maximum;

	//  array[0] is one-element heap; entire array is unsorted
	while (heapSize < size)
	{
		// array[ 0 .. heapSize-1 ] is a heap; rest of array is unsorted
		heapInsert(array - 1, heapSize, array[heapSize]);
		// array[ 0 .. heapSize-1 ] is a heap; rest of array is unsorted
	}
	// array[ 0 .. size-1 ] is a heap
	while (heapSize > 1)
	{
		// array[0 .. heapSize-1 ] is a heap; rest of array is sorted
		heapRemove(array - 1, heapSize, maximum);	// get max value      
		array[heapSize] = maximum;		// and place after heap
		// array[0 .. heapSize-1 ] is a heap; rest of array is sorted
	}
	// entire array is sorted
}




//  A couple heap operations to support the heap sort
//  A max heap is chosen here, to facilitate sorting in increasing order

//  heapInsert
//  Inserts a new value into a heap whose root is represented in heap[1]
//  Parameters:
//  	heap	(modified int array)	actual heap (implemented as an array)
//  	size	(modified int)		number of elements in the heap
//  	value   (input int)		value to insert
//  Pre-Conditions and Post-conditions
//      heap follows all the requirements of a max heap (balance and ordering)
//  NOTE: size is the last occupied element in the heap (size 1 occupies heap[1])
void heapInsert(int heap[], int &size, int value)
{
	int pos;			// position in heap currently considered
	//  for all i, (2 <= i <= size): heap[floor(i/2)] > heap[i]
	size++;
	pos = size;
	//  for all i, (2 <= i <= size && i != pos):  heap[floor(i/2)] > heap[i]
	while (pos > 1 && heap[pos / 2] < value)
	{
		// for all i, (2 <= i <= size && i != pos):  heap[floor(i/2)] > heap[i]
		heap[pos] = heap[pos / 2];
		// for all i, (2 <= i <= size && i != pos/2):heap[floor(i/2)] > heap[i]
		pos = pos / 2;
		// for all i, (2 <= i <= size && i != pos):  heap[floor(i/2)] > heap[i]
	}
	heap[pos] = value;
	//  for all i, (2 <= i <= size):  heap[floor(i/2)] > heap[i]
}

//  heapRemove
//  Removes and replaces the root of a heap, represented in heap[1]
//  Parameters:
//  	heap	(modified int array)	actual heap (implemented as an array)
//  	size	(modified int)		number of elements in the heap
//  	value   (output int)		value removed from the root
//  Pre-Conditions and Post-conditions
//      heap follows all the requirements of a max heap (balance and ordering)
//  NOTE: size is the last occupied element in the heap (size 1 occupies heap[1])
void heapRemove(int heap[], int &size, int &value)
{
	int pos;				// position of value to consider
	int child;				// position of child to consider
	int sinker;				// value to sink from root downwards
	bool done = false;			// not yet done adjustments

	value = heap[1];			// obtain return value
	//  for all i, (2 <= i <= size):  heap[floor(i/2)] > heap[i]
	sinker = heap[size];		// the value that would be at the root
	size--;
	pos = 1;
	child = 2;				// left child of pos
	//  for all i, (2 <= i <= size && floor(i/2)!=pos): heap[floor(i/2)] > heap[i]
	while (child <= size && !done)	// still within the heap data
	{
		// for all i, (2 <= i <= size && floor(i/2)!=pos): heap[floor(i/2)] > heap[i]
		if (child < size && heap[child] < heap[child + 1])
			child = child + 1;		// refer to greater child value
		if (sinker > heap[child])
			done = true;		// invariant holds without exception
		else
		{
			heap[pos] = heap[child];
			// for all i, (2 <= i <= size && floor(i/2)!=child): heap[floor(i/2)] > heap[i]
			pos = child;
			// for all i, (2 <= i <= size && floor(i/2)!=pos): heap[floor(i/2)] > heap[i]
			child = 2 * pos;		// proceed down the heap
		}
	}
	heap[pos] = sinker;			// put the value in
	//  for all i, (2 <= i <= size): heap[floor(i/2)] > heap[i]
}

