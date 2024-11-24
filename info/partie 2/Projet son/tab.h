#ifndef tab_h
#define tab_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* a tab which is used to store a 2D array of strings
   to store argments from user input */
typedef struct tab{
    int* tab_len;
    int len;
    char*** args;
}tab_t;

/* frees the tab entirely */
void free_tab(tab_t* tab);

/* returns the tab with the arg added on the line i*/
tab_t* add_arg(tab_t* tab, int i, char* arg);

/* returns the tab with one more line */
tab_t* add_line(tab_t* tab);

/* returns the tab totally empty (1 row, 0 arguments) */
tab_t* reset_tab(tab_t* tab);
#endif
