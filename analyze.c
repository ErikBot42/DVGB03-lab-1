#include "analyze.h"
#include "algorithm.h"

#include "ui.h" // TODO: remove when no longer debugging

#include <stdio.h> // TODO: remove when no longer needed for debugging

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
	//printf("SORTED!!!\n");
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
	//printf("RANDOM!!!\n");
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
	int maxVal = RAND_MAX;//100;
	int searchIndex = 0;
	switch(a)
	{
		case bubble_sort_t:
			//printf("BUBBLE SORT");
			switch(c)
			{
				case best_t:
					generateSortedList(d,n,false);
					break;
				case worst_t:
					generateSortedList(d,n,true);
					break;
				case average_t:
					//printf("	RANDOM LIST");
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
	{
		time_t t;
		srand((unsigned) time(&t));	
	}

	
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
			break;
		case binary_search_t:
			*searchResult = binary_search(d, n, v);
			break;
	}

	clock_t t2 = clock();

	return ((double)(t2-t))/CLOCKS_PER_SEC;;

}

bool isSorted(int *d, int n)
{
	for (int i = 0; i<n-1; i++)
	{
		if (d[i]>d[i+1])
		{
			return false;
		}
	}
	return true;
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

	int numTests = 1;//ITERATIONS;


	for (int i = 0; i<n; i++) // changing size
	{
		int d[size];

		//printf("CURRENT SIZE: %d", size);
		// TODO: REMOVE THIS WHEN DEBUGGING IS DONE
		
		double averageTime = 0;
		for (int j = 0; j<numTests; j++) // multiple iterations at a given size
		{
			int v = generateTestList(a,c,d,size);
			bool searchResult;
			//printf("debug> before:\n");
			//ui_DEBUG_print_list(d, size);

			averageTime += runTimedBenchmark(a,d,size,v,&searchResult)/((double)numTests);

			//printf("debug> after:\n");
			//ui_DEBUG_print_list(d, size);
			if (!isSorted(d, size))
			{
				printf("debug> LIST IS NOT SORTED!!!!!!!!!!!!!!!!");
				//TODO: verify result
			}

		}
		
		// TODO: REMOVE THIS WHEN DEBUGGING IS DONE


		buf[i].size = size;
		buf[i].time = averageTime;

		size *= 2;
	}
}




