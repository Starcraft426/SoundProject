#ifndef track_h
#define track_h

#include <string.h>
#include "sound.h"
#include "waveform.h"
#include <stdbool.h>

/* structure of a track which is basically a list of sounds
   of lenght n_sounds */
typedef struct track{
    unsigned int n_sounds;
    sound_t** sounds;
    float duree;
} track_t;

/* functions that frees the track, and its sounds */
void free_tracks(track_t* track);

/* returns a sound created by putting all of the songs
   in track one after the other */
sound_t* reduce_track(track_t* track);

/* returns a track read form file */
track_t* read_track(FILE* file, bool display_error);

/* adds a sound into the track */
track_t* add_sound(track_t* track, sound_t* sound);

#endif /* track_h */
