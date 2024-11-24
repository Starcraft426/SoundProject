#include "mix.h"

void free_mix(mix_t* mix){
    for (unsigned int i = 0; i<mix->n_tracks; i++){
        free_tracks(mix->tracks[i]);
    }
    free(mix->tracks);
    free(mix->vols);
    free(mix);
}

sound_t* reduce_mix(mix_t* mix){
    track_t* track = malloc(sizeof(track_t));
    track->n_sounds = mix->n_tracks;
    track->sounds = malloc(mix->n_tracks*sizeof(sound_t *));
    sound_t* sound = malloc(sizeof(sound_t));
    sound->n_sample = 0;
    for (int i = 0; (unsigned int)i<mix->n_tracks; i++){
        track->sounds[i] = reduce_track(mix->tracks[i]);
        if (sound->n_sample < track->sounds[i]->n_sample){
            sound->n_sample = track->sounds[i]->n_sample;
        }
    }
    if (track->n_sounds == 1){
        free(sound);
        sound = track->sounds[0];
        track->sounds[0] = NULL;
        free_tracks(track);
        return sound;
    }
    sound->samples = malloc(sound->n_sample*sizeof(int16_t));
    for (int i = 0; (unsigned int)i<sound->n_sample; i++){
        sound->samples[i] = 0;
    }
    float total = 0;
    for (int j = 0; (unsigned int)j<track->n_sounds; j++){
        total += mix->vols[j];
    }
    for (int i = 0; (unsigned int)i<sound->n_sample; i++){
        for (int j = 0; (unsigned int)j<track->n_sounds; j++){
            if (track->sounds[j]->n_sample > (unsigned int)i){
                sound->samples[i] += (int16_t)((track->sounds[j]->samples[i]*mix->vols[j])/(mix->n_tracks*total));
            }
        }
    }
    for (int i = 0; (unsigned int)i<sound->n_sample; i++){
        sound->samples[i] = (sound->samples[i]) * mix->n_tracks;
    }
    free_tracks(track);
    return sound;
}
