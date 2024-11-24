#include "melody.h"

mix_t* load_mix(char* filename){
    FILE* file = fopen(filename, "r");
    mix_t* mix = malloc(sizeof(mix_t));
    int n;
    fscanf(file, "%d", &n);
    mix->n_tracks = n;
    
    mix->tracks = malloc(mix->n_tracks*sizeof(track_t*));
    mix->vols = malloc(mix->n_tracks*sizeof(float));
    for (unsigned int i = 0; i<mix->n_tracks; i++){
        fscanf(file, "%f", &mix->vols[i]);
    }
    for (unsigned int i = 0; i<mix->n_tracks; i++){
        mix->tracks[i] = read_track(file);
    }
    return mix;
}
