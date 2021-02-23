#include <stdio.h>
#include <fcntl.h> // for å åpne fil
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for å skrive til fil man 2 write


typedef struct person 
{
    int alder;
    char *navn;
    struct person *next;
} person;

void printList(struct person *liste)
{
    while (liste)
    {
        printf("%s er %d år\n", liste->navn, liste->alder);
        liste = liste->next;
    }

    // ANNEN MÅTE Å ITERERE
    // person *tmp;
    // for(tmp = liste; tmp != NULL; tmp = tmp->next)
    // {
    //     printf("\n");
    // }
}

int main (void)
{
    struct person p;
    p.alder = 39;
    p.navn = "Ola";

    struct person *p2 = malloc (sizeof(struct person));
    if (p2 == NULL )
    {
        fprintf(stderr, "Mallox failed\n");
        return EXIT_FAILURE;
    }

    p2->alder = 12;
    p2->navn = "Viktoria";

    p.next = p2; //går fordi p2 er en peker
    p2->next = NULL; //siden p2 er en peker må man akkessere den 

    printList(&p); //vi vil aksessere selve pekeren så vi sender inn adressen til pekeren


    free(p2);
    return EXIT_SUCCESS;
}