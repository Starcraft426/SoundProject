#include "waveform.h"

#define PI 3.141592654
#define A_max 32767

sound_t* white(float duree, int f_ech){
    srand((unsigned int)time(NULL));
    if (duree < 0){
        duree = 0;
    }
    sound_t* sound = malloc(sizeof(sound_t));
    sound->n_sample = duree*f_ech;
    sound->samples = malloc(sound->n_sample*sizeof(int16_t));
    for (int i = 0; (unsigned int)i<sound->n_sample; i++){
        sound->samples[i] = rand();
    }
    return sound;
}

sound_t* sine(float freq, int amplitude, float duree, int f_ech){
    sound_t* sound = malloc(sizeof(sound_t));
    if (duree < 0){
        duree = 0;
    }
    sound->n_sample = duree*f_ech;
    sound->samples = malloc(sound->n_sample*sizeof(int16_t));
    for(float i = 0; (unsigned int)i < sound->n_sample; i++){
        sound->samples[(int)i] = amplitude*sinf( (2 * PI * i * freq) / (f_ech) );
    }
    return sound;
}

sound_t* square(float freq, int amplitude, float duree, int f_ech){
    sound_t* sound = sine(freq, amplitude, duree, f_ech);
    if (duree < 0){
        duree = 0;
    }
    for (int i = 0; (unsigned int)i< sound->n_sample; i++){
        if (sound->samples[i] > 0){
            sound->samples[i] = amplitude;
        }else{
            sound->samples[i] = -amplitude;
        }
    }
    return sound;
}

sound_t* triangle(float freq, int amplitude, float duree, int f_ech){
    sound_t* sound = sawtooth(freq*2, amplitude, duree, f_ech);
    if (duree < 0){
        duree = 0;
    }
    sound_t* sound2 = sine(freq, amplitude, duree, f_ech);
    for (int i = 0; (unsigned int)i< sound->n_sample; i++){
        if ((sound2->samples[i] * sound->samples[i] < 0)){
            sound->samples[i] = sound->samples[i]*-1;
        }
    }
    free_sound(sound2);
    return sound;
}

sound_t* sawtooth(float freq, int amplitude, float duree, int f_ech){
    sound_t* sound = malloc(sizeof(sound_t));
    if (duree < 0){
        duree = 0;
    }
    sound->n_sample = duree*f_ech;
    sound->samples = malloc(sound->n_sample*sizeof(int16_t));

    for (unsigned int i = 0; i< sound->n_sample; i++){
        sound->samples[i] = (2*A_max*freq*i)/(f_ech);
        sound->samples[i] = sound->samples[i]*amplitude/A_max;
    }
    return sound;
}
