#ifndef ANALYZE_H
#define ANALYZE_H

#include <stdbool.h>

#define ITERATIONS 64
//#define ITERATIONS 1
#define ITERATIONS_SEARCH (ITERATIONS*1024)
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

// model info, eg for O(n^2)
typedef struct {
    double (*fp) (double);
    char * name;
    double avg;
    double sd;
    double k;
} model_t;

// benchmark benchmarks an algorithm a for a specific case c, writing n results
// to the result buffer buf
void benchmark(const ac_t ac, result_t *buf, int n, int start_size);


double linearRegression(double (*points)[2], int n);

void calcModelData(model_t *m, result_t *r, int ms, int rs);

double c_res(model_t m, result_t r);
#endif
