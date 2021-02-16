#include <stdio.h>
#include <stdlib.h>
#define EXPECTED_C 4 

int main(int argc, char *argv[])
{
    if (argc != EXPECTED_C)
    {
        printf("useage: %s\n", argv[0]);
        return 1;
    }

    char *sentence = argv[1];
    char oldC = *argv[2];
    char newC = *argv[3];

    while(*sentence)
    {
        printf("%c", (*sentence == oldC) ? newC : *sentence);
       sentence++;
    }
   printf("\n"); 

}



