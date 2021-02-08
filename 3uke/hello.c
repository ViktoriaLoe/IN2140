#include <stdio.h>

void greet(char *);

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("useage: %s\n", argv[0]);
        return 1;
    }

    printf("forste arg%s\n", argv[0]); 
    greet(argv[1]);
    return 0;
}
