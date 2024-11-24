#ifndef waveform_h
#define waveform_h

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "sound.h"

// note: removed the sampling frequency because it was pointless

/* generates a white noise i.e. a sound with random amplitude */
sound_t* white(float duree);

/* generates a sine with freq as frequency, amplitude as amplitude
   and duree as lenght*/
sound_t* sine(float freq, int amplitude, float duree);

/* generates a square shaped sound with freq as frequency,
   amplitude as amplitude duree as lenght */
sound_t* square(float freq, int amplitude, float duree);

/* generates a triangle shaped sound with freq as frequency,
   amplitude as amplitude duree as lenght and f_ech as sampling frequency */
sound_t* triangle(float freq, int amplitude, float duree);

/* generates a sawtooth shaped sound with freq as frequency,
   amplitude as amplitude duree as lenght and f_ech as sampling frequency */
sound_t* sawtooth(float freq, int amplitude, float duree);

#endif
