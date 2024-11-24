#include "mix.h"

void free_mix(mix_t* mix){
    if (mix == NULL){
        return;
    }
    for (int i = 0; (unsigned int)i<mix->n_tracks; i++){
        free_tracks(mix->tracks[i]);
    }
    free(mix->tracks);
    free(mix->vols);
    free(mix);
    mix = NULL;
}

mix_t* add_track(mix_t* mix, track_t* track){
    if (mix == NULL){
        mix = malloc(sizeof(mix_t));
        mix->n_tracks = 0;
        mix->tracks = malloc(sizeof(track_t*));
        mix->vols = malloc(sizeof(int));
        mix->n_tracks++;
    }else{
        mix->n_tracks++;
        mix->vols = realloc(mix->vols, mix->n_tracks*sizeof(int));
        mix->tracks = realloc(mix->tracks, mix->n_tracks*sizeof(track_t*));
    }
    mix->vols[mix->n_tracks-1] = 1;
    mix->tracks[mix->n_tracks-1] = track;
    return mix;
}

sound_t* reduce_mix(mix_t* mix){
    track_t* track = malloc(sizeof(track_t));
    track->n_sounds = mix->n_tracks;
    track->sounds = malloc(mix->n_tracks*sizeof(sound_t *));
    sound_t* sound = malloc(sizeof(sound_t));
    sound->n_sample = 0;
    sound->duree = 0;
    for (int i = 0; (unsigned int)i<mix->n_tracks; i++){
        track->sounds[i] = reduce_track(mix->tracks[i]);
        if (sound->duree < mix->tracks[i]->duree){
            sound->duree = mix->tracks[i]->duree;
        }
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
