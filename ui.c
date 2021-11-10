#include "ui.h"
#include "io.h"
#include "analyze.h"

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

//
// Private
//
static void ui_invalid_input()
{
	printf("info> bad input\n");
}

static void ui_exit()
{
	printf("info> bye\n");
}

static char ui_get_choice()
{
	char buf[3];

	printf("input> ");
	return read_line(buf, 3) ? buf[0] : 0;
}

static void ui_line(char c, int n)
{
	while (n-- > 0) {
		putchar(c);
	}
	putchar('\n');
}

static void ui_menu_options(const char *options[], int num_options)
{
	int i;

	for (i=0; i<num_options; i++) {
		printf("    %c) %s\n", 'a'+i, options[i]);
	}
}

// just directly prints the algorithm so that I don't have to do memory management
static void ui_print_algorithm(algorithm_t a)
{
	switch(a)
	{
		case bubble_sort_t:    printf("Bubble sort"); break;
		case insertion_sort_t: printf("Insertion sort"); break;
		case quick_sort_t:     printf("Quick sort"); break;
		case linear_search_t:  printf("Linear search"); break;
		case binary_search_t:  printf("Binary search"); break;
	}
}
static void ui_print_algorithm_full(algorithm_t a)
{
	printf("Current algorithm: "); ui_print_algorithm(a); printf("\n");
}

// just directly prints the case so that I don't have to do memory management
static void ui_print_case(case_t c)
{
	switch(c)
	{
		case best_t:    printf("Best"); break;
		case worst_t:   printf("Worst"); break;
		case average_t: printf("Average"); break;
	}
	
}
static void ui_print_case_full(algorithm_t a)
{
	printf("Current case: "); ui_print_case(a); printf("\n");
}

static void ui_menu(algorithm_t a, case_t c)
{
	const char *options[] = {
		"Menu",
		"Exit",
		"Run benchmark\n",
		"Bubble sort",
		"Insertion sort",
		"Quick sort",
		"Linear search",
		"Binary search\n",
		"Best case",
		"Worst case",
		"Average case",

		// TODO: complete menu
	};

	ui_line('=', MENU_WIDTH);
	ui_print_algorithm_full(a); 
	ui_print_case_full(c); 
	ui_line('-', MENU_WIDTH);
	ui_menu_options(options, sizeof(options) / sizeof(char *));
	ui_line('-', MENU_WIDTH);
}

static double O_n(double n) {return n;}
static double O_n2(double n) {return n*n;}
static double O_n3(double n) {return n*n*n;}
static double O_n_log_n(double n) {return n*log(n);}

// Long, but splitting it would reduce readability and would not significantly reduce size.
static void ui_results(result_t *results, int n)
{
	int width = 10;

	model_t models[] = {
		{O_n,       "O(n)"},
		{O_n2,      "O(n^2)"},
		{O_n3,      "O(n^3)"},
		{log,       "O(log(n))"},
		{O_n_log_n, "O(n*log(n))"},
		{sqrt,      "O(sqrt(n))"},
	}; int num_models = sizeof(models)/sizeof(models[0]);
	printf("\nResults:\n\n");

	printf("%*s ", width+3, "size"); for (int j = 0; j<num_models; j++) { printf("%*s ", width+3, models[j].name); } printf("\n");

	for (int i = 0; i<n; i++)
	{
		printf("%*d ", width+3, results[i].size);
		for (int j = 0; j<num_models; j++) 
		{ 
			double res = results[i].time/(*(models[j].fp))(results[i].size);
			models[j].avg += res/((double) n);
			printf("% *e ", width, res); 
		}
		printf("\n");
	}

	for (int j = 0; j<num_models; j++) 
	{
		for (int i = 0; i<n; i++)
		{
			double res = results[i].time/(*(models[j].fp))(results[i].size); res -= models[j].avg; res*=res;
			models[j].sd += res/((double) n);
		}
		models[j].sd = sqrt(models[j].sd);
	}

	printf("\n%*s ", width+3, "avg"); for (int j = 0; j<num_models; j++) { printf("% *e ", width, models[j].avg); } printf("\n");
	printf("%*s ", width+3, "sd"); for (int j = 0; j<num_models; j++) { printf("% *e ", width, models[j].sd); } printf("\n");
	printf("%*s ", width+3, "sd/avg"); for (int j = 0; j<num_models; j++) { printf("% *e ", width, models[j].sd/models[j].avg); } printf("\n");

	model_t minModel = models[0];
	for (int i = 1; i<num_models; i++) { if ((minModel.sd/minModel.avg)>(models[i].sd/models[i].avg)) minModel = models[i]; }

	printf("\n%*s has the lowest standard deviation per average.\n\n", width+3, minModel.name);

}

// Set algorithm and inform user
static void ui_set_print_algorithm(algorithm_t *a, algorithm_t a_new)
{
	*a = a_new;	
	printf("info> "); ui_print_algorithm_full(a_new); 
}

// Set algorithm and inform user
static void ui_set_print_case(case_t *c, case_t c_new)
{
	*c = c_new;	
	printf("info> "); ui_print_case_full(c_new); 
}


//
// Public
//
void ui_run()
{
	bool running, show_menu;

	result_t result[RESULT_ROWS];
	
	case_t c = average_t;
	algorithm_t a = bubble_sort_t;

	ui_set_print_case(&c,c);
	ui_set_print_algorithm(&a,a);

	show_menu = true;
	running = true;
	while (running) {
		if (show_menu) {
			show_menu = false;
			ui_menu(a, c);
		}
		switch (ui_get_choice()) {
			// House keeping
			case 'a':
				show_menu = true;
				break;
			case 'b':
				running = false;
				break;
			case 'c':
				benchmark(a, c, result, RESULT_ROWS);
				ui_results(result, RESULT_ROWS);
				break;

			case 'd': ui_set_print_algorithm(&a,bubble_sort_t); break;
			case 'e': ui_set_print_algorithm(&a,insertion_sort_t); break;
			case 'f': ui_set_print_algorithm(&a,quick_sort_t); break;
			case 'g': ui_set_print_algorithm(&a,linear_search_t); break;
			case 'h': ui_set_print_algorithm(&a,binary_search_t); break;

			case 'i': ui_set_print_case(&c,best_t); break;
			case 'j': ui_set_print_case(&c,worst_t); break;
			case 'k': ui_set_print_case(&c,average_t); break;

			default:
				show_menu = false;
				ui_invalid_input();
				break;
		}
	}
	ui_exit();
}


// only to be used for debug, the user should not ever see the lists
void ui_DEBUG_print_list(int *d, int n)
{
	printf("debug> ");
	for (int i = 0; i<n; i++) printf("%d ", d[i]);
	printf("\n");
}
