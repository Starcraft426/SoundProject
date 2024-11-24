#include "sound.h"

void free_sound(sound_t* sound){
    if (sound != NULL){
        free(sound->samples);
        free(sound);
    }
}

int pitch_to_freq(int pitch){
    return 440 * pow(2, (float)pitch/12);
}
