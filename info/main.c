#include "Projet son/testmode.h"
#include <time.h>

int main(int argc, char *argv[]) {
    int index = -1;
    char** args = malloc(argc*sizeof(char*));
    int params = 0;
    
    // set up the params
    bool testmode = false;
    bool nowarn = false;
    bool debugmode = false;
    
    for (int i = 0; i<argc; i++){
        if (argv[i][0] == '-' && argv[i][1] == '-'){
            // mode to test all the commands by yourself
            if (!strcmp(argv[i], "--testmode")){
                testmode = true;
                params ++;
            // to remove warnings when you try to replace a file
            }else if(!strcmp(argv[i], "--nowarn")){
                printf("Running without warns...\n");
                nowarn = true;
                params++;
            // to do the sets of tests
            }else if(!strcmp(argv[i], "--debug")){
                debugmode = true;
                params++;
            }
        }else{
            index ++;
            args[index] = argv[i];
        }
    }
    argc -= params;
    args = realloc(args, argc*sizeof(char*));
    
    // sets up the mode
    if (debugmode){
        printf("Warns are disabled automatically on testmode\n");
        nowarn = true;
        printf("Running the sets of tests for");
        if (testmode){
            printf(" test mode...\n\n");
            free(args);
            fclose(stdin);
            stdin = fopen("testmode tests.txt", "r");
            test_mode(nowarn, debugmode);
            fclose(stdin);
            printf("\nAll tests passed with success!\n");
            return 0;
        }else{
            printf(" normal mode...\n\n");
            free(args);
            sound_t* sound = NULL;
            track_t* track = NULL;
            mix_t* mix = NULL;
            
            // testting negative values on sound functions
            sound = sine(-1, -1, -1);
            free_sound(sound);
            sound = sawtooth(-1, -1, -1);
            free_sound(sound);
            sound = square(-1, -1, -1);
            free_sound(sound);
            sound = triangle(-1, -1, -1);
            free_sound(sound);
            sound = white(-1);
            track = add_sound(track, sound);
            mix = add_track(mix, track);
            free_mix(mix);
            track = NULL;
            
            // testing uncompatible files for rad_wav and load_mix
            FILE* file = fopen("testmode tests.txt", "r");
            read_wav(file);
            fclose(file);
            load_mix("testmode tests.txt", true);
            
            // testing on a real file
            mix = load_mix("sound1.txt", true);
            sound = reduce_mix(mix);
            track = add_sound(track, sound);
            file = fopen("sound1.wav", "w");
            save_sound(file, track);
            free_tracks(track);
            free_mix(mix);
            fclose(file);
            
            // testing null on free
            sound = NULL;
            track = NULL;
            mix = NULL;
            free_sound(sound);
            free_tracks(track);
            free_mix(mix);
            
            printf("\nAll tests passed with success!\n");
            return 0;
        }
    }
    if (testmode){
        printf("Running in test mode...\n\n");
        free(args);
        test_mode(nowarn, debugmode);
        return 0;
    }
    else if (argc < 3){
        // print the usage of the program
        printf("must be used like %s list_of_file_in file_out\n\n", argv[0]);
        printf("  * use --testmode to test the program\n");
        printf("  * use --nowarn to remove file warnings\n");
        printf("  * use --debug to run the sets of tests\n    with either testmode or normal mode\n");
        free(args);
        exit(1);
    }
    printf("Running the program normally...\n");
    // check presence of the files then warn if a file already exist
    char* filename;
    FILE* file;
    for (int i = 1; i<argc-1; i++){
        filename = args[i];
        file = fopen(filename, "r");
        if (file == NULL){
            printf("Can't find %s\n", filename);
            free(args);
            exit(1);
        }
        fclose(file);
    }
    
    bool exit_ = warn_file(args[argc-1], nowarn);
    if (!exit_){
        free(args);
        exit(0);
    }
    
    clock_t start = clock();
    
    // if user agrees or no warning, get the sound done
    track_t* track_ = NULL;
    sound_t* sound = NULL;
    mix_t* mix4 = NULL;
    for (int i = 1; i<argc-1; i++){
        printf("\nLoading %s.. (%d/%d).\n", args[i], i, argc-2);
        filename = args[i];
        mix4 = load_mix(filename, true);
        if (mix4 == NULL){
            printf("%s is not a loading mix file, skipping...\n", filename);
        }else{
            sound = reduce_mix(mix4);
            track_ = add_sound(track_, sound);
            free_mix(mix4);
        }
    }
    
    if (track_ == NULL){
        printf("No sound to save, exiting...\n");
        free(args);
        exit(1);
    }

    // writes in the file
    filename = args[argc-1];
    file = fopen(filename, "w");
    save_sound(file, track_);
    fclose(file);
    
    
    // gives the user a recap of the sound
    int size = 0;
    int duree = (int)sound->duree;
    for (int i = 0; (unsigned int)i<track_->n_sounds; i++){
        size += track_->sounds[i]->n_sample;
    }
    size *= 2;
    int n_pow = 0;
    while (size > 1000) {
        size = (int)roundf((float)(size/1000));
        n_pow++;
    }
    
    free_tracks(track_);
    track_ = NULL;
    
    clock_t stop = clock();
    unsigned long t = (stop - start)/CLOCKS_PER_SEC;
    int sub = roundf((t-roundf(t))*10);
    printf("Fichier %s généré (temps écoulé: %d.%ds)\n", args[argc-1], (int)t, sub);
    free(args);
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
            break;
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
