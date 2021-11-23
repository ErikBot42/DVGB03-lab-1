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


// post:
// generate sorted list with elements [0,n-1] or [n-1,0] if reverse is true.
static void generateSortedList(int *d, int n, bool reverse)
{
    //if (ui_debug()) printf("gen: sorted\n");
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


// pre: n=k^2 && k > 0
// because of data reuse, this algorithm is O(n*log(n))
// produces a best case for quick sort where all elements are unique
static void quickSortBestCase_rec(int *d, int n)
{
    if (n <= 1)
    {
        d[0] = 1;
    }
    else 
    {
        int m = n/2;
        quickSortBestCase_rec(d+m,m);

        // add, reverse and put in front
        for (int i = 0; i<m; i++)
        {
            int j = (n-1)-i;
            d[i] = d[j] + m;
        }
    }
}


// pre: n=k^2 && k > 0
// Caches previous result
static void quickSortBestCase(int *d, int n)
{

    static int *d_cached = NULL; // cached list
    static int n_cached = 0; // num of elements in cache

    // check if cache valid, if not remove it.
    if (d_cached != NULL)
    {
        if (n_cached == n)
        {
            memcpy(d, d_cached, n*sizeof(int));
            return;
        }
        else
        {
            free(d_cached);
            d_cached = NULL;
            n_cached = 0;
        }
    }

    if (d_cached == NULL)
    {
        //if (ui_debug()) printf("gen: qsort best case\n");
        //if (ui_debug()) printf("NEW LIST!!! \n");
        quickSortBestCase_rec(d,n);

        if (true) // should values be cached?
        {
            d_cached = (int*) malloc(sizeof(int)*n);
            memcpy(d_cached, d, n*sizeof(int));
            n_cached = n;
        }
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
            break;
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
            //        default:
            //            err_case_not_implemented();         
            //            generateRandomList(d,n,maxVal);
            //            break;
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

#ifndef USE_POSIX_COMPLIANT    
    clock_t t = clock();
#else
#define CLOCK_THAT_IS_USED CLOCK_MONOTONIC
    struct timespec before;
    clock_gettime(CLOCK_THAT_IS_USED, &before);
#endif 

    // the overhead from the switch is assumed to be completely negligable.
    switch(a)
    {

        case bubble_sort_t: bubble_sort(d, n); break; 
        case insertion_sort_t: insertion_sort(d, n); break; 
        case quick_sort_t: quick_sort(d, n); break; 
        case linear_search_t: *searchResult = linear_search(d, n, v); break; 
        case binary_search_t: *searchResult = binary_search(d, n, v); break; 
    }

#ifndef USE_POSIX_COMPLIANT
    clock_t t2 = clock();
    return ((double)(t2-t))/CLOCKS_PER_SEC;;
#else
    struct timespec after;
    clock_gettime(CLOCK_THAT_IS_USED, &after);
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
// start with SIZE_START, double each time
//
// run a number of benchmarks for a variable size
//
void benchmark(const ac_t ac, result_t *buf, int n, int start_size)
{

    bool isSearchingAlgorithm = ac.a == linear_search_t || ac.a == binary_search_t;

    int size = SIZE_START;
    int numTests = isSearchingAlgorithm ? ITERATIONS_SEARCH : ITERATIONS;
    int numPreheat = numTests/4; // discard these benchmarks, used to fill cpu cache/similar stuff

    for (int i = 0; i<n; i++) // changing size
    {
        //int d[size];

        // stack is very limited (about 1 MB), allocating on heap instead.
        if ((RAND_MAX)<size) ui_debug("LIST SIZE IS GREATER THAN RAND_MAX!\n");
        ui_debug("allocating new array.\n");

        int *d = (int *) malloc(sizeof(int)*(size));
        //int *d2 = (int *) malloc(sizeof(int)*(size));

        if (d == NULL)
        {
            printf("error> %d bytes of memory could not be allocated.\n", (int)(size/sizeof(int)));
            break;
        }

        volatile double averageTime = 0;
        bool debug = ui_debug("");
        bool cache = false; // should old list be reused?
        for (int j = 0; j<(numTests + numPreheat); j++) // multiple iterations at a given size
        {
            if (debug) { printf("."); fflush(stdout); }
            int v;

            //if (cache) memcpy(d,d2,size); // O(n) << O(n*log(n))
            //if (!cache) 
            v = generateTestList(ac,d,size,&cache);
            //if (debug) {ui_DEBUG_print_list(d, size); printf("\n\n\n");}
            //if (debug && isSorted(d, size)) {fflush(stdout); printf("INPUT LIST (%d elements) IS ALREADY SORTED!!!", size); exit(1);}
            //if (cache && j==0) memcpy(d2,d,size);
            bool searchResult = true;


            // hopefully forces gcc to actually run the function
            volatile double time = runTimedBenchmark(ac.a,d,size,v,&searchResult);
            if (!(j<numPreheat))
            {
                averageTime += time/((double)numTests);
                if (debug) { printf("_"); fflush(stdout); }
            }
            else if (debug) { printf("|"); fflush(stdout); }
            

            if (!isSearchingAlgorithm && !isSorted(d, size)) ui_error("LIST WAS NOT SORTED!\n");
            if (isSearchingAlgorithm && !searchResult) ui_error("ALGORITHM COULD NOT FIND ELEMENT!\n");

        }
        if (debug) printf("\n");

        free(d);

        buf[i].size = size;
        buf[i].time = averageTime;

        size *= 2;
    }
}


// perform linear regression to get slope.
// pre: n>=2, sorted
// https://en.wikipedia.org/wiki/Simple_linear_regression#Fitting_the_regression_line
// O(n) | n is a constant => O(1)
double linearRegression(double (*points)[2], int n)
{
    double dn = (double)n;
    double s_x, s_y, s_xy, s_x2;
    for (int i = 0; i<n; i++)
    {
        s_x+=points[i][0];
        s_y+=points[i][1];
        s_x2+=points[i][0]*points[i][0];
        s_xy+=points[i][0]*points[i][1];
    }

    //double a_x = s_x/dn;
    //double a_y = s_y/dn;

    //double t_1 = s_x2 - s_x * a_x;
    //double t_2 = s_xy - s_x * a_x;
    //return t_2/t_1;
    return (dn*s_xy-s_x*s_y)/(dn*s_x2*s_x*s_x);

}

double c_res(model_t m, result_t r) {return r.time/(*(m.fp))(r.size);}

// fill the model struct
void calcModelData(model_t *m, result_t *r, int ms, int rs)
{
    for (int j = 0; j<ms; j++) 
    {
        double points[rs][2]; // for linear regression
        
        // k, avg
        for (int i = 0; i<rs; i++)
        {   
            double res = c_res(m[j], r[i]);

            m[j].avg += res/((double) rs);

            points[i][0] = r[i].size;
            points[i][1] = res;

        }   
        m[j].k = linearRegression(points, rs);

        // sd
        for (int i = 0; i<rs; i++)
        {
            double res = c_res(m[j], r[i]);
            double err = res - m[j].avg; 
            double squared = err*err;
            m[j].sd += squared;
        }
        m[j].sd = sqrt(m[j].sd/((double) rs));
    }

}
