
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

 enum type //konstanter;
 {
     BEHOLDNING, LEVERING, SALG
 };

struct vare *varer;

void lagVarer(char *navn, unsigned char antall, enum type t)
{
    struct vare *v = malloc(sizeof(struct vare));
     //init
    v->navn = strdup(navn); //heap allokerer og kopierer en streng
    v->beholdning = 0;
    v->registrertInn = 0;
    switch(t)
    {
        case BEHOLDNING:
            v->beholdning = antall;
            break;

        case LEVERING:
            v-> registrertInn = antall;
            break;

        case SALG:
            v-> registrertInn = -antall;
            break;
    }
}

void lesLog(char *filnavn, enum type t)
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