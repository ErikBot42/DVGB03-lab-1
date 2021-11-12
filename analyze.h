#ifndef ANALYZE_H
#define ANALYZE_H

#include <stdbool.h>

#define SIZE_START 512
//#define SIZE_START 8
//#define ITERATIONS 4
#define ITERATIONS 256

// algorithm_t defines different algorithms
typedef enum {
	bubble_sort_t,
	insertion_sort_t,
	quick_sort_t,
	linear_search_t,
	binary_search_t
} algorithm_t;

// case_t defines different cases
typedef enum {
	best_t,
	worst_t,
	average_t,
} case_t;

// result_t defines a timed measurement for a given array size
typedef struct {
	int size;
	double time;
} result_t;


//void generateSortedList(int *d, int n, bool reverse);


// benchmark benchmarks an algorithm a for a specific case c, writing n results
// to the result buffer buf
void benchmark(const algorithm_t a, const case_t c, result_t *buf, int n);

#endif
