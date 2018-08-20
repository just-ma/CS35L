//Justin Ma

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>


int fopt = 0;

char decide(char a){
    if (fopt == 0){
        return a;
    }
    else{
        return toupper((unsigned char) a);
    }
}

int frobcmp( char const *a, char const *b ) {
    int num = 0;
    char aa = *a;
    char bb = *b;
    while (aa != ' ' && bb != ' ') {
        if (decide(aa ^ 42) == decide(bb ^ 42)) {
            num++;
            aa = a[num];
            bb = b[num];
        }
        else if (decide(aa ^ 42) > decide(bb ^ 42)) { return 1; }
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

int main(int argc, const char * argv[]) {
    char character;
    char *letters = NULL;
    char *tletters = NULL;
    char **words = NULL;
    int len, length;
    int p = 0;
    ssize_t num;
    struct stat fileStat;
    len = length = 0;

    if (argc >= 2 && *argv[1] == '-' && *(argv[1]+1) == 'f'){
        fopt = 1;
    }
    
    if (fstat(0, &fileStat) < 0){
        fprintf(stderr, "Unable to read or write\n");
        free(letters);
        exit(1);
    }
    
    if (S_ISREG(fileStat.st_mode) == 1){
        length = (int)fileStat.st_size + 1;
    }
    else{
        length = 16;
    }
    
    letters = (char*)malloc(length*sizeof(char));
    
    while (1) {
        int wordlen = 0;
        int donzo = 0;
        while(donzo == 0){
            num = read(0, &character, 1);

            if (num == 0){
                donzo = 1;
                if (letters[wordlen-1] == ' '){
                    break;
                }
                character = ' ';
            }
            else if (num < 0) {    //unable to read standard input
                free(letters);
                fprintf(stderr, "Unable to read standard input.");
                exit(1);
            }
            
            if (wordlen == length){
                length = 2*length;
                tletters = (char*)realloc(letters, length*sizeof(char));
                letters = tletters;
            }
            
            if (character == ' ') { //add word
                if (wordlen == 0) { continue; } //skip duplicate spaces
                letters[wordlen] = ' ';
                len++;
                wordlen++;
            }
            else {                  //add letter
                letters[wordlen] = character;
                wordlen++;
            }
        }
        if (donzo == 1){
            break;
        }
    }
    
    tletters = letters;
    words = (char**)malloc(len*sizeof(char*));
    
    if (len > 0){
        words[0] = tletters;
    }
    for (int n = 1; n < len; n++){
        while (letters[p] != ' '){
            p++;
        }
        p++;
        tletters = letters + p;
        words[n] = tletters;

    }

    qsort(words, len, sizeof(char*), format);

    for (int n = 0; n < len; n++){
        int m = 0;
        while (words[n][m] != ' '){
            write(1, (&words[n][m]), 1);
            m++;
        }
        char space = ' ';
        write(1, &space, 1);
    }
    
    free(letters);
    free(words);
    return 0;
}
