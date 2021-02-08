#include <stdio.h>
#define EXPECTED_C 2

int main(int argc, char *argv[])
{
    if (argc != EXPECTED_C)
    {
        printf("useage: %s\n", argv[0]);
        return 1;
    }

    //oppgvae 2.1
    //printf("%s %s %s\n", argv[1], argv[2], argv[3]);

    //oppgave 2.2
    char sym = *argv[1] + 1;
    printf("%c\n", sym);

}