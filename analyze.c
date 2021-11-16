#include "analyze.h"
#include "algorithm.h"

#include "ui.h" 
#include <stdio.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define USE_POSIX_COMPLIANT



//
// Private
//
//

// post: swap the value the pointers point to.
// copied from algorithm.c to make sure algorithm.c remains completely independent.
static inline void swp (int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// random value with the max size of an int
// device dependant, could be used to make sure rand() gives enough bits
static inline int intRand()
{
    return rand();
    //int n = log2(RAND_MAX); // this line will hopefully be optimized away by the compiler...
    //int maxVal = 1 << n;

    //int maxIntSizeLog2 = sizeof(int);
    //int maxIntSize = 1 << maxIntSizeLog2;

    //if (maxVal == maxIntSize) return rand();
    //
    //printf("RAND_MAX IS VERY LOW: %d", RAND_MAX);
    //exit(1);

}

// post:
// generate sorted list with elements [0,n-1] or [n-1,0] if reverse is true.
static void generateSortedList(int *d, int n, bool reverse)
{
    if (ui_debug()) printf("NEW sorted list\n");
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
		d[i] = intRand()%(maxValue+1);
	}
}


static int randomIndex(int list_size)
{
	return intRand()%list_size;
}

//static void err_case_not_implemented()
//{
//    if(ui_error()) printf("Case not implemented for this algorithm, using a random list\n");
//}

// n = 2^k for this to work well, otherwise behavior is undefined
// because of memcpy, this algorithm is O(n*log(n))
// produces a best case for quick sort where all elements are unique
static void quickSortBestCase_rec(int *d, int n)
{
    if (n <= 2)
    {
        d[0] = 1;
        d[1] = 2;
    }
    else 
    {
        int m = n/2;
        quickSortBestCase_rec(d,m);
        //memcpy(d+m,d,m);
        quickSortBestCase_rec(d+m, m);

        for (int i = 0; i<m; i++) d[i] += m;
    }
}

static void quickSortBestCase(int *d, int n)
{
    static int *d_cached = NULL;
    static int n_cached = 0;
    if (d_cached != NULL)
    {
        if (n_cached == n)
        {
            memcpy(d, d_cached, n);
            return;
        }
        free(d_cached);
        d_cached = NULL;
    }
    else
    {
        quickSortBestCase_rec(d,n);
        d_cached = (int*) malloc(sizeof(int)*n);
        memcpy(d_cached, d, n);
        n_cached = n;
    }
}

// pre: d is a pre allocated buffer of length n && n>0.
// post: generate apropriate list based on algorithm and case also returns a
// value to search for if using a search algorithm.
static int generateTestList(const ac_t ac, int *d, int n, bool *cache)
{
    //if (ui_debug()) printf("Generating NEW test list\n");
	int maxVal = RAND_MAX;//100;
	int searchIndex = 0;
	switch(ac.a)
	{
		case bubble_sort_t:
        case insertion_sort_t:
			switch(ac.c)
			{
				case best_t:
					if (!*cache) generateSortedList(d,n,false);
                    *cache = true;
					break;
				case worst_t:
					generateSortedList(d,n,true);
					break;
				case average_t:
					generateRandomList(d,n,maxVal);
					break;
			}
			break;
        case quick_sort_t:
            switch(ac.c)
            {
                case best_t:
                    //if (ui_debug()) printf("quicksort best case\n");
                    quickSortBestCase(d,n);
                    break;
                case worst_t:
					if (!*cache) generateSortedList(d,n,false);
                    *cache = true;
                    break;
                case average_t:
					generateRandomList(d,n,maxVal);
                    break;
            }
		case binary_search_t:
            // constant in terms of list contents.
            switch(ac.c)
            {
                case best_t:
                    searchIndex = (n-1)/2;
                    break;
                case worst_t:
                    searchIndex = 0;
                    break;
                case average_t:
                    searchIndex = randomIndex(n);
                    break;
            }
			if (!*cache) generateSortedList(d,n,false);
            *cache = true;
			break;
		case linear_search_t:
			switch(ac.c)
			{
				case best_t:
					searchIndex = 0;
					break;
				case worst_t:
					searchIndex = n-1;
				case average_t:
					searchIndex = randomIndex(n);
			}
			if (!*cache) generateSortedList(d,n,false);
            *cache = true;
			break;
//		default:
//            err_case_not_implemented();         
//			generateRandomList(d,n,maxVal);
//			break;
	}
	
    //if (ui_debug()) printf("index = %d/%d\n", searchIndex, n-1);
	return d[searchIndex];
}

static double runTimedBenchmark(const algorithm_t a, int *d, int n, int v, bool* searchResult)
{
	// function pointers was decided against to make it possible to test both sorting and searching in a single function.
	{
		time_t t;
		srand((unsigned) time(&t));	
	}

#ifndef USE_POSIX_COMPLIANT	
    clock_t t = clock();
#else
    struct timespec before;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &before);
#endif 
	
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

#ifndef USE_POSIX_COMPLIANT
	clock_t t2 = clock();
	return ((double)(t2-t))/CLOCKS_PER_SEC;;
#else
    struct timespec after;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &after);
    double ndiff = (after.tv_nsec-before.tv_nsec);
    double sdiff = (after.tv_sec-before.tv_sec);
    return (ndiff/(1E9))+sdiff; // nanosecond->second

#endif



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
void benchmark(const ac_t ac, result_t *buf, int n, int start_size)
{

    bool isSearchingAlgorithm = ac.a == linear_search_t || ac.a == binary_search_t;

	int size = SIZE_START;
	int numTests = isSearchingAlgorithm ? ITERATIONS_SEARCH : ITERATIONS;

	for (int i = 0; i<n; i++) // changing size
	{
		//int d[size];
        
        // stack is very limited (about 1 MB), allocating on heap instead.
        if ((RAND_MAX)<size && ui_debug()) printf("LIST SIZE IS GREATER THAN RAND_MAX!\n");
        if (ui_debug()) printf("allocating %d bytes.\n", (int)(size*sizeof(int)));

        int *d = (int *) malloc(sizeof(int)*(size));
        //int *d2 = (int *) malloc(sizeof(int)*(size));

        if (d == NULL)
        {
            printf("error> %d bytes of memory could not be allocated.\n", (int)(size/sizeof(int)));
            break;
        }
		
		double averageTime = 0;
        bool debug = ui_debug();
        bool cache = false; // should old list be reused?
		for (int j = 0; j<numTests; j++) // multiple iterations at a given size
		{
            if (debug) { printf("."); fflush(stdout); }
            int v;

            //if (cache) memcpy(d,d2,size); // O(n) << O(n*log(n))
            //if (!cache) 
            v = generateTestList(ac,d,size,&cache);
            //if (cache && j==0) memcpy(d2,d,size);
			bool searchResult = true;

            if (debug) { printf("_"); fflush(stdout); }
			averageTime += runTimedBenchmark(ac.a,d,size,v,&searchResult)/((double)numTests);

			if (!isSorted(d, size)) printf("error> LIST WAS NOT SORTED!\n");
			if (!searchResult) printf("error> ALGORITHM COULD NOT FIND ELEMENT!\n");

		}
        if (debug) printf("\n");

        if (debug) printf("releasing d\n");
        free(d);
        //free(d2);

		buf[i].size = size;
		buf[i].time = averageTime;

		size *= 2;
	}
}




