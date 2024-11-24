#include "melody.h"

mix_t* load_mix(char* filename, bool display_error){
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        if (display_error){
            printf("Can't find %s\n", filename);
        }
        return NULL;
    }
    mix_t* mix = malloc(sizeof(mix_t));
    int n = 0;
    fscanf(file, "%d", &n);
    mix->n_tracks = n;
    mix->tracks = malloc(mix->n_tracks*sizeof(track_t*));
    mix->vols = malloc(mix->n_tracks*sizeof(float));
    printf("Loading %d tracks\n", n);
    if (n <= 0){
        if (display_error){
            printf("File %s is not a valid mix file\n", filename);
        }
        free_mix(mix);
        fclose(file);
        return NULL;
    }
    
    for (int i = 0; (unsigned int)i<mix->n_tracks; i++){
        fscanf(file, "%f", &mix->vols[i]);
    }
    for (int i = 0; (unsigned int)i<mix->n_tracks; i++){
        printf("Loading track %d/%d\n", i+1, mix->n_tracks);
        mix->tracks[i] = read_track(file, display_error);
        if (mix->tracks[i] == NULL){
            if (display_error){
                printf("File %s is not a valid mix file\n", filename);
            }
            free_mix(mix);
            fclose(file);
            return NULL;
        }
    }
    fclose(file);
    return mix;
}
