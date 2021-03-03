#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

struct person //er som et array bare av flere datatyper
{
    int alder;
    char *navn;
    
};

void insertStruct(struct person *p, int alder, char *nanv)
{
    p -> alder = alder; //følg peker og gå til alder og sett den til alder (dereferencing)
    (*p).navn = strdup(nanv);


}
int main(void)
{
    struct person *ole = malloc(sizeof(struct person));
    insertStruct(ole, 23, "ole"); //sender med adressen til structen
    printf("navn %s, alder %d\n", ole->navn, ole->alder);
    free(ole);
    return 0;
}