#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int stringsum(char *s);

void stringsum2(char *s, int *res)
{
    *res = stringsum(s); 
}




//oppgave 2 d)
char *string_between(char *s, char c)
{
    char *nyStreng = malloc(strlen(s)); 
    char *peker = nyStreng; 

    int antForkomster = 0;  
    while(*s)
    {
        if (*s == c)
        {
            antForkomster++;
            if (antForkomster == 2)
            {
                *(peker) = '\0';
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
        free(nyStreng);
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
int stringsum(char *s)
{
    int totalt = 0;
    int i = 0;

    while (*s) 
    {
        int verdi = tolower(*s)-96;
        if(verdi < 1 || verdi > 27)
        {
            return -1;
        }

        totalt+=verdi;
        s++;

    }
    return totalt;
}