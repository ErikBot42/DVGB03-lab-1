#ifndef UI_H
#define UI_H

#define MENU_WIDTH 40
#define RESULT_ROWS 7

void ui_DEBUG_print_list(int *d, int n); // FOR DEBUG ONLY

// ui_run starts a terminal-based user interface
void ui_run();

typedef struct {
	double (*fp) (double);
	char * name;
	double avg;
	double sd;
} model_t;


#endif
