#include <stdio.h>

//Oppgave 1.3

void flipcaps(char *str)
{
    int hex = 0x20; //00100000 / 32
    while(*str)
    {
        /* hvordan sjekker jeg en spesisifik bit?
         sjekk om den '6' bitten er 0 eller 1
         flip den bitten 
         LES OM MASKER*/

    }
}
// Oppgave 1.2
void tilSmaaBokstaver(char *str)
{
    while(*str)
    {
        *str = *str | 0b100000;
        str++;
    }
}

int main(void)
{
    /* Opppgave 1.1 : 

    Fjern den øveste biten i hver byte og printer strengen*/
    /* & - Operatoren gjør alt usannt 
    unatatt når a og b er sann*/

    printf("Oppgave 1.1\n");
    unsigned int a = 0x7f; // 127 eller 0b01111111 fungerer også
    unsigned char bokstaver[] = {195, 226, 242, 225, 161, 128};

    for (int i = 0; i <= 6; i++)
    {
        bokstaver[i] = bokstaver[i] & a; 
        printf("%c", bokstaver[i]);
    }
    printf("\n");


    // OPPGAVE 1.2
    printf("Oppgave 1.2\n");
    /* | - operatoren gjør alt usannt
     untatt når a eller b er sann (1) */

    char str[] = "HELloCBrA";
    tilSmaaBokstaver(&str);
    printf("Oppgave 1.2: %s\n", str);


    //Oppgave 1.3
    printf("Oppgave 1.3\n");
    /* ^ - operasjonen (xor) gjør alt som ikke det samme
    til sant, og alt som er likt til usannt(0)*/

    fliCaps(&str);
    return 0;
}