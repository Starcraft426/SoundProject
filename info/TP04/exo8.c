#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define char_width 8
#define char_height 8
//#define line_width 9*char_width

typedef struct _char{
    int width;
    int height;
    char* str;
    size_t len;
} _char_t;

_char_t* img_filename(char c, int max_size, char drawing_c){
    FILE* file;
    _char_t* b_char = malloc(sizeof(_char_t));
    b_char->width = max_size;
    b_char->height = 0;
    b_char->str = NULL;
    b_char->len = 0;
    char name[15] = {'l','e','t','t','r','e','s','/',c,c,'\0'};

    char drawing_str[2] = {drawing_c, '\0'};

    if (('a'<=c && c<='z')||('0'<=c && c<='9')){
        name[9] = '\0';
    }else if (c == ' '){
        name[8] = '\0';
    }else if (!('A'<=c && c<='Z')){
        free(b_char);
        return NULL;
    }
    
    strcat(name, ".txt");
    file = fopen(name, "r");
    assert(file != NULL);

    char* ligne = malloc(max_size*sizeof(char));
    char c2[2];
    
    int len = 1;
    char* lettre = malloc(len*sizeof(char));
    b_char->str = malloc(len*sizeof(char));
    len--;

    int n = -1;
    int index = 0;
    while(fscanf(file, "%d", &n) != EOF){
        for (int i = 0; i < max_size+1; i++){
            ligne[i] = '\0';
        }

        fscanf(file, " ");
        len += (max_size+1);
        lettre = realloc(lettre, len*sizeof(char));
        b_char->str = realloc(b_char->str, len*sizeof(char));
        
        b_char->len = len*sizeof(char);

        for (int i = 0; i<max_size; i++){
            if (n%2){
                strcat(ligne, drawing_str);
            }else{
                strcat(ligne, " ");
            }
            n = n/2;
        }

        for (int i = 0; i<max_size; i++){
            c2[0] = ligne[max_size-1-i];
            strcat(lettre, c2);
        }

        strcat(lettre, "\n");
        b_char->height ++;
        index ++;
    }
    lettre[len-1] = '\0';
    strcpy(b_char->str, lettre);
    free(lettre);
    free(ligne);
    fclose(file);
    return b_char;
}

// ajoute le grand carractère stocké dans c à la fin de la chaine de grands carractères phrase
void concat(_char_t* phrase, _char_t* _char){ //finally working
    _char_t tmp = {
        .width = phrase -> width,
        .height = phrase -> height,
        .str = malloc(phrase -> len*sizeof(char)),
        .len = phrase -> len
    };
    if (tmp.len == 0){
        tmp.len = 1;
        tmp.str = realloc(tmp.str, tmp.len*sizeof(char));
    }else{
        for(int i = 0; i<tmp.len; i++){
            tmp.str[i] = phrase->str[i];
        }
    }
    
    char* p;

    phrase->width += _char->width;
    if (phrase -> str == NULL){
        phrase->height = _char->height;
        phrase -> len = _char -> len;
        phrase -> str = malloc(phrase -> len);
    }else{
        phrase -> len = phrase->len + _char->len - _char->height;
        // - height pour les '\n' inutiles, 1 à chaque ligne
        phrase -> str = realloc(phrase -> str, phrase -> len);
        //phrase->height += _char->height;
    }

    p = malloc(phrase -> len);

    int index_loop_1 = 0;
    int index_loop_2 = 0;

    int index = 0;

    for (int i = 0; i < phrase->height; i++){
        if (tmp.len != 0){
            while (index_loop_1 < tmp.len && tmp.str[index_loop_1] != '\n' && tmp.str[index_loop_1] != '\0'){
                p[index] = tmp.str[index_loop_1];
                index_loop_1++;
                index ++;
            }
            index_loop_1++;
        }
        while (_char->str[index_loop_2] != '\n' && _char->str[index_loop_2] != '\0'){
            p[index] = _char -> str[index_loop_2];
            index ++;
            index_loop_2++;
        }
        index_loop_2++;
        p[index] = '\n';
        index ++;
    }
    free(phrase->str);
    p[index-1] = '\0';
    phrase->str = p;
    free(tmp.str);
}

void print_bloc(_char_t G, int K){
    assert(K>0);
    if (K > G.width){
        K = G.width;
    }
    int n_loop = G.width/K;
    char* phrase = malloc(G.width*G.height*sizeof(char));
    int i2 = 0;
    for (int i = 0; i<G.len; i++){
        if (G.str[i] != '\n'){
            phrase[i2] = G.str[i];
            i2 ++;
        }else{
            phrase[i2] = '\0';
        }
    }
    
    for (int i= 0; i<n_loop; i++){
        for (int j = 0; j<G.len/G.width; j++){
            for (int k = 0; k<K; k++){
                printf("%c", phrase[k+j*G.width+i*K]);
            }
            printf("\n");
        }
    }
    if (G.width%K){
        for (int j = 0; j<G.len/G.width; j++){
            for (int k = 0; k<G.width%K; k++){
                printf("%c", phrase[k+j*G.width+(K-1+(n_loop-1)*K)]);
            }
            printf("\n");
        }
    }
    free(phrase);
}
int _main(int argc, char* argv[]){
    assert(argc == 3);
    _char_t phrase = {
        .width = 0,
        .height = 0,
        .str = NULL,
        .len = 0
    };
    
    int line_width = atoi(argv[2]);
    _char_t** liste_c = malloc(123*sizeof(_char_t*));
    for (int i = 0; i<123; i++){
        liste_c[i] = NULL;
    }

    printf("Rentrez la phrase à afficher: ");
    char c;
    scanf("%c", &c);
    while(c != '\n'){
        if (liste_c[c] == NULL){
            liste_c[c] = img_filename(c, char_width, argv[1][0]);
            assert(liste_c[c] != NULL);
        }
        concat(&phrase, liste_c[c]);
        scanf("%c", &c);
    }

    print_bloc(phrase, line_width);

    for (int i = 0; i<123; i++){
        if(liste_c[i]){
            free(liste_c[i]->str);
            free(liste_c[i]);
        }
    }
    free(liste_c);
    free(phrase.str);
    
    scanf("\n");
    return 0;
}
