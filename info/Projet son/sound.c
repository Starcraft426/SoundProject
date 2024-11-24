#include "sound.h"

void free_sound(sound_t* sound){
    if (sound == NULL){
        return;
    }
    if (sound->samples != NULL){
        free(sound->samples);
    }
    free(sound);
    sound = NULL;
}

int pitch_to_freq(int pitch){
    return 440 * pow(2, (float)pitch/12);
}
