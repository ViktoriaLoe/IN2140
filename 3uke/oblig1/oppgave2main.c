#include <stdio.h>
#include "oppgave2a.h"


int main(void)
{
    // int test1 = strSum("abcd"); //dette blir 10
    // int test2 = strSum("abcd!"); // dette returnerer -1
    int avstand = distance_between("ha123456", 'a');
    char *test = string_between("a1234a", 'a');
    printf("%d, %s\n", avstand, test);
    return 0;
}