#include "Projet son/melody.h"
#include "Projet son/mix.h"
#include "Projet son/sound.h"
#include "Projet son/track.h"
#include "Projet son/wav.h"
#include "Projet son/waveform.h"
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 3){
        // print the usage of the program
        printf("must be used like %s file_in file_out\n\n", argv[0]);
        exit(1);
    }
    // check presence of the file
    char* filename = argv[1];
    FILE* file = fopen(filename, "r");
    if (file == NULL){
        printf("Can't find %s\n", filename);
        exit(1);
    }
    fclose(file);
    
    clock_t start = clock();
    filename = argv[1];
    mix_t* mix4 = load_mix(filename);
    sound_t* sound = reduce_mix(mix4);
    
    // writes in the file
    filename = argv[2];
    file = fopen(filename, "w");
    save_sound(file, sound);
    free_mix(mix4);
    fclose(file);
    
    // gives the user a recap of the sound
    int size = sound->n_sample*2;
    int duree = (int)sound->n_sample/44100;
    int n_pow = 0;
    while (size > 1000) {
        size = (int)roundf((float)(size/1000));
        n_pow++;
    }
    
    free_sound(sound);
    
    clock_t stop = clock();
    unsigned long t = (stop - start)/CLOCKS_PER_SEC;
    int sub = roundf((t-roundf(t))*10);
    printf("Fichier %s généré (temps écoulé: %d.%ds)\n", argv[2], (int)t, sub);
    printf("Taille du fichier: %d", size);
    switch (n_pow) {
        case 0:
            printf("o\n");
            break;
        case 1:
            printf("Ko\n");
            break;
        case 2:
            printf("Mo\n");
            break;
        // these three should never happen but we never know
        case 3:
            printf("Go\n");
            break;
        case 4:
            printf("To\n");
            break;
        default:
            printf("*10^(%d) o", n_pow);
    }
    printf("Durée du fichier: ");
    if (duree/3600){
        printf("%dh", duree/3600);
    }
    if (duree/60){
        printf("%dm", (duree/60)%60);
    }
    printf("%ds\n", duree%3600);
}
