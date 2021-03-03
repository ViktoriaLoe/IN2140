
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

void fyllV(int *arr)
{
    int c = 2;
    for (int i = 0; i < 9; i++)
    {
        *arr = c;
        // printf("%d\n", *arr);
        //array[i] = i * i;
        arr++; c+=2;
    }

    *arr = 0;
}


int main(void)
{
    //oppgave 1.2

    int *peker = malloc(sizeof(int) * 10);
    fyllV(peker);

    while(*peker)
    {
        printf("now: %d\n",*peker);
        peker++;
    }

    free(peker); //hvorfor gir dette en segmen fault?
    return 0;
    
}