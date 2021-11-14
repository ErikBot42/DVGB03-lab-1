#ifndef UI_H
#define UI_H

#include <stdbool.h>

#define MENU_WIDTH 45
#define RESULT_ROWS 7
#define SIZE_START 512


#define DEBUG


void ui_DEBUG_print_list(int *d, int n); // FOR DEBUG ONLY

// ui_run starts a terminal-based user interface
void ui_run();

typedef struct {
    double (*fp) (double);
    char * name;
    double avg;
    double sd;
    double k;
} model_t;

// print info
typedef struct {
    int w;  // width of text
    bool l; // latex mode
    bool s; // prefix &&
    bool e; // add "\\\\\\hline"
    bool m; // add $ before and after
    bool n; // add \num
} pi_t;

bool ui_debug();

bool ui_error();

#endif
