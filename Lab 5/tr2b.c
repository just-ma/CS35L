#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    //Number of operands check
    if (argc != 3){
        fprintf(stderr, "Error: Requires 3 operands\n");
        exit (1);
    }
    
    const char * from = argv[1];
    const char * to = argv[2];
    
    int fromlen, tolen;
    
    //Duplicate check
    for (fromlen = 0; from[fromlen] != '\0'; fromlen++) {
        for (int n = fromlen + 1; from[n] != '\0'; n++){
            if (from[fromlen] == from[n]){
                fprintf(stderr, "Error: Duplicates in from operand\n");
                exit (1);
            }
        }
    }
    
    for (tolen = 0; to[tolen] != '\0'; tolen++) {}
    
    if (tolen != fromlen){
        fprintf(stderr, "Error: Operands not same size\n");
        exit (1);
    }
    
    char thischar;
    while (1){
        thischar = getchar();
        if (thischar == EOF){
            break;
        }
        for (int n = 0; n < fromlen; n++){
            if (thischar == from[n]){
                thischar = to[n];
                break;
            }
        }
        putchar((int)thischar);
    }
    
    return 0;
}
