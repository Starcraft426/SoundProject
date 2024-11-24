#include "track.h"

void free_tracks(track_t* track){
    for (unsigned int i = 0; i< track->n_sounds; i++){
        free_sound(track->sounds[i]);
    }
    free(track->sounds);
    free(track);
}

sound_t* reduce_track(track_t* track){
    sound_t* sound = malloc(sizeof(sound_t));
    sound->samples = malloc(0);
    sound->n_sample = 0;
    int index;
    for (unsigned int i = 0; i < track->n_sounds; i++){
        index = sound->n_sample;
        assert(track->sounds != NULL);
        sound->n_sample += track->sounds[i]->n_sample;
        sound->samples = realloc(sound->samples, sound->n_sample*sizeof(int16_t));
        for (unsigned int j = 0; j< track->sounds[i]->n_sample; j++){
            sound->samples[index + j] = track->sounds[i]->samples[j];
        }
    }
    return sound;
}

track_t* read_track(FILE* file){
    int len = 30;
    float args[3];
    char* line = malloc(len*sizeof(char));
    track_t* track = malloc(sizeof(track_t));
    track->n_sounds = 0;
    fscanf(file, "%d", &track->n_sounds);
    track->sounds = malloc(track->n_sounds*sizeof(sound_t*));
    fscanf(file, "%s", line);
    assert(!strcmp(line, "sine") || !strcmp(line, "square") || !strcmp(line, "triangle") || !strcmp(line, "sawtooth"));
    for (unsigned int i = 0; i<track->n_sounds; i++){
        for(int j = 0; j<3; j++){
            fscanf(file, "%f", &args[j]);
        }
        if (!strcmp(line, "sine")){
            track->sounds[i] = sine(pitch_to_freq((int)args[0]), args[2]*32000, args[1], 44100);
        }else if (!strcmp(line, "square")){
            track->sounds[i] = square(pitch_to_freq((int)args[0]), args[2]*32000, args[1], 44100);
        }else if (!strcmp(line, "triangle")){
            track->sounds[i] = triangle(pitch_to_freq((int)args[0]), args[2]*32000, args[1], 44100);
        }else if (!strcmp(line, "sawtooth")){
            track->sounds[i] = sawtooth(pitch_to_freq((int)args[0]), args[2]*32000, args[1], 44100);
        }
    }
    free(line);
    return track;
}
