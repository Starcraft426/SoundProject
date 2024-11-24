#ifndef wav_h
#define wav_h

#include "sound.h"
#include "track.h"
#include <stdio.h>
#include <stdbool.h>

/* shows a message to the user if expression is false
   like an assert without ending the program */
bool assert_file(bool expression);

/* writes the integer n on size bits to the file
   if int is written returns True, False if not */
bool write_int(FILE* file, int a, int size);

/* saves the track to the file */
void save_sound(FILE* file, track_t* track);

/* checks if the string str is on the next len bytes in file */
bool check_str(FILE* file, char* str, int len);

/* checks if the integer n is on the next len bytes in file */
bool check_int(FILE* file, int n, int len);

/* reads the wav file and converts it back to a track */
track_t* read_wav(FILE* file);

/* writes the header  */
void write_header(FILE* file, int n, int n_channels);
int convert(char c);

#endif /* wav_h */
