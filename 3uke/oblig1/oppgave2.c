#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "oppgave2a.h"

//oppgave 2 d)
char *string_between(char *s, char c)
{
    char *nyStreng = malloc(strlen(s) + 1); 
    char *peker = nyStreng; 

    int antForkomster = 0; 
    while(*s)
    {
        if (*s == c)
        {
            antForkomster++;
            if (antForkomster == 2)
            {
                return nyStreng;
            }
        }

        if (antForkomster == 1 && *s != c)
        {
            *(peker) = *s;
            peker++;
        }

        s++;
    }  

    if (antForkomster < 2) //hvis det ikke var nok forekomster
    {
        return NULL;
    }
}


//oppgave 2 c)
int distance_between(char *s, char c) //avstanden av 1. & 2. forkomst av c i s
{
    int antForekomster = 0; int avstand = 0;
    while(*s)
    {
        if (antForekomster == 1) // hvis vi har funnet første
        {
            avstand++;
        }

        if (*s == c) // hvis vi har nådd riktig char
        {
            antForekomster++;
            if (antForekomster == 2) //hvis det er andre forkomst
            {
                return avstand;
            }
        }

        s++;
    }

    if (antForekomster < 2) //hvis det ikke var nok forekomster
    {
        return -1;
    }
}

//oppgave 2 b
int strSum(char *s)
{
    int totalt = 0;
    int i = 0;

    while (*s) 
    {
        int verdi = tolower(*s)-96;
        if(verdi < 1 || verdi > 27)
        {
            printf("'%s', Is not a valid character\n ", s);
            return -1;
        }

        totalt+=verdi;
        s++;

    }
    printf("Total verdi : %d\n", totalt);
    return 0;
}