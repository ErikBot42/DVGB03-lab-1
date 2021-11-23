#include "ui.h"
#include "io.h"
#include "analyze.h"

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//
// Private
//

// Function that behaves like a global variable
bool p_enableExtraPrints(bool *enableExtraPrints)
{
    static bool *e = NULL;
    e = enableExtraPrints==NULL ? e : enableExtraPrints;
    return (e == NULL) ? false : *e;
}

// should extra info be printed to the screen
// currently only changed for making a LaTeX only print
bool enableExtraPrints() { return p_enableExtraPrints(NULL);}

static bool ui_info()
{
    if (enableExtraPrints()) printf("info> ");
    return enableExtraPrints(); // always print info
}

static void ui_invalid_input()
{
    if (ui_info()) printf("bad input, press \'a\' for menu.\n");
}

static void ui_exit()
{
    if (ui_info()) printf("exiting...\n");
}

//static char ui_get_choice()
//{ char buf[3];
//    printf("input> ");
//    return read_line(buf, 3) ? buf[0] : 0;
//}

static char * ui_get_choices(int n)
{
    n+=2; 
    char * buf = (char *) malloc(sizeof(char)*n);
    printf("input> ");
    if(!read_line(buf, n)) buf[0] = '\0';
    return buf;
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

static int calc_num_el(int rows, int start_size)
{
    return start_size << (rows-1);
}

static void ui_print_num_rows(int rows, int start_size)
{
    printf("Current number of rows: %d (%d elements)\n", rows, calc_num_el(rows, start_size));
}

static void ui_menu(algorithm_t a, case_t c, int rows, int start_size)
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
        "Average case\n",
        "Increase rows",
        "Decrease rows",
        "LaTeX table output only",
    };

    ui_line('=', MENU_WIDTH);
    ui_print_algorithm_full(a); 
    ui_print_case_full(c); 
    ui_print_num_rows(rows, start_size);
    ui_line('-', MENU_WIDTH);
    ui_menu_options(options, sizeof(options) / sizeof(char *));
    ui_line('-', MENU_WIDTH);
}

static double sizeToGB(double size)
{
    return (size*sizeof(int))/1024.0/1024.0/1024.0;
}

static bool mm(pi_t *p) {if (p->m) printf("$"); return p->m;}

static void table_begin() {printf("\\begin{table}\n\\tabcolsep=0.09cm\n");}
static void table_end() {printf("\\end{table}\n");}
static void tabular_begin() {printf("\\begin{tabular}{c|c|*{30}{c}}\\hline\n");}
static void tabular_end() {printf("\\end{tabular}\n");}

static void ln()
{
    printf("\n");
}

// produces caption for table
static void ui_results_print_min_model(model_t *models, ac_t ac, pi_t p, int n)
{
    p.m = p.l;
    if (!p.l) ln();
    if (p.l) printf("\\caption{");
    model_t minModel = models[0];
    for (int i = 1; i<n; i++) { if ((minModel.sd/minModel.avg)>(models[i].sd/models[i].avg)) minModel = models[i]; }
    printf("In ");
    ui_print_case(ac.c);
    printf(" case ");
    ui_print_algorithm(ac.a);
    printf(", ");
    mm(&p); printf("%s", minModel.name); mm(&p);
    printf(" has the lowest sd/avg, where sd is the standard deviation and avg is the average value. ");
    printf("sd/avg is a measure of how varying the values are. ");
    printf("Values are calculated with ");
    mm(&p); printf("time/f(size)"); mm(&p);
    printf(", where "); 
    mm(&p); printf("f"); mm(&p);
    printf(" is the highest order term of the complexity. ");
    if (p.l) printf("This table and caption was automatically generated. ");
    if (p.l) printf("}");
    ln();
}

// either prints ui header or LaTeX code for table
static void ui_results_print_header(pi_t p, int cols)
{
    if (!p.l) {
        ln();
        //ui_line('=', MENU_WIDTH);
        ui_line('=', (p.w+4)*cols);
        printf("Results:\n\n");
    }
}


// c lacks function overloading
static void pln0(pi_t *p, bool lx_tbl_newline)
{ 
    if (p->e) 
    {
        if (lx_tbl_newline) printf("\\\\");
        printf("\\hline");
    }
    printf("\n");
    p->s = false;
}

static void pln(pi_t *p)
{
    pln0(p, true);
}

static void pln1(pi_t *p)
{
    pln0(p, false);
}

// print functions to print aligned and for latex print
static void p_pre(pi_t *p) { if (p->s) printf("&"); if(mm(p) && p->n) printf("\\num{"); } // print prefix
static void p_suf(pi_t *p) { if (p->m && p->n) printf("}"); mm(p); p->m = false; } // print suffix
static void p_int(int n, pi_t *p) { p->m = p->l; p_pre(p); printf("%*d ", p->w+3, n); p_suf(p); }
static void p_fix(double d, pi_t *p) { p->m = p->l; p_pre(p); printf("% *f ", p->w+3, d); p_suf(p); }
static void p_dbl(double d, pi_t *p) { p->m = p->l; p_pre(p); printf("% *.*E ", p->w, 6+p->w-10, d);  p_suf(p); }
static void p_str(char *s, pi_t *p) { p_pre(p); printf("%*s ", p->w+3, s);  p_suf(p); }
//static void p_tbl_pre(pi_t *p) { if (p->l) printf("LaTeX table:\n"); else printf("Results:\n");}

// To get function pointers for diffrent models
static double O_1(double n) {return 1;}
static double O_n(double n) {return n;}
static double O_n2(double n) {return n*n;}
//static double O_n3(double n) {return n*n*n;}
static double O_n_log_n(double n) {return n*log(n);}

static model_t* getModels(int *n)
{

    model_t models[] = {
        {O_1,       "O(1)"},
        {log,       "O(log(n))"},
        //{sqrt,      "O(sqrt(n))"},
        {O_n,       "O(n)"},
        {O_n_log_n, "O(nlog(n))"},
        {O_n2,      "O(n^2)"},
        //{O_n3,      "O(n^3)"},
    }; 
    *n = sizeof(models)/sizeof(models[0]);
    return memcpy((model_t*) malloc(sizeof(models)), models, sizeof(models));
}


// print results from benchmark
// Long, but splitting it might not reduce readability.
static void ui_results(result_t *results, ac_t ac, int n, bool LaTeX_mode)
{

    pi_t p = {LaTeX_mode ? 6 : 10, LaTeX_mode};
    if (p.l) table_begin();
    if (p.l) tabular_begin();
    p.e = p.l;

    int num_models;
    model_t *models = getModels(&num_models);
    int cols = num_models + 2;

    ui_results_print_header(p, cols);
    p_str("size",&p); p.s=p.l; p_str("size (GB)",&p); for (int j = 0; j<num_models; j++) { p.m=p.l; p_str(models[j].name,&p); } pln(&p); 
    p.n = true;
    for (int i = 0; i<n; i++)
    {
        p_int(results[i].size,&p);p.s=p.l;
        p_fix(sizeToGB(results[i].size),&p);
        for (int j = 0; j<num_models; j++) 
        { 
            double res = c_res(models[j],results[i]);
            p_dbl(res,&p); 
        }   
        pln(&p);
    }
    calcModelData(models,results,num_models,n);
    pln1(&p); p_str("avg",&p); p.s=p.l; p_str("",&p); 
    for (int j = 0; j<num_models; j++) { p_dbl(models[j].avg,&p); }
    pln(&p); p_str("sd",&p); p.s=p.l; p_str("",&p); 
    for (int j = 0; j<num_models; j++) { p_dbl(models[j].sd,&p); }
    //pln(&p); p_str("k",&p); p.s=p.l; p_str("",&p); 
    //for (int j = 0; j<num_models; j++) { p_dbl(models[j].k,&p); }
    pln(&p); p_str("sd/avg",&p); p.s=p.l; p_str("",&p); 
    for (int j = 0; j<num_models; j++) { p_dbl(models[j].sd/models[j].avg,&p); }
    //pln(&p); p_str("k/avg",&p); p.s=p.l; p_str("",&p); 
    //for (int j = 0; j<num_models; j++) { p_dbl(models[j].k/models[j].avg,&p); }
    pln(&p);
    if (p.l) tabular_end();
    ui_results_print_min_model(models, ac, p, num_models);
    if (p.l) table_end();
    free(models);
}


// Set algorithm and inform user
static void ui_set_print_algorithm(algorithm_t *a, algorithm_t a_new)
{
    *a = a_new;    
    if (ui_info()) ui_print_algorithm_full(a_new); 
}

// Set algorithm and inform user
static void ui_set_print_case(case_t *c, case_t c_new)
{
    *c = c_new;    
    if (ui_info()) ui_print_case_full(c_new); 
}

//
// Public
//

void ui_run()
{
    bool extraPrints = true;
    p_enableExtraPrints(&extraPrints);
    bool running, show_menu;
    int result_rows = RESULT_ROWS;

    ac_t ac = {bubble_sort_t, average_t};

    show_menu = true;
    running = true;

    int max_num_choices = 1000;

    int start_size = SIZE_START;

    while (running) {
        if (show_menu) { show_menu = false; ui_menu(ac.a, ac.c, result_rows, start_size); }
        char * choices = ui_get_choices(max_num_choices); // allow user to input multiple things at once
        bool keepReadingChoices = true;
        bool LaTeX_mode = false;
        bool resetExtraPrints = extraPrints;
        for (int i = 0; i<max_num_choices && keepReadingChoices; i++)
        {
            char choice = choices[i];
            switch (choice) {
                // House keeping
                case 'a': show_menu = true; break;
                case 'b': running = false; break;
                case 'c':
                          {
                              result_t result[result_rows];
                              benchmark(ac, result, result_rows, SIZE_START);
                              ui_results(result, ac, result_rows, true);
                              if (!LaTeX_mode) ui_results(result, ac, result_rows, false);
                          } break;
                case 'd': ui_set_print_algorithm(&ac.a,bubble_sort_t); break;
                case 'e': ui_set_print_algorithm(&ac.a,insertion_sort_t); break;
                case 'f': ui_set_print_algorithm(&ac.a,quick_sort_t); break;
                case 'g': ui_set_print_algorithm(&ac.a,linear_search_t); break;
                case 'h': ui_set_print_algorithm(&ac.a,binary_search_t); break;
                case 'i': ui_set_print_case(&ac.c,best_t); break;
                case 'j': ui_set_print_case(&ac.c,worst_t); break;
                case 'k': ui_set_print_case(&ac.c,average_t); break;
                case 'l': result_rows++;
                          result_rows = calc_num_el(result_rows, start_size)>=0 ? result_rows : result_rows-1; // stop before element overflow
                          if (ui_info()) { ui_print_num_rows(result_rows, start_size); } break;
                case 'm': result_rows = result_rows > 1 ? result_rows-1 : 1; if (ui_info()) { ui_print_num_rows(result_rows, start_size); } break;
                case 'n': LaTeX_mode = true; extraPrints=false; break;
                case '\0': keepReadingChoices = false; if (i == 0) { show_menu = false; ui_invalid_input(); } break;
                default: keepReadingChoices = false; show_menu = false; ui_invalid_input(); break;
            }
        }
        extraPrints = resetExtraPrints;
        free(choices);
    }
    ui_exit();
}

// only to be used for debug, the end user should never see the lists
void ui_DEBUG_print_list(int *d, int n)
{
    if (ui_debug(""))
    {
        for (int i = 0; i<n; i++) printf("%d ", d[i]);
        printf("\n");
    }
}

bool ui_error(char * t)
{
    if (enableExtraPrints()) printf("error> %s", t);
    return enableExtraPrints(); // always print errors
}

bool ui_debug(char * t)
{
#ifdef DEBUG
    if (enableExtraPrints()) printf("debug> %s", t);
    return enableExtraPrints();
#else
    return false;
#endif
}

