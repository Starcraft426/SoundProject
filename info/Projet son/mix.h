#ifndef mix_h
#define mix_h

#include "sound.h"
#include "track.h"

/* structure of a mix which is a list of track of lenght n_tracks
   which will plays all in parralell each with a different volume stored in vols */
typedef struct mix{
    unsigned int n_tracks;
    track_t** tracks;
    float* vols;
} mix_t;

/* free the mix, its track its sounds and its volumes */
void free_mix(mix_t* mix);

/* returns a sound which is a mean of all the tracks played
   in parrallel */
sound_t* reduce_mix(mix_t* mix);

/* adds the track to the mix */
mix_t* add_track(mix_t* mix, track_t* track);

#endif /* mix_h */
