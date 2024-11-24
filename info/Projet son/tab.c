#include "tab.h"

void free_tab(tab_t* tab){
    for (int i = 0; i<tab->len; i++){
        for (int j = 0; j<tab->tab_len[i]; j++){
            free(tab->args[i][j]);
        }
        free(tab->args[i]);
    }
    free(tab->args);
    free(tab->tab_len);
    free(tab);
    tab = NULL;
}

tab_t* reset_tab(tab_t* tab){
    if (tab != NULL){
        free_tab(tab);
    }
    tab = malloc(sizeof(tab_t));
    tab->tab_len = NULL;
    tab->args = NULL;
    tab->len = 0;
    tab = add_line(tab);
    return tab;
}

tab_t* add_arg(tab_t* tab, int i, char* arg){
    tab->tab_len[i] ++;
    tab->args[i] = realloc(tab->args[i], tab->tab_len[i]*sizeof(char*));
    tab->args[i][tab->tab_len[i]-1] = malloc(50*sizeof(char));
    strcpy(tab->args[i][tab->tab_len[i]-1], arg);
    return tab;
}

tab_t* add_line(tab_t* tab){
    if (tab->tab_len == NULL){
        tab->len = 1;
        tab->tab_len = malloc(tab->len*sizeof(int));
        tab->args = malloc(tab->len*sizeof(char**));
    }else{
        tab->len++;
        tab->tab_len = realloc(tab->tab_len, tab->len*sizeof(int));
        tab->args = realloc(tab->args, tab->len*sizeof(char**));
    }
    tab->args[tab->len-1] = malloc(0*sizeof(char*));
    tab->tab_len[tab->len-1] = 0;
    return tab;
}
