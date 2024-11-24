#ifndef testmode_h
#define testmode_h

#include "melody.h"
#include "mix.h"
#include "sound.h"
#include "track.h"
#include "wav.h"
#include "waveform.h"
#include "tab.h"

/* converts str to an int and throw a custom error
   if needed and if diplay_error is set to true */
int str_to_int(char* str, bool display_error, char* var_name, int var_n);

/* converts str to a float and throw a custom error
   if needed and if display error is set to true */
float str_to_float(char* str, bool display_error, char* var_name, int var_n);

/* shows a warning to the user that the file filename
   already exists and ask for a response
   if nowarn is set to true, nothing happens */
bool warn_file(char* filename, bool nowarn);

/* a whole user friendly interface which acts like
   a command line where you can type functions, variables
   to test all the useful functions of this project
   nowarn disables the warning when a file already exists */
void test_mode(bool nowarn, bool debugmode);

/* help notice on how the test mode works
   and what is available. if debugmode is true, skips
   the waiting of user input */
void help(bool debugmode);

/* function that awaits for an entry by clicking on enter
   if debugmode, only print text */
void next_page(bool debugmode);

#endif
