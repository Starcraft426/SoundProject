#ifndef melody_h
#define melody_h

#include "mix.h"
#include <stdbool.h>

/* returns a mix read from file */
mix_t* load_mix(char* filename, bool display_error);

#endif /* melody_h */