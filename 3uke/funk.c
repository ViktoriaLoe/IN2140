#include <stdio.h>
#include <string.h>

// Checks size of String oppgave 1.1 
int len(char *str)
{
    int size = 0;
    while(*str++)
    {
        size++;
    }
    return size;
}

char * cpyArray(char *firstA, char *secondA)
{
    while(*firstA++)
    {
        *secondA = *firstA;
        printf("%c", *secondA);
        secondA++;
    }
    return secondA;
}

int main(void)
{
    char *str = "setning";
    int size = len(str);
    int trueSize = strlen(str);
    //printf("%d\n%d\n", size, trueSize);

    char *first = "Denne oppgaven er litt rar synes jeg";
    char second[] = "Ma dette arrayet være mindre da eller motsatt bitchh ";
    char *newArray = cpyArray(first, second);

    printf("%s\n", newArray);
}

