#include "testmode.h"
// I chose this number because it has a small chance to be confused with a real number
#define maxint -2147483648

int str_to_int(char* str, bool display_error, char* var_name, int var_n){
    if (!strcmp(var_name, str)){
        return var_n;
    }
    int n = 0;
    int n2;
    int sign = 1;
    // the sign or firts number
    if (str[0] == '-'){
        sign = -1;
    }else{
        n = str[0]-48;
    }
    if (n < 0 || n > 9){
        if (display_error){
            printf("\"%s\" is not an integer\n", str);
        }
        return maxint;
    }
    // get the rest of the numbers
    for (int i = 1; str[i] != '\0'; i++){
        n2 = str[i]-48;
        if (n2 < 0 || n2 > 9){
            if (display_error){
                printf("\"%s\" is not an integer\n", str);
            }
            return maxint;
        }
        n = 10*n + n2;
    }
    return sign*n;
}

float str_to_float(char* str, bool display_error, char* var_name, int var_n){
    if (!strcmp(var_name, str)){
        return (float)var_n;
    }
    float n = 0;
    float n2;
    float sign = 1;
    
    // the sign or firts number
    if (str[0] == '-'){
        sign = -1;
    }else{
        n = str[0]-48;
    }
    if (n < 0 || n > 9){
        if (display_error){
            printf("\"%s\" is not a floating\n", str);
        }
        return maxint;
    }
    int index = 1;
    
    // get the rest of the numbers before the comma
    while (str[index] != '.' && str[index] != '\0') {
        n2 = str[index]-48;
        if (n2 < 0 || n2 > 9){
            if (display_error){
                printf("\"%s\" is not a floating\n", str);
            }
            return maxint;
        }
        n = 10*n + n2;
        index ++;
    }
    
    if (str[index] == '\0'){
        return sign*n;
    }
    int index2 = 1;
    index ++;
    
    // get the rest of the number
    while (str[index] != '.' && str[index] != '\0') {
        n2 = str[index]-48;
        if (n2 < 0 || n2 > 9){
            if (display_error){
                printf("\"%s\" is not a floating\n", str);
            }
            return maxint;
        }
        n = n + pow(0.1, index2)*n2;
        index2 ++;
        index ++;
    }
    return sign*n;
}

bool warn_file(char* filename, bool nowarn){
    if (nowarn) return true;
    FILE* file = fopen(filename, "r");
    char c;
    if (file != NULL){
        fclose(file);
        printf("%s already exists, do you want to replace it? y/n\n", filename);
        scanf("%c", &c);
        switch (c) {
            case 'n':
                return false;
            case 'y':
                return true;
            default:
                return false; // if none of them, counted as a no
        }
    }
    return true;
}

void test_mode(bool nowarn, bool debugmode){
    printf("Welcome in test mode\n");
    printf("Here, you can test all the function implemented in this project\n");
    printf("To see all the available functionalities type \"help\"\n");
    printf("To exit this program, type \"exit\"\n");
    
    // variables accesible to the user
    float n = 0;
    sound_t* sound = NULL;
    track_t* track = NULL;
    mix_t* mix = NULL;
    // variables used to transfer data across commands
    sound_t* tmp_sound = NULL;
    track_t* tmp_track = NULL;
    mix_t* tmp_mix = NULL;
    
    char arg[50];
    int n_arg;
    tab_t* tab = NULL;

    char c;
    int n_command = 0;
    bool failed_output;
    bool is_output_variable;
    bool is_function = false;
    int variable_index;
    
    FILE* file = NULL;
    // main loop
    while (true) {
        variable_index = -1;
        is_output_variable = false;
        failed_output = false;
        n_command = 0;
        n_arg = 0;
        c = '\0';
        tab = reset_tab(tab);
        printf("> ");
        // get the entry and separate it in words
        // don't be lazy and do it with chars instead of string to catch spaces better
        while (c != '\n') {
            arg[0] = '\0';
            scanf("%s", arg);
            if (debugmode){
                printf("%s ", arg);
            }
            if (!strcmp(arg, ">>")){
                if (n_arg == 0){
                    tab = add_arg(tab, n_command, arg);
                }
                tab = add_line(tab);
                n_command ++;
                n_arg = 0;
            }else{
                tab = add_arg(tab, n_command, arg);
                n_arg++;
            }
            scanf("%c", &c);
        }
        if (debugmode){
            printf("\n");
        }
        
        // treat the entry
        bool output;
        char* output_type;
        int int1 = 0, int2 = 0;
        float float1 = 0.0, float2 = 0.0;
        output_type = "None";
        for (int i = 0; i < tab->len; i++){
            is_function = false;
            output = (i == tab->len -1);
            if (!strcmp(tab->args[i][0], "n") && tab->tab_len[i] == 1){
                if (!strcmp(output_type, "integer")){
                    n = int1;
                }else if (!strcmp(output_type, "floating")){
                    n = float1;
                }else if (strcmp(output_type, "None")){
                    failed_output = true;
                    break;
                }
                if (output){
                    if (n-roundf(n)==0){
                        printf("%d\n", (int)n);
                    }else{
                        printf("%f\n", n);
                    }
                }else{
                    is_output_variable = true;
                    if (n-roundf(n)==0){
                        output_type = "integer";
                        int1 = n;
                    }else{
                        output_type = "floating";
                        float1 = n;
                    }
                    n = 0;
                }
            }
            else if (!strcmp(tab->args[i][0], "sound") && tab->tab_len[i] == 1){
                if (!strcmp(output_type, "sound")){
                    if (sound != NULL){
                        free_sound(sound);
                    }
                    sound = tmp_sound;
                    tmp_sound = NULL;
                }else if (!strcmp(output_type, "track")){
                    if (tmp_track == NULL){
                        printf("Given track is empty\n");
                        break;
                    }
                    if (sound != NULL){
                        free_sound(sound);
                    }
                    sound = reduce_track(tmp_track);
                    free_tracks(tmp_track);
                    tmp_track = NULL;
                }else if (!strcmp(output_type, "mix")){
                    if (tmp_mix == NULL){
                        printf("Given mix is empty\n");
                        break;
                    }
                    if (sound != NULL){
                        free_sound(sound);
                    }
                    sound = reduce_mix(tmp_mix);
                    free_mix(tmp_mix);
                    tmp_mix = NULL;
                }else if (strcmp(output_type, "None")){
                    failed_output = true;
                    break;
                }
                if (output){
                    if (sound == NULL || sound->n_sample == 0){
                        printf("an empty sound\n");
                    }else{
                        printf("sound of lenght %f, with %d samples\n", sound->duree, sound->n_sample);
                    }
                }else{
                    tmp_sound = sound;
                    sound = NULL;
                    output_type = "sound";
                    is_output_variable = true;
                }
            }
            else if (!strcmp(tab->args[i][0], "track") && tab->tab_len[i] <= 2){
                if (tab->tab_len[i] == 2){
                    int1 = str_to_int(tab->args[i][1], true, "n", n);
                    if (int1 == maxint){
                        failed_output = true;
                        break;
                    }
                    if (track == NULL || track->n_sounds == 0){
                        printf("track is empty\n");
                        break;
                    }
                    if (int1 >= (int)track->n_sounds || int1<0){
                        printf("Sound index out of range (max=%d, given=%d)\n", (int)track->n_sounds-1, int1);
                        failed_output = true;
                        break;
                    }
                    if (!strcmp(output_type, "sound")){
                        if (tmp_sound == NULL || tmp_sound->duree == 0){
                            printf("Sound given is empty\n");
                            break;
                        }
                        free_sound(track->sounds[int1]);
                        track->sounds[int1] = tmp_sound;
                        tmp_sound = NULL;
                    }
                    else if (!strcmp(output_type, "mix")){
                        if (tmp_mix == NULL || tmp_mix->n_tracks == 0){
                            printf("Mix given is empty\n");
                            break;
                        }
                        tmp_sound = reduce_mix(tmp_mix);
                        free_mix(tmp_mix);
                        tmp_mix = NULL;
                        free_sound(track->sounds[int1]);
                        track->sounds[int1] = tmp_sound;
                        tmp_sound = NULL;
                    }
                    else if (strcmp(output_type, "None")) {
                        printf("Argument given has to be sound\n");
                        break;
                    }
                    if (output){
                        printf("sound of lenght %f, with %d samples\n", track->sounds[int1]->duree, track->sounds[int1]->n_sample);
                    }else{
                        variable_index = int1;
                        tmp_sound = track->sounds[variable_index];
                        track->sounds[variable_index] = NULL;
                        output_type = "sound";
                        is_output_variable = true;
                    }
                }else{
                    if (!strcmp(output_type, "track")){
                        if (track != NULL){
                            free_tracks(track);
                        }
                        track = tmp_track;
                        tmp_track = NULL;
                        output_type = "None";
                    }else if (!strcmp(output_type, "sound")){
                        if (tmp_sound == NULL || tmp_sound->duree == 0){
                            printf("Sound given is empty\n");
                            break;
                        }
                        track = add_sound(track, tmp_sound);
                        tmp_sound = NULL;
                    }else if (!strcmp(output_type, "mix")){
                        if (tmp_mix == NULL || tmp_mix->n_tracks == 0){
                            printf("Mix given is empty\n");
                            break;
                        }
                        tmp_sound = reduce_mix(tmp_mix);
                        free_mix(tmp_mix);
                        tmp_mix = NULL;
                        track = add_sound(track, tmp_sound);
                        tmp_sound = NULL;
                    }else if (strcmp(output_type, "None")){
                        failed_output = true;
                        break;
                    }
                    if (output){
                        if (track == NULL){
                            printf("an empty track\n");
                        }else{
                            printf("track of lenght %f, with %d sounds\n", track->duree, (int)track->n_sounds);
                        }
                    }else{
                        tmp_track = track;
                        track = NULL;
                        output_type = "track";
                        is_output_variable = true;
                    }
                }
            }
            else if (!strcmp(tab->args[i][0], "mix") && tab->tab_len[i] <= 2){
                if (tab->tab_len[i] == 2){
                    int1 = str_to_int(tab->args[i][1], true, "n", n);
                    if (int1 == maxint){
                        failed_output = true;
                        break;
                    }
                    if (mix == NULL || mix->n_tracks == 0){
                        printf("mix is empty\n");
                        if (strcmp(output_type, "None")){
                            failed_output = true;
                        }
                        break;
                    }
                    if (int1 >= (int)mix->n_tracks || int1 < 0){
                        printf("Track index out of range (max=%d, given=%d)\n", (int)mix->n_tracks-1, int1);
                        failed_output = true;
                        break;
                    }
                    if (!strcmp(output_type, "track")){
                        if (tmp_track == NULL || tmp_track->duree == 0){
                            printf("track given is empty\n");
                            break;
                        }
                        free_tracks(mix->tracks[int1]);
                        mix->tracks[int1] = tmp_track;
                        mix->vols = malloc(sizeof(float) * mix->n_tracks);
                        for (int j = 0; j < (int)mix->n_tracks; j++){
                            mix->vols[j] = 1;
                        }
                        tmp_track = NULL;
                    }
                    else if (strcmp(output_type, "None")) {
                        printf("Argument given has to be track\n");
                        break;
                    }
                    if (output){
                        printf("track of lenght %f, with %d sound\n", mix->tracks[int1]->duree, mix->tracks[int1]->n_sounds);
                    }else{
                        variable_index = int1;
                        tmp_track = mix->tracks[variable_index];
                        mix->tracks[variable_index] = NULL;
                        output_type = "track";
                        is_output_variable = true;
                    }
                }else{
                    if (!strcmp(output_type, "mix")){
                        if (mix != NULL){
                            free_mix(mix);
                        }
                        mix = tmp_mix;
                        tmp_mix = NULL;
                        output_type = "None";
                    }else if (!strcmp(output_type, "track")){
                        if (tmp_track == NULL || tmp_track->n_sounds == 0){
                            printf("track given is empty\n");
                            break;
                        }else{
                            mix = add_track(mix, tmp_track);
                            tmp_track = NULL;
                        }
                    }else if (strcmp(output_type, "None")){
                        failed_output = true;
                        break;
                    }
                    if (output){
                        if (mix ==  NULL || mix->n_tracks == 0){
                            printf("an empty mix\n");
                        }else{
                            printf("mix of %d tracks\n", mix->n_tracks);
                        }
                    }else{
                        tmp_mix = mix;
                        mix = NULL;
                        output_type = "mix";
                        is_output_variable = true;
                    }
                }
            }
            else if (!strcmp(tab->args[i][0], "exit")){
                if (strcmp(output_type, "None")){
                    failed_output = true;
                    is_function = true;
                    break;
                }
                if (!output){
                    printf("\"exit\" function does not give any output\n");
                    break;
                }
                if (tab->tab_len[i] > 1){
                    printf("The function \"exit\" does not take any argument\n");
                    printf("Usage: exit\n");
                    break;
                }
                printf("Exiting program...\n");
                free_tab(tab);
                if (mix != NULL){
                    free_mix(mix);
                }
                if (track != NULL){
                    free_tracks(track);
                }
                if(sound != NULL){
                    free_sound(sound);
                }
                if (tmp_mix != NULL){
                    free_mix(tmp_mix);
                }
                if (tmp_track != NULL){
                    free_tracks(tmp_track);
                }
                if(tmp_sound != NULL){
                    free_sound(tmp_sound);
                }
                return;
            }
            else if (!strcmp(tab->args[i][0], "help")){
                if (strcmp(output_type, "None")){
                    failed_output = true;
                    is_function = true;
                    break;
                }
                if (!output){
                    printf("\"help\" function does not give any output\n");
                    break;
                }
                if (tab->tab_len[i] > 1){
                    printf("The function \"help\" does not take any argument\n");
                    printf("Usage: help\n");
                    break;
                }
                help(debugmode);
            }
            else if (!strcmp(tab->args[i][0], "white")){
                if (strcmp(output_type, "None")){
                    failed_output = true;
                    is_function = true;
                    break;
                }
                if (tab->tab_len[i] == 1){
                    printf("Usage: white <float duree>\n");
                    break;
                }
                if (tab->tab_len[i] != 2){
                    printf("The function \"white\" takes exactly 1 argument\n");
                    printf("Usage: white <float duree>\n");
                    break;
                }
                float1 = str_to_float(tab->args[i][1], true, "n", n);
                if (float1 == maxint){
                    break;
                }
                tmp_sound = white(float1);
                if (output){
                    printf("white of lenght %f, with %d samples\n", tmp_sound->duree, tmp_sound->n_sample);
                    free_sound(tmp_sound);
                    tmp_sound = NULL;
                }else{
                    output_type = "sound";
                }
            }
            else if (!strcmp(tab->args[i][0], "sine")){
                if (strcmp(output_type, "None")){
                    failed_output = true;
                    is_function = true;
                    break;
                }
                if(tab->tab_len[i] == 1){
                    printf("Usage: sine <float freq> <int amplitude> <float duree>\n");
                    break;
                }
                if (tab->tab_len[i] != 4){
                    printf("The function \"sine\" takes exactly 3 argument\n");
                    printf("Usage: sine <float freq> <int amplitude> <float duree>\n");
                    break;
                }
                float1 = str_to_float(tab->args[i][1], true, "n", n);
                int1 = str_to_int(tab->args[i][2], true, "n", n);
                float2 = str_to_float(tab->args[i][3], true, "n", n);
                if (float1 == maxint || int1 == maxint || float2 == maxint){
                    break;
                }
                tmp_sound = sine(float1, int1, float2);
                if (output){
                    printf("sine of lenght %f, with %d samples\n", tmp_sound->duree, tmp_sound->n_sample);
                    free_sound(tmp_sound);
                    tmp_sound = NULL;
                }else{
                    output_type = "sound";
                }
            }
            else if (!strcmp(tab->args[i][0], "sawtooth")){
                if (strcmp(output_type, "None")){
                    failed_output = true;
                    is_function = true;
                    break;
                }
                if (tab->tab_len[i] == 1){
                    printf("Usage: sawtooth <float freq> <int amplitude> <float duree>\n");
                    break;
                }
                if (tab->tab_len[i] != 4){
                    printf("The function \"sawtooth\" takes exactly 3 argument\n");
                    printf("Usage: sawtooth <float freq> <int amplitude> <float duree>\n");
                    break;
                }
                float1 = str_to_float(tab->args[i][1], true, "n", n);
                int1 = str_to_int(tab->args[i][2], true, "n", n);
                float2 = str_to_float(tab->args[i][3], true, "n", n);
                if (float1 == maxint || int1 == maxint || float2 == maxint){
                    break;
                }
                tmp_sound = sawtooth(float1, int1, float2);
                if (output){
                    printf("sawtooth of lenght %f, with %d samples\n", tmp_sound->duree, tmp_sound->n_sample);
                    free_sound(tmp_sound);
                    tmp_sound = NULL;
                }else{
                    output_type = "sound";
                }
            }
            else if (!strcmp(tab->args[i][0], "triangle")){
                if (strcmp(output_type, "None")){
                    failed_output = true;
                    is_function = true;
                    break;
                }
                if (tab->tab_len[i] == 1){
                    printf("Usage: triangle <float freq> <int amplitude> <float duree>\n");
                    break;
                }
                if (tab->tab_len[i] != 4){
                    printf("The function \"triangle\" takes exactly 3 argument\n");
                    printf("Usage: triangle <float freq> <int amplitude> <float duree>\n");
                    break;
                }
                float1 = str_to_float(tab->args[i][1], true, "n", n);
                int1 = str_to_int(tab->args[i][2], true, "n", n);
                float2 = str_to_float(tab->args[i][3], true, "n", n);
                if (float1 == maxint || int1 == maxint || float2 == maxint){
                    break;
                }
                
                tmp_sound = triangle(float1, int1, float2);
                if (output){
                    printf("triangle of lenght %f, with %d samples\n", tmp_sound->duree, tmp_sound->n_sample);
                    free_sound(tmp_sound);
                    tmp_sound = NULL;
                }else{
                    output_type = "sound";
                }
            }
            else if (!strcmp(tab->args[i][0], "square")){
                if (strcmp(output_type, "None")){
                    failed_output = true;
                    is_function = true;
                    break;
                }
                if (tab->tab_len[i] == 1){
                    printf("Usage: square <float freq> <int amplitude> <float duree>\n");
                    break;
                }
                if (tab->tab_len[i] != 4){
                    printf("The function \"square\" takes exactly 3 argument\n");
                    printf("Usage: square <float freq> <int amplitude> <float duree>\n");
                    break;
                }
                float1 = str_to_float(tab->args[i][1], true, "n", n);
                int1 = str_to_int(tab->args[i][2], true, "n", n);
                float2 = str_to_float(tab->args[i][3], true, "n", n);
                if (float1 == maxint || int1 == maxint || float2 == maxint){
                    break;
                }
                tmp_sound = triangle(float1, int1, float2);
                if (output){
                    printf("square of lenght %f, with %d samples\n", tmp_sound->duree, tmp_sound->n_sample);
                    free_sound(tmp_sound);
                    tmp_sound = NULL;
                }else{
                    output_type = "sound";
                }
            }
            else if (!strcmp(tab->args[i][0], "write_int")){
                if (strcmp(output_type, "None")){
                    failed_output = true;
                    is_function = true;
                    break;
                }
                if (!output){
                    printf("\"write_int\" function does not retun any output\n");
                    break;
                }
                if (tab->tab_len[i] == 1){
                    printf("Usage: write_int <FILE file> <int n>, <int size>\n");
                    break;
                }
                if (tab->tab_len[i] != 4){
                    printf("The function \"write_int\" takes exactly 3 argument\n");
                    printf("Usage: write_int <FILE file> <int n>, <int size>");
                    break;
                }
                if (!warn_file(tab->args[i][1], nowarn)){
                    break;
                }
                file = fopen(tab->args[i][1], "w");
                int1 = str_to_int(tab->args[i][2], true, "n", n);
                int2 = str_to_int(tab->args[i][3], true, "n", n);
                if (int1 == maxint || int2 == maxint){
                    fclose(file);
                    break;
                }
                if (!write_int(file, int1, int2)){
                    fclose(file);
                    break;
                }
                fclose(file);
                if (output){
                    printf("\"%d\" written on %d bytes in %s\n", int1, int2, tab->args[i][1]);
                }else{
                    output_type = "sound";
                }
            }
            else if (!strcmp(tab->args[i][0], "read_wav")){
                if (strcmp(output_type, "None")){
                    is_function = true;
                    failed_output = true;
                    break;
                }
                if (tab->tab_len[i] == 1){
                    printf("Usage: read_wav <FILE file>\n");
                    break;
                }
                if (tab->tab_len[i] != 2){
                    printf("The function \"read_wav\" takes exactly 1 argument\n");
                    printf("Usage: read_wav <FILE file>\n");
                    break;
                }
                file = fopen(tab->args[i][1], "r");
                if (file == NULL){
                    printf("Cant' find %s\n", tab->args[i][1]);
                    break;
                }
                tmp_track = read_wav(file);
                if (tmp_track == NULL){
                    break;
                }
                if (output){
                    printf("track of lenght %f, with %d sounds\n", tmp_track->duree, tmp_track->n_sounds);
                    free_tracks(tmp_track);
                    tmp_track = NULL;
                }else{
                    output_type = "track";
                }
            }
            else if (!strcmp(tab->args[i][0], "load_mix")){ // done
                if (strcmp(output_type, "None")){
                    is_function = true;
                    failed_output = true;
                    break;
                }
                if (tab->tab_len[i] == 1){
                    printf("Usage: load_mix <FILE file>\n");
                    break;
                }
                if (tab->tab_len[i] != 2){
                    printf("The function \"load_mix\" takes exactly 1 argument\n");
                    printf("Usage: load_mix <FILE file>\n");
                    break;
                }
                file = fopen(tab->args[i][1], "r");
                if (file == NULL){
                    printf("Can't find %s\n", tab->args[i][1]);
                    break;
                }
                fclose(file);
                tmp_mix = load_mix(tab->args[i][1], true);
                if (tmp_mix == NULL){
                    break;
                }
                if (output){
                    printf("mix of %d tracks\n", tmp_mix->n_tracks);
                    free_mix(tmp_mix);
                    tmp_mix = NULL;
                }else{
                    output_type = "mix";
                }
            }
            else if (!strcmp(tab->args[i][0], "save_sound")){ // done
                if (strcmp(output_type, "None")){
                    is_function = true;
                    failed_output = true;
                    break;
                }
                if (!output){
                    printf("\"save_sound\" function does not give any output\n");
                    break;
                }
                if (tab->tab_len[i] == 1){
                    printf("Usage: save_sound <FILE file>\n");
                    break;
                }
                if (tab->tab_len[i] != 2){
                    printf("The function \"save_sound\" takes exactly 1 argument\n");
                    printf("Usage: save_sound <FILE file>\n");
                    break;
                }
                if (warn_file(tab->args[i][1], nowarn)){
                    if (track == NULL || track->duree == 0){
                        printf("Track is empty\n");
                        break;
                    }
                    file = fopen(tab->args[i][1], "w");
                    save_sound(file, track);
                    free_tracks(track);
                    track = NULL;
                    printf("Saved track to \"%s\"\n", tab->args[i][1]);
                    fclose(file);
                }
            }
            else if (!strcmp(tab->args[i][0], "add_sound")){
                if (strcmp(output_type, "None")){
                    is_function = true;
                    failed_output = true;
                    break;
                }
                if (!output){
                    printf("\"add_sound\" function does not give any output\n");
                    break;
                }
                if (tab->tab_len[i] > 1){
                    printf("The function \"add_sound\" does not take any argument\n");
                    printf("Usage: add_sound\n");
                    break;
                }
                if (sound == NULL || sound->duree == 0){
                    printf("Sound is empty\n");
                    break;
                }
                if (track != NULL){
                    free_tracks(track);
                }
                track = add_sound(track, sound);
                sound = NULL;
                printf("track of lenght %f, with %d sounds\n", track->duree, (int)track->n_sounds);
            }
            else if (!strcmp(tab->args[i][0], "add_track")){
                if (strcmp(output_type, "None")){
                    is_function = true;
                    failed_output = true;
                    break;
                }
                if (!output){
                    printf("\"add_track\" function does not give any output\n");
                    break;
                }
                if (tab->tab_len[i] > 1){
                    printf("The function \"add_track\" does not take any argument\n");
                    printf("Usage: add_track\n");
                    break;
                }
                if (track == NULL || track->duree == 0){
                    printf("track is empty\n");
                    break;
                }
                mix = add_track(mix, track);
                track = NULL;
                printf("mix of %d tracks\n", mix->n_tracks);
            }
            else if (!strcmp(tab->args[i][0], "reduce_track")){
                if (strcmp(output_type, "None")){
                    is_function = true;
                    failed_output = true;
                    break;
                }
                if (!output){
                    printf("\"reduce_track\" function does not give any output\n");
                    break;
                }
                if (tab->tab_len[i] > 1){
                    printf("The function \"recude_track\" does not take any argument\n");
                    printf("Usage: reduce_track\n");
                    break;
                }
                if (track == NULL || track->duree == 0){
                    printf("track is empty\n");
                    break;
                }
                if (sound != NULL){
                    free_sound(sound);
                }
                sound = reduce_track(track);
                free_tracks(track);
                track =  NULL;
                printf("sound of lenght %f, with %d samples\n", sound->duree, sound->n_sample);
            }
            else if (!strcmp(tab->args[i][0], "reduce_mix")){
                if (strcmp(output_type, "None")){
                    is_function = true;
                    failed_output = true;
                    break;
                }
                if (!output){
                    printf("\"reduce_mix\" function does not give any output\n");
                    break;
                }
                if (tab->tab_len[i] > 1){
                    printf("The function \"reduce_mix\" does not take any argument\n");
                    printf("Usage: reduce_mix\n");
                    break;
                }
                if (mix == NULL || mix->n_tracks == 0){
                    printf("mix is empty\n");
                    break;
                }
                if (sound != NULL){
                    free_sound(sound);
                }
                sound = reduce_mix(mix);
                free_mix(mix);
                mix = NULL;
                printf("sound of lenght %f, with %d samples\n", sound->duree, sound->n_sample);
            }
            else if (!strcmp(tab->args[i][0], "pitch_to_freq")){
                if (strcmp(output_type, "None")){
                    failed_output = true;
                    is_function = true;
                    break;
                }
                if (tab->tab_len[i] == 1){
                    printf("Usage: pitch_to_freq <integer n>\n");
                    break;
                }
                if (tab->tab_len[i] != 2){
                    printf("The function \"pitch_to_freq\" takes exactly 1 argument\n");
                    printf("Usage: pitch_to_freq <integer n>\n");
                    break;
                }
                if (!strcmp(tab->args[i][1], "n")){
                    int2 = n;
                }else{
                    int2 = str_to_int(tab->args[i][1], true, "n", n);
                }
                if (int2 == maxint){
                    break;
                }
                int1 = pitch_to_freq(int2);
                if (output){
                    printf("a %d pitch correspond to a %d frequency\n", int2, int1);
                }else{
                    output_type = "integer";
                }
            }
            else{
                /* if none of the following command matches, seaches for numbers of files
                   checks for numbers (either integer or floating) */
                int1 = str_to_int(tab->args[i][0], false, "n", n);
                float1 = str_to_float(tab->args[i][0], false, "n", n);
                if(int1 != maxint && tab->tab_len[i] == 1){
                    if (output){
                        printf("%d\n", int1);
                    }else{
                        output_type = "integer";
                    }
                }else if (float1 != maxint && tab->tab_len[i] == 1){
                    if (output){
                        printf("%f\n", float1);
                    }else{
                        output_type = "floating";
                    }
                }else{
                    /* checks for files */
                    bool is_file = false;
                    for (int j = 0; (unsigned long)j< strlen(tab->args[i][0]); j++){
                        if (tab->args[i][0][j] == '.'){
                            is_file = true;
                        }
                    }
                    if (is_file){
                        if (i == 0){
                            file = fopen(tab->args[i][0], "r");
                            if(file == NULL){
                                printf("\"%s\" not found\n", tab->args[i][0]);
                                break;
                            }
                            fclose(file);
                            tmp_mix = load_mix(tab->args[i][0], false);
                            if (tmp_mix == NULL){
                                printf("File isn't a mix\n");
                                file = fopen(tab->args[i][0], "r");
                                tmp_track = read_wav(file);
                                fclose(file);
                                if (tmp_track == NULL){
                                    break;
                                }else{
                                    printf("File is a wav of lenght %f with %d sounds\n", tmp_track->duree, (int)tmp_track->n_sounds);
                                }
                            }
                            else{
                                if (output){
                                    printf("mix of %d tracks\n", tmp_mix->n_tracks);
                                    free_mix(tmp_mix);
                                    tmp_mix = NULL;
                                }else{
                                    output_type = "mix";
                                }
                            }
                        }
                        else{
                            if (!strcmp(output_type, "track")){
                                if (tmp_track == NULL){
                                    printf("Error : track is empty\n");
                                    break;
                                }
                            }
                            else if (!strcmp(output_type, "mix")){
                                if (tmp_mix == NULL){
                                    printf("Error : mix is empty\n");
                                    break;
                                }
                                tmp_sound = reduce_mix(tmp_mix);
                                free_mix(tmp_mix);
                                tmp_mix = NULL;
                                tmp_track = add_sound(tmp_track, tmp_sound);
                                tmp_sound = NULL;
                            }
                            else if (!strcmp(output_type, "sound")){
                                if (tmp_sound == NULL){
                                    printf("Error : sound is empty\n");
                                    break;
                                }
                                tmp_track = add_sound(tmp_track, tmp_sound);
                                tmp_sound = NULL;
                            }
                            else if (strcmp(output_type, "None")){
                                failed_output = true;
                                break;
                            }
                            if (warn_file(tab->args[i][0], nowarn)){
                                file = fopen(tab->args[i][0], "w");
                                save_sound(file, tmp_track);
                                printf("Sound saved to \"%s\"\n", tab->args[i][0]);
                                fclose(file);
                            }
                            free_tracks(tmp_track);
                            tmp_track = NULL;
                        }
                    }else{
                        /* if all the tests failed, it doesn't exists and shows an error */
                        printf("function \"%s\" is not recognized\n", tab->args[i][0]);
                        if (is_output_variable){
                            failed_output = true;
                        }else{
                            if (tmp_sound != NULL){
                                free_sound(tmp_sound);
                                tmp_sound = NULL;
                            }
                            if (tmp_track != NULL){
                                free_tracks(tmp_track);
                                tmp_track = NULL;
                            }
                            if (tmp_mix != NULL){
                                free_mix(tmp_mix);
                                tmp_mix = NULL;
                            }
                        }
                        break;
                    }
                }
            }
        }
        /* returns error when the data shared with
           the next function isn't the right type */
        if (failed_output){
            if (is_function){
                printf("Syntax Error: reciever is not a variable\n");
            }else{
                printf("Syntax Error: argument given is invalid\n");
            }
            
            if (variable_index != -1){
                if (tmp_sound != NULL){
                    track->sounds[variable_index] = tmp_sound;
                    tmp_sound = NULL;
                }else if (tmp_track != NULL){
                    mix->tracks[variable_index] = tmp_track;
                    tmp_track = NULL;
                }
            }else if (is_output_variable){
                if (!strcmp(output_type, "sound")){
                    sound = tmp_sound;
                    tmp_sound = NULL;
                }else if (!strcmp(output_type, "track")){
                    track = tmp_track;
                    tmp_track = NULL;
                }else if (!strcmp(output_type, "mix")){
                    mix = tmp_mix;
                    tmp_mix = NULL;
                }else if (!strcmp(output_type, "integer")){
                    n = int1;
                }else if (!strcmp(output_type, "floating")){
                    n = float1;
                }
            }
            if (tmp_sound != NULL){
                free_sound(tmp_sound);
                tmp_sound = NULL;
            }
            if (tmp_track != NULL){
                free_tracks(tmp_track);
                tmp_track = NULL;
            }
            if (tmp_mix != NULL){
                free_mix(tmp_mix);
                tmp_mix = NULL;
            }
        }
        /* Handles the track and the mix being used as iterables
           by changing them only if the change as been made */
        else if (variable_index != -1){
            if (tmp_sound == NULL && track != NULL && (unsigned int)variable_index < track->n_sounds && track->sounds[variable_index] == NULL){
                track->n_sounds --;
                track->duree = 0;
                for (int j = variable_index; j<(int)track->n_sounds; j++){
                    track->sounds[j] = track->sounds[j+1];
                    if (track->sounds[j]->duree > track->duree){
                        track->duree = track->sounds[j]->duree;
                    }
                }
                track->sounds = realloc(track->sounds, (int)track->n_sounds*sizeof(sound_t*));
            }else if (tmp_track == NULL && mix != NULL && (unsigned int)variable_index < mix->n_tracks && mix->tracks[variable_index] == NULL){
                mix->n_tracks --;
                for (int j = variable_index; j<(int)mix->n_tracks; j++){
                    mix->tracks[j] = mix->tracks[j+1];
                    mix->vols[j] = mix->vols[j+1];
                }
                mix->tracks = realloc(mix->tracks, (int)mix->n_tracks*sizeof(track_t*));
                mix->vols = realloc(mix->vols, (int)mix->n_tracks*sizeof(float));
            }
        }
        printf("\n");
    }
}

void next_page(bool debugmode){
    char c = '\0';
    printf("press [ENTER] to continue\n");
    while (c != '\n' && !debugmode) {
        scanf("%c", &c);
    }
    if (debugmode){
        printf("\n");
    }
}

void help(bool debugmode){
    printf("Here is a list of what you can do in test mode:\n\n");
    
    printf("===== variables =====\n\n");
    printf("There are 4 variables which can be used:\n");
    printf("* n, which is an integer\n");
    printf("* sound, which represent a sound to be stored in a wav file\n");
    printf("* track, which is a sort of list of sounds, which plays one at the time\n");
    printf("* mix, which is a sort of list of tracks which plays all together\n\n");
    
    printf("You can use variables to store values from function\nand use them as arguments in function\n\n");
    next_page(debugmode);
    
    printf("===== functions =====\n\n");
    printf("There are a lot of functions to choose from,\ndivided in 4 categories:\n\n");
    printf("1. Function to create sounds:\n");
    printf(" * white <float duree>\n    => returns a sound\n");
    printf("    generates a white noise i.e. a sound with random amplitude\n");
    printf(" * sine <float freq> <int amplitude> <float duree>\n");
    printf("    generates a sine with freq as frequency, amplitude as amplitude\n");
    printf("    and duree as lenght\n");
    printf("    => returns a sound\n\n");
    printf(" * square <float freq> <int amplitude> <float duree>\n");
    printf("    generates a square shaped sound with freq as frequency, amplitude as amplitude\n");
    printf("    and duree as lenght\n");
    printf("    => returns a sound\n\n");
    printf(" * triangle <float freq> <int amplitude> <float duree>\n");
    printf("    generates a triangle shaped sound with freq as frequency, amplitude as amplitude\n");
    printf("    and duree as lenght\n");
    printf("    => returns a sound\n\n");
    printf(" * sawtooth <float freq> <int amplitude> <float duree>\n");
    printf("    generates a sawtooth shaped sound with freq as frequency, amplitude as amplitude\n");
    printf("    and duree as lenght\n");
    printf("    => returns a sound\n\n");
    next_page(debugmode);
    
    // file modification
    printf("2. Function to modify files:\n");
    printf(" * write_int <FILE file> <int n>, <int size>\n");
    printf("    writes the integer given in the file on size bytes\n");
    printf("    => returns nothing\n");
    printf(" * read_wav <FILE file>\n");
    printf("    reads the sound for a wav file\n");
    printf("    => returns a track\n");
    printf(" * load_mix <FILE file>\n");
    printf("    reads a mix for a txt file\n");
    printf("    => returns a mix\n");
    printf(" * save_sound <FILE file>\n");
    printf("    saves the current track into the file\n");
    printf("    => returns nothing\n\n");
    next_page(debugmode);
    
    // modify different types of variables
    printf("3. Function to modify variables:\n");
    printf(" * add_sound\n");
    printf("    adds the sound to the track\n");
    printf("    => returns nothing\n");
    printf(" * add_track\n");
    printf("    adds the track to the mix\n");
    printf("    => returns nothing\n");
    printf(" * reduce_track\n");
    printf("    create a sound out of the track\n");
    printf("    => returns a sound\n");
    printf(" * reduce_mix\n");
    printf("    create a sound out of the mix\n");
    printf("    => returns a sound\n\n");
    next_page(debugmode);
    
    printf("4. Other functions:\n");
    printf(" * pitch_to_freq <int pitch>\n");
    printf("    gives the frequency of the given pitch\n");
    printf("    => returns an integer\n\n");
    next_page(debugmode);
    
    printf("===== store/use variables =====\n\n");
    printf("The operator \">>\" can be used as following:\n");
    printf("function >> variable, which stores the result of the function to the variable\n");
    printf("sound >> track, which is the same as \"add_sound >> track\"\n");
    printf("track >> mix, which is the same as \"add_track >> mix\"\n");
    printf("sound >> file, which is the same as \"save_sound file\"\n");
    printf("\">>\" operators can be chained like: sound >> track >> mix >> sound >> file\n");
    printf("You can also use variables as arguments for functions\n");
    printf("Finally, you can see some infos of the variable by typing its name\n");
}
