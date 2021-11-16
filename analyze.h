#ifndef ANALYZE_H
#define ANALYZE_H

#include <stdbool.h>

#define ITERATIONS 16
#define ITERATIONS_SEARCH (ITERATIONS*8)
//#define ITERATIONS_SEARCH 1

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

// algorithm and case inside a single struct
typedef struct {
    algorithm_t a;
    case_t c;
} ac_t;



// benchmark benchmarks an algorithm a for a specific case c, writing n results
// to the result buffer buf
void benchmark(const ac_t ac, result_t *buf, int n, int start_size);

#endif
