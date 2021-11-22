#ifndef UI_H
#define UI_H

#include <stdbool.h>


#define MENU_WIDTH 45
#if 1
#define RESULT_ROWS 7
#define SIZE_START 512
//#define SIZE_START 1024
//#define SIZE_START 2048
//#define SIZE_START 4096
//#define SIZE_START 8192

//#define SIZE_START 16384

//#define SIZE_START 524288
//#define SIZE_START 4194304
#else
#define RESULT_ROWS 1
#define SIZE_START 16
#endif


//#define DEBUG


void ui_DEBUG_print_list(int *d, int n); // FOR DEBUG ONLY

// ui_run starts a terminal-based user interface
void ui_run();


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
