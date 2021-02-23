
#include <stdio.h>
#include <fcntl.h> // for å åpne fil
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for å skrive til fil man 2 write
 
/* Alt som er kommet inn - alt som har gått ut
    Det som er igjen  = SVINN
    
    Lage stuct for hver av */

typedef struct vare
{
    int beholdning;
    int registrertInn;
    char *navn;
    struct vare *neste;
} vare;

struct vare *varer;

void lagVarer(char *navn, unsigned char antall)
{
    struct vare *v = malloc(sizeof(struct vare));
    v->navn = strdup(navn); //heap allokerer og kopierer en streng
}

void lesLog(char *filnavn)
{
    FILE *fil;
    char navn[200];
    unsigned char antall, navnLen;
    int readCount;

    fil = fopen(filnavn, "rb");
    if (fil == NULL)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    while (readCount = fread(&antall, sizeof(char), 1, fil));
    {
        readCount = fread(&navnLen, sizeof(char), 1, fil);
        readCount = fread(navn, sizeof(char), navnLen, fil);
        navn[navnLen] = 0;


    }
}

int main(void)
{
    

}