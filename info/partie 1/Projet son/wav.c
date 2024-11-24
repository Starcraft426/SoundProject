#include "wav.h"

void write_header(FILE* file, int n){
    fprintf(file, "RIFF"); // sur 4 octes
    write_int(file, 36+2*n, 4);
    fprintf(file, "WAVEfmt "); // sur 8 octes
    write_int(file, 16, 4);
    write_int(file, 1, 2);
    write_int(file, 1, 2);
    write_int(file, 44100, 4);
    write_int(file, 88200, 4);
    write_int(file, 2, 2);
    write_int(file, 16, 2);
    fprintf(file, "data"); // sur 4 octes
    write_int(file, 2*n, 4);
}

void save_sound(FILE* file, sound_t* sound){
    write_header(file, sound->n_sample);
    for (unsigned int i = 0; i<sound->n_sample; i++){
        write_int(file, sound->samples[i], 2);
    }
}

void write_int(FILE* file, int a, int size){
    if (size >0){
        for (int i = 0; i<size; i++){
            fprintf(file, "%c", a%256);
            a = a/256;
        }
    }
}
