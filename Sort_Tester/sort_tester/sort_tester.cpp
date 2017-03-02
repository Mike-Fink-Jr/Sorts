// A Cavalcade of Sorts
// This program implements several different sorting algorithms
// for illustration, comparison, etc.
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

// Some function prototypes for all the sorts themselves
void selectSort(int[], int);		// selection sort
void insertSort(int[], int);		// insertion sort
void bubbleSort(int[], int);		// bubble sort
void mergeSort(int[], int);		// merge sort
void quickSort(int[], int);		// quick sort
void quickSortIter(int[], int);	// iterative quick sort
void radixSort(int[], int);		// radix sort
void shellSort1(int[], int);		// shell sort, strides 2^k-1
void shellSort2(int[], int);		// shell sort, strides 2^p 3^q
void heapSort(int[], int);		// heap sort
void bucketSort(int[], int);		// 100000 buckets, using insertion

// Some helper functions to assist with the sorting above
void quickSortPartition(int[], int, int, int &);
void heapInsert(int[], int &, int);
void heapRemove(int[], int &, int &);

// Some utility to functions to run and test the sorting functions
void randomize(int[], int);		// make some random data
void verify(const int[], int);		// verify the sorts worked!
void test(const char[], int size, void(*)(int[], int));

// And this makes swap into an inline function, so it is slightly faster
#define swap( x, y ) { int t=x; x=y; y=t; }

// Arrays of gap sizes for the Shell Sort, placed here globally
// so they can be initialized before the sorts begin, so they
// don't have to get a time penalty initializing them.
int powMinus[25], twoThree[300], twoSize;

// This is also made global for no other reason than to allow
// all of the sort function parameters lists look the same
bool naiveQuick;		// governs how quick sort picks its pivot

// Main Program
// Initializes the arrays of gap sizes fot the Shell Sorts,
// and then proceeds to run several tests on each sort.
int main()
{
	int stride;
	stride = 1;			// all array values here are 2^k-1
	for (int i = 0; i<25; i++)
	{				// noting that 2^(k+1) - 1 =
		powMinus[i] = stride;	//    2*(2^k-1) + 1
		stride = 2 * stride + 1;
	}
	twoSize = 0;		// all array values are 2^p * 3^q
	for (int twoPow = 1; twoPow < 10000000; twoPow += twoPow)
	{
		for (stride = twoPow; stride < 10000000; stride *= 3)
		{
			twoThree[twoSize] = stride;
			twoSize++;
		}
	}
	insertSort(twoThree, twoSize);	// and these values must be sorted

	//  The real stuff happens after this point

	for (int size = 25000; size <= 15000000; size += size)
	{
		cout << "size " << size << "\t" << "random" << "\t" << "sorted"
			<< "\t" << "reverse" << "\t" << "two out" << endl;
		if (size < 200000)	// beyond here, quadratic sorts are slow!
		{
			test("Selection sort", size, selectSort);
			test("Bubble sort", size, bubbleSort);
			test("Insertion sort", size, insertSort);
			naiveQuick = true;
			test("Naive Quicksort", size, quickSortIter);
		}

		if (size > 100000)
		{
			naiveQuick = false;
			if (size < 2000000)
				test("Quick sort", size, quickSort);
			test("Iterative Qsort", size, quickSortIter);
			test("Merge sort", size, mergeSort);
			test("Bucket sort", size, bucketSort);
			test("Radix sort", size, radixSort);
			test("Heap sort", size, heapSort);
		}

		test("Shell gap 2^k-1", size, shellSort1);
		//	test("Shell gap 2p3q", size, shellSort2);
		cout << endl;
	}
	return 0;
}

//  test
//  Tests and analyzes the performance of various sorting methods
//  Every sort call is surrounded by calls that consult the system clock
//  to see how much time has elapsed during that sort operation.
//  Parameters:
//      name	(string input)		name of sort to test
//      size	(integer input)		array sizes to use
//      sort	(function input)	implementation of sort to test
void test(const char name[], int size, void sort(int[], int))
{
	int *array = new int[size];
	int tick, tock;

	randomize(array, size);		// make up some data
	tick = clock();
	sort(array, size);			// sort it
	tock = clock();
	verify(array, size);		// and check that it is sorted
	cout << name << "\t" << ((tock - tick)*1.0 / CLOCKS_PER_SEC) << flush;
	tick = clock();
	sort(array, size);			// sort an array that's already sorted
	tock = clock();
	verify(array, size);
	cout << "\t" << ((tock - tick)*1.0 / CLOCKS_PER_SEC) << flush;
	for (int i = 0, j = size - 1; i<j; i++, j--)
		swap(array[i], array[j]);	// place in reverse sorted order
	tick = clock();
	sort(array, size);			// and try to reverse it back
	tock = clock();
	cout << "\t" << ((tock - tick)*1.0 / CLOCKS_PER_SEC) << flush;
	verify(array, size);		// make sure it is all still sorted
	swap(array[size / 3], array[2 * size / 3]);	// 2 out of order
	tick = clock();
	sort(array, size);			// and try to reverse it back
	tock = clock();
	cout << "\t" << ((tock - tick)*1.0 / CLOCKS_PER_SEC) << endl;
	verify(array, size);		// make sure it is all still sorted

	delete[] array;
}

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

void quickSort(int array[], int size)
{
	quickSortRecurse(array, 0, size - 1);
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

void quickSortIter(int array[], int size)
{
	int top, bottom;			// bounds of current subarray
	int pivotPos;			// separation between partitions

	int *topStack = new int[size / 2];	// some array stacks
	int *botStack = new int[size / 2];	//   for a to do list
	int depth = 0;			// initially empty


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

void shellSort1(int array[], int size)
{
	shellSort(array, size, powMinus, 25);
}

void shellSort2(int array[], int size)
{
	shellSort(array, size, twoThree, twoSize);
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

//  And the utility functions to manage everything
void randomize(int array[], int size)
{
	for (int i = 0; i<size; i++)
		array[i] = rand();
}

void verify(const int array[], int size)
{
	int sorted = 1;		// assume true
	for (int i = 1; i<size; i++)
		if (array[i] < array[i - 1])
			sorted = 0;		// not increasing!
	if (!sorted)
		cout << endl << "This array is not sorted!" << endl;
}