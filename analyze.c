#include "analyze.h"
#include "algorithm.h"

//#include <stdlib.h>


//
// Private
//
//
//
// post:
// generate sorted list with elements [0,n-1] or [n-1,0] if reverse is true.
/*void generateSortedList(int *d, int n, bool reverse)
{
	for (int i = 0; i<n; i++)
	{
		if (!reverse)
			d[i] = i;
		else
			d[i] = n-i-1;
	}
}

void generateRandomList(int *d, int n, int maxValue)
{
	int a;
}


//TODO: VARIABLE RANDOM RANGE
void generateTestList(const algorithm_t a, const case_t c, int *d, int n)
{
	switch(a)
	case bubble_sort_t: // BUBBLE SORT:
		switch(c):
	case best_t:
			generateSortedList(d,n);
	case worst_t:
			generateSortedList(d,n,true);
	case default:
			int a;

	default:
			//TODO: implement creation of random list
			generateSortedList(d,n);
			//switch		
}*/

//
// Public
//
// pre:
// result_t *buf already allocated.
//
//
// start with SIZE_START, double each time
// -> table to print
// run until time is too large?
// fp for timer.
void benchmark(const algorithm_t a, const case_t c, result_t *buf, int n)
{
	int size = SIZE_START;
	for (int i = 0; i<n; i++)
	{
		
		
		size = size << 1;
	}
}




