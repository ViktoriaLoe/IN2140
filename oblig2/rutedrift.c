#include <stdio.h>
#include <fcntl.h> // for å åpne fil
#include <unistd.h> //for å skrive til fil man 2 write
#include <string.h>
#include <stdlib.h>


typedef struct infoBlokk
{
    int id;
    int flagg;
    char *modell; //produsent
    struct infoBlokk **pekere;//usikker her
    int antallPekere;
} infoBlokk;


//global liste over alle rutere
struct infoBlokk **rutere = NULL;
//globalt int om antall rutere
int numberOfRutere = 0;

void freeRute(int id)
{
    free(rutere[findIndex(id)]);
}

void freeAllRuter()
{
    for (int i = 0; i < numberOfRutere;i++)
    {
        free(rutere[i]);
        free(rutere[i]->modell);
        free(rutere[i]->pekere);
    }
    free(rutere);
}

void printOneRute(int i)
{
    printf("ID: %d, Flagg: %d, Modell: %s\nKoblet med: \n", rutere[i]->id, rutere[i]->flagg, rutere[i]->modell);
    for (int j = 0;j < rutere[i]->antallPekere; j++)
    {
        printf(stdout, "    ID: %d \n", rutere[i]->pekere[j]->id);
    }
    printf("\n");

}

void printAllInfo()
{
    for (int i = 0; i <numberOfRutere; i++)
    {
        printf("ID: %d, Flagg: %d, Modell: %s\nKoblet med: \n", rutere[i]->id, rutere[i]->flagg, rutere[i]->modell);
        for (int j = 0;j < rutere[i]->antallPekere; j++)
        {
                printf("    ID: %d \n", rutere[i]->pekere[j]->id);
        }
        printf("\n");
    }
}

void createInfoBlokk(int id, int flagg, char *modell, int index)
{

    infoBlokk *ruter = malloc(sizeof(struct infoBlokk));
    ruter->pekere  = malloc(sizeof(struct infoBlokk) * 10); 

    rutere[index] = ruter; 
    
    if (ruter == NULL)
    {
        fprintf(stderr, "mallloc failed, out of memory?\n");
        //free_info;
        exit(EXIT_FAILURE);
    }

    ruter->modell = strdup(modell); // les antall bytes og navn til char * i main
    ruter->antallPekere = 0;
    ruter->id = id; //gjør om fra bits til int i main
    ruter->flagg = flagg; // gjør om fra bits til int i main
    //printf("id: %d, flagg : %d, produsent : %s\n", rutere[index]->id, rutere[index]->flagg, modell);
    
}

void makeSpaceForInfo(int N)
{
    printf("%d\n", N);
    rutere = malloc(sizeof(struct infoBlokk*) * N);
    //alloker nok plass til all structs 
    //og opprett et array med alle sammen som er globalt
}

void checkResult(int res, FILE *f) {
    if (res == 0 && ferror(f)) {
        fprintf(stderr, "Error performing read\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }
}

//funksjon som leser fil1
void readRuterFile(char *filen)
{
    //FOPEN
    FILE *fil = fopen(filen, "rb");

    if (fil == NULL)
    {
        perror("fopen");
        freeAllRuter();
        exit(EXIT_FAILURE);
    }

    int rc;
    unsigned char offset;
    int N;


    rc = fread(&N, sizeof(int), 1, fil);
    checkResult(rc, fil);
    makeSpaceForInfo(N);
    numberOfRutere = N;

    for (int i = 0; i < N; i++)
    {
        unsigned int ruteID; //4 etterfølgene bytes
        unsigned char flagg; //1 byte
        unsigned char lengdeChar; //hvor lang produsent char * er, usikker på denne
        unsigned char produsent[249];

        rc = fread(&ruteID, sizeof(int), 1, fil);
        checkResult(rc, fil);

        rc = fread(&flagg, sizeof(char), 1, fil);
        checkResult(rc, fil);

        rc = fread(&lengdeChar, sizeof(char), 1, fil);
        checkResult(rc, fil);

        rc = fread(&produsent, sizeof(char), sizeof(char) * lengdeChar, fil);
        produsent[lengdeChar] = 0;

        checkResult(rc, fil);
        createInfoBlokk(ruteID, flagg, produsent, i);

        fseek(fil, 1, SEEK_CUR);
    }

//LES KOBLINGER
    unsigned int R1;
    unsigned int R2;
    while(fread(&R1, 4, 1, fil))
    {
        fread(&R2, 4, 1, fil);

        for (int i = 0; i < N; i++)
        {
            if (rutere[i]->id == R1) //Finner R1
            {
                for (int j = 0; j < N; j++)
                {
                    if (rutere[j]->id == R2) // Finner R2
                    {
                        int plass = rutere[i]->antallPekere;
                        rutere[i]->pekere[plass] = rutere[j];
                        rutere[i]->antallPekere++;
                        printf("id: %d, peker nå på id: %d\n", rutere[i]->id, rutere[j]->id );
                        break;
                        
                    }
                }
            }
        }
    }
    fclose(fil);
}

int findIndex(int id)
{
    for (int i = 0; i < numberOfRutere;i++)
    {
        if (rutere[i]->id == id)
        {
            return i;
        }
    }
    return numberOfRutere+1;
}

void doCommand(char *command, int id, int id2, char *nyModell, char verdi, char nyFlagg)
{
    if (strstr(command, "print")){
        printOneRute(findIndex(id));
    }
    if (strstr(command, "modell")){
        rutere[findIndex(id)]->modell = nyModell;
    }
    if (strstr(command, "flagg")){
        if (nyFlagg == '3') {printf("Ugyldig flaggverdi!\n");}
        if ((int)nyFlagg > 8) {printf("Ugyldig flaggverdi!");}
        else {
            rutere[findIndex(id)]->flagg = nyFlagg+verdi;
        }
    }
    if (strstr(command, "finnes")){
        //DFS
    }
    if (strstr(command, "slett")){
        //traversere?
    }
    if(strstr(command, "kobling")){
        int n = rutere[findIndex(id)]->antallPekere;
        rutere[findIndex(id)]->pekere[n+1] = rutere[findIndex(id2)];
    } else {
        printf("Det er ikke en kommando");
    }

}

//funksjon som leser fil2 
void readCommandsFile(char *filen)
{
    //FOPEN
    FILE *fil = fopen(filen, "r");

    if (fil == NULL)
    {
        perror("fopen");
        freeAllRuter();
        exit(EXIT_FAILURE);
    }

    char *linje;
    size_t lengde = 0;
    //int readCount;
}


#define BUFFERSIZE 100
#define EXPECTED_C 2 

int main(int argc, char *argv[])
{

    if (argc < EXPECTED_C)
    {
        printf("useage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *fil1 = argv[1];
    char *fil2 = argv[2];
    //process bits in file
    readRuterFile(fil1);
    //readCommandsFile(fil2);
    printAllInfo();
    
    freeAllRuter();
    return 0;
}