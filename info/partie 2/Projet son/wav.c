#include "wav.h"
#include "sound.h"

bool assert_file(bool expression){
    if (expression){
        return true;
    }
    printf("File given isn't a wav file\n");
    return false;
}

bool write_int(FILE* file, int a, int size){
    if (size <0 || size >4){
        printf("Error: size of int to write mut be between 1 and 4\nGiven: %d\n", size);
        return false;
    }
    char emptyfill = 0;
    if (a<0) emptyfill = -1;
    for (int i = 0; i<size; i++){
        if (a == 0){
            fprintf(file, "%c", emptyfill);
        }else{
            fprintf(file, "%c", a%256);
        }
        a = a/256;
    }
    return true;
}

/* writes the correct headers for a wav file with n notes
   and n_channels channels */
void write_header(FILE* file, int n, int n_channels){
    fprintf(file, "RIFF"); // sur 4 octes
    write_int(file, 36+2*n*n_channels, 4);
    fprintf(file, "WAVEfmt "); // sur 8 octes
    write_int(file, 16, 4);
    write_int(file, 1, 2);
    write_int(file, n_channels, 2);
    write_int(file, 44100, 4);
    write_int(file, 88200, 4);
    write_int(file, 2, 2);
    write_int(file, 16, 2);
    fprintf(file, "data"); // sur 4 octes
    write_int(file, 2*n*n_channels, 4);
}

/* writes the notes of sound to the wav file */
void save_sound(FILE* file, track_t* track){
    for (int i = 0; (unsigned int)i<track->n_sounds; i++){
        if (track->sounds[i] == NULL){
            track->sounds[i] = malloc(sizeof(sound_t));
            track->sounds[i]->n_sample = 0;
            track->sounds[i]->samples = malloc(0*sizeof(int16_t));
            track->sounds[i]->duree = 0;
        }
    }
    write_header(file, track->duree*44100, track->n_sounds);
    for (int i = 0; (unsigned int)i< track->duree*44100; i++){
        for (int j = 0; (unsigned int)j<track->n_sounds; j++){
            if (track->sounds[j]->n_sample > (unsigned int)i){
                write_int(file, track->sounds[j]->samples[i], 2);
            }else{
                write_int(file, 0, 2);
            }
        }
    }
}

// converts the char into an int when read in the file
int convert(char c){
    int n = c;
    if (n<0){
        n += 256;
    }
    return n;
}

track_t* read_wav(FILE* file){
    // reading header
    int taille_H_D = 0;
    int taille_D = 0;
    int n_tracks = 0;
    int f_ech = 0;
    int bpm = 0;
    char c;
    if (!assert_file(check_str(file, "RIFF", 4))) return NULL;
    for (int i = 0; i<4; i++){
        fscanf(file, "%c", &c);
        taille_H_D = taille_H_D + convert(c) * pow(256, i);
    }
    if (!assert_file(check_str(file, "WAVEfmt ", 8))) return NULL;
    if (!assert_file(check_int(file, 16, 4))) return NULL;
    if (!assert_file(check_int(file, 1, 2))) return NULL;
    for (int i = 0; i<2; i++){
        fscanf(file, "%c", &c);
        n_tracks = n_tracks + convert(c) * pow(256, i);
    }
    for (int i = 0; i<4; i++){
        fscanf(file, "%c", &c);
        f_ech = f_ech + convert(c) * pow(256, i);
    }
    for (int i = 0; i<4; i++){
        fscanf(file, "%c", &c);
        bpm = bpm + convert(c) * pow(256, i);
    }
    bpm = (bpm*60)/f_ech;
    if (!assert_file(check_int(file, 2, 2))) return NULL;
    if (!assert_file(check_int(file, 16, 2))) return NULL;
    if (!assert_file(check_str(file, "data ", 4))) return NULL;
    for (int i = 0; i<4; i++){
        fscanf(file, "%c", &c);
        taille_D = taille_D + convert(c) * pow(256, i);
    }
    if (!assert_file(taille_D == taille_H_D-36)) return NULL;
    
    // read data
    track_t* track = malloc(sizeof(track_t));
    track->n_sounds = n_tracks;
    track->sounds = malloc(track->n_sounds*sizeof(sound_t*));
    for (int i = 0; i<n_tracks; i++){
        track->sounds[i] = malloc(sizeof(sound_t));
        track->sounds[i]->n_sample = taille_D / track->n_sounds;
        track->sounds[i]->samples = malloc(track->sounds[i]->n_sample*sizeof(int16_t));
    }
    for (int j = 0; (unsigned int)j < track->sounds[0]->n_sample; j++){
        for (int i = 0; (unsigned int)i < track->n_sounds; i++){
            fscanf(file, "%c", &c);
            track->sounds[i]->samples[j] = convert(c);
        }
    }
    track->duree = taille_D / (track->n_sounds*f_ech*(float)bpm)*60;
    return track;
    
    
}

bool check_str(FILE* file, char* str, int len){
    char c;
    for (int i = 0; i<len; i++){
        fscanf(file, "%c", &c);
        if (c != str[i]){
            return false;
        }
    }
    return true;
}

bool check_int(FILE* file, int n, int len){
    char c;
    char c_i;
    for (int i = 0; i<len; i++){
        fscanf(file, "%c", &c);
        c_i = n%256;
        if(c != c_i){
            return false;
        }
        n = n/256;
    }
    return true;
}
