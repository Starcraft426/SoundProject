#ifndef sound_h
#define sound_h

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* sound structure which represent
 a sound to be stored in a wav file 
 of n_samples samples */
typedef struct sound{
    unsigned int n_sample;
    int16_t* samples;
} sound_t;

/* free the sound and its samples */
void free_sound(sound_t* sound);

/* converts pitch to frequency */
int pitch_to_freq(int pitch);

#endif /* sound_h */
