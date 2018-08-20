//Justin Ma

#include <stdio.h>
#include <stdlib.h>

int frobcmp( char const *a, char const *b ) {
    int num = 0;
    char aa = *a;
    char bb = *b;
    while (aa != ' ' && bb != ' ') {
        if ((aa ^ 42) == (bb ^ 42)) {
            num++;
            aa = a[num];
            bb = b[num];
        }
        else if ((aa ^ 42) > (bb ^ 42)) { return 1; }
        else { return -1; }
    }
    if (aa == ' ') {
        if (bb == ' ') { return 0; }
        else { return 1; }
    }
    else { return -1; }
}

int format( void const *a, void const *b ) {//required for qsort
    return frobcmp(*(char const **)a, *(char const **)b);
}

void clearall( char** words, int count){
    for (int n = 0; n < count; n++) {
        free( words[n] );
    }
    free( words );
}

int main( void ) {
    char character;
    char **words = (char**)malloc(sizeof(char*));
    int len = 0;
    
    while (1) {
        int donzo = 0;
        char *letters = (char*)malloc(sizeof(char));
        int wordlen = 0;
        while(1){
            character = getchar();
            if (ferror(stdin)) {    //unable to read standard input
                clearall(words, len);
                free(letters);
                fprintf(stderr, "Unable to read standard input.");
                exit(1);
            }
            if (character == EOF) {
                donzo = 1;
                character = ' ';
            }
            if (character == ' ') { //add word
                if (wordlen == 0) { continue; }
                letters[wordlen] = ' ';
                words[len] = letters;
                len++;
                words = (char**)realloc(words, (len+2) * sizeof(char*));
                break;
            }
            else {                  //add letter
                letters[wordlen] = character;
                wordlen++;
                letters = (char*)realloc(letters, (wordlen+2) * sizeof(char));
            }
        }
        if (donzo == 1){
            break;
        }
    }
    
    qsort(words, len, sizeof(char*), format);
    
    for (int n = 0; n < len; n++){
        int m = 0;
        while (words[n][m] != ' '){
            putchar(words[n][m]);
            m++;
        }
        putchar(' ');
    }
    
    clearall(words, len);
    
    return 0;
}
