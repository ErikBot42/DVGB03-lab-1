#include "analyze.h"
#include "algorithm.h"

#include <stdlib.h>
#include <time.h>


//
// Private
//
//
//
// post:
// generate sorted list with elements [0,n-1] or [n-1,0] if reverse is true.
static void generateSortedList(int *d, int n, bool reverse)
{
	for (int i = 0; i<n; i++)
	{
		if (!reverse)
			d[i] = i;
		else
			d[i] = n-i-1;
	}
}

// pre: srand() has been run at some point.
// post: [0,maxValue]
static void generateRandomList(int *d, int n, int maxValue)
{
	for (int i = 0; i<n; i++)
	{
		d[i] = rand()%(maxValue+1);
	}
}


static int randomIndex(int list_size)
{
	return rand()%list_size;
}

// pre: d is a pre allocated buffer of length n && n>0.
// post: generate apropriate list based on algorithm and case also returns a
// value to search for if using a search algorithm.
static int generateTestList(const algorithm_t a, const case_t c, int *d, int n)
{
	int maxVal = RAND_MAX;
	int searchIndex = 0;
	switch(a)
	{
		case bubble_sort_t:
			switch(c)
			{
				case best_t:
					generateSortedList(d,n,false);
					break;
				case worst_t:
					generateSortedList(d,n,true);
					break;
				case average_t:
					generateRandomList(d,n,maxVal);
					break;
			}
			break;
		case binary_search_t:
			generateSortedList(d,n,false);
			searchIndex = randomIndex(n);
			break;
		case linear_search_t:
			switch(c)
			{
				case best_t:
					searchIndex = 0;
					break;
				case worst_t:
					searchIndex = n-1;
				case average_t:
					randomIndex(n);
			}
			generateSortedList(d,n,false);
			break;
		default:
			//TODO: implement best/worst creation of list for other sort algorithms.
			generateRandomList(d,n,maxVal);
			break;
	}
	
	return d[searchIndex];
}

static double runTimedBenchmark(const algorithm_t a, int *d, int n, int v, bool* searchResult)
{
	// function pointers was decided against to make it possible to test both sorting and searching in a single function.
	

	
	clock_t t = clock();
	
	// the overhead from the switch is assumed to be completely negligable.
	switch(a)
	{
		case bubble_sort_t:
			bubble_sort(d, n);
			break;
		case insertion_sort_t:
			insertion_sort(d, n);
			break;
		case quick_sort_t:
			quick_sort(d, n);
			break;
		case linear_search_t:
			*searchResult = linear_search(d, n, v);
		case binary_search_t:
			*searchResult = binary_search(d, n, v);
	}

	clock_t t2 = clock();

	return ((double)(t2-t))/CLOCKS_PER_SEC;

}

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
//
// run a number of benchmarks for a variable size
//
// TODO: verify sort/search, search for elements that do not exist 
void benchmark(const algorithm_t a, const case_t c, result_t *buf, int n)
{
	int size = SIZE_START;

	int numTests = 1;


	for (int i = 0; i<n; i++) // changing size
	{
		int d[size];
		
		double averageTime = 0;
		for (int j = 0; j<numTests; j++) // multiple iterations at a given size
		{
			int v = generateTestList(a,c,d,size);
			bool searchResult;
			averageTime += runTimedBenchmark(a,d,n,v,&searchResult)/((double)numTests);

			//TODO: verify result
		}

		buf[i].size = size;
		buf[i].time = averageTime;

		size *= 2;
	}
}




