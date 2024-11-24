#ifndef wav_h
#define wav_h

#include "sound.h"
#include "track.h"
#include <stdio.h>
#include <stdbool.h>

/* saves the sound to the file */
void save_sound(FILE* file, sound_t* sound);

/* writes the header of a wav containing n samples into file */
void write_header(FILE* file, int n);

/* writes the integer n on size bits to the file */
void write_int(FILE* file, int a, int size);

#endif /* wav_h */
