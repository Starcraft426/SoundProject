#include "track.h"

void free_tracks(track_t* track){
    if (track == NULL){
        return;
    }
    for (int i = 0; (unsigned int)i< track->n_sounds; i++){
        if (track->sounds[i] != NULL){
            free_sound(track->sounds[i]);
        }
    }
    track->n_sounds = 0;
    free(track->sounds);
    free(track);
    track = NULL;
}

sound_t* reduce_track(track_t* track){
    sound_t* sound = malloc(sizeof(sound_t));
    sound->samples = malloc(0);
    sound->n_sample = 0;
    sound->duree = 0;
    int index;
    for (int i = 0; (unsigned int)i < track->n_sounds; i++){
        sound->duree += track->sounds[i]->duree;
        index = sound->n_sample;
        sound->n_sample += track->sounds[i]->n_sample;
        sound->samples = realloc(sound->samples, sound->n_sample*sizeof(int16_t));
        for (int j = 0; (unsigned int)j< track->sounds[i]->n_sample; j++){
            sound->samples[index + j] = track->sounds[i]->samples[j];
        }
    }
    return sound;
}

track_t* add_sound(track_t* track, sound_t* sound){
    if (track == NULL){
        track = malloc(sizeof(track_t));
        track->n_sounds = 0;
        track->sounds = malloc(sizeof(sound_t*));
        track->duree = 0;
    }
    track->n_sounds ++;
    track->sounds = realloc(track->sounds, track->n_sounds*sizeof(sound_t*));
    
    if (sound->duree > track->duree){
        track->duree = sound->duree;
    }
    track->sounds[track->n_sounds-1] = sound;
    return track;
}

track_t* read_track(FILE* file, bool display_error){
    int len = 30;
    float args[3];
    char* line = malloc(len*sizeof(char));
    track_t* track = malloc(sizeof(track_t));
    track->n_sounds = 0;
    track->duree = 0;
    fscanf(file, "%d", &track->n_sounds);
    track->sounds = malloc(track->n_sounds*sizeof(sound_t*));
    fscanf(file, "%s", line);
    if (strcmp(line, "sine") && strcmp(line, "square") && strcmp(line, "triangle") && strcmp(line, "sawtooth")){
        if (display_error){
            printf("Error: invalid sound type\n");
        }
        free(line);
        free_tracks(track);
        return NULL;
    }
    for (int i = 0; (unsigned int)i<track->n_sounds; i++){
        for(int j = 0; j<3; j++){
            fscanf(file, "%f", &args[j]);
        }
        track->duree += args[1];
        if (!strcmp(line, "sine")){
            track->sounds[i] = sine(pitch_to_freq((int)args[0]), args[2]*32000, args[1]);
        }else if (!strcmp(line, "square")){
            track->sounds[i] = square(pitch_to_freq((int)args[0]), args[2]*32000, args[1]);
        }else if (!strcmp(line, "triangle")){
            track->sounds[i] = triangle(pitch_to_freq((int)args[0]), args[2]*32000, args[1]);
        }else if (!strcmp(line, "sawtooth")){
            track->sounds[i] = sawtooth(pitch_to_freq((int)args[0]), args[2]*32000, args[1]);
        }
    }
    free(line);
    return track;
}
