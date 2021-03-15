#include <stdio.h>
#include <fcntl.h> // for å åpne fil
#include <unistd.h> //for å skrive til fil man 2 write
#include <string.h>
#include <stdlib.h>


typedef struct infoBlokk
{
    int id, flagg;
    char *modell; //produsent
    struct infoBlokk **pekere;//usikker her
    int antallPekere, visited, endringsnummer;
} infoBlokk;


//global liste over alle rutere
struct infoBlokk **rutere = NULL;
//globalt int om antall rutere
int numberOfRutere = 0;

void freeRute(int id)
{
    free(rutere[findIndexById(id)]);
}

void freeAllRuter()
{
    for (int i = 0; i < numberOfRutere;i++)
    {
        free(rutere[i]->modell);
        free(rutere[i]->pekere);
        free(rutere[i]);
    }
    free(rutere);
}

void printOneRute(int i)
{
    printf("ID: %d, Flagg: %d, Modell: %s\nKoblet med: \n", rutere[i]->id, rutere[i]->flagg, rutere[i]->modell);
    for (int j = 0;j < rutere[i]->antallPekere; j++)
    {
        printf( "    ID: %d \n", rutere[i]->pekere[j]->id);
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

void createInfoBlokk(int id, int flagg, unsigned char *modell, int index)
{

    infoBlokk *ruter = malloc(sizeof(struct infoBlokk));
    ruter->pekere  = malloc(sizeof(struct infoBlokk) * 10); 

    rutere[index] = ruter; 
    
    if (ruter == NULL)
    {
        fprintf(stderr, "mallloc failed, out of memory?\n");
        freeAllRuter();
        exit(EXIT_FAILURE);
    }
    
    ruter->modell = strdup((char *)modell); // les antall bytes og navn til char * i main
    ruter->antallPekere = 0;
    ruter->id = id; //gjør om fra bits til int i main
    ruter->flagg = flagg; // gjør om fra bits til int i main
    ruter->visited = 0;
    ruter->endringsnummer = 0; 
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

    //LESE FIL
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
        createInfoBlokk(ruteID, flagg, produsent, i); //er det noe galt med å sende flagg 

        fseek(fil, 1, SEEK_CUR);
    }

//LES KOBLINGER
    int R1;
    int R2;
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
                        break;
                        
                    }
                }
            }
        }
    }
    fclose(fil);
}

int findIndexById(int id)
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


int DFS(struct infoBlokk *ruter, int idLookingFor)
{
    ruter->visited = 1;
    for (int i = 0; i < ruter->antallPekere;i++) 
    {
        if(ruter->pekere[i]->id == idLookingFor)
        {
            printf("Det finnes en kobling\n");
            return 1;
        }
        if (ruter->pekere[i]->visited == 0)
        {
            DFS(ruter->pekere[i], idLookingFor);
        }
    }
}

int doCommand(char *command)
{
    int id, id2;
    char *nyModell;
    char verdi, nyFlagg;

    //Sorter innholdet i kommandlinjen
    struct infoBlokk *ruter1 = rutere[findIndexById(id)];

    if (strstr(command, "print")){
        printOneRute(findIndexById(id));
        return 0;
    }

    if (strstr(command, "modell")){
        rutere[findIndexById(id)]->modell = nyModell;
        return 0;
    }

    if (strstr(command, "flagg")){
        if (nyFlagg == '3') {printf("Ugyldig flaggverdi!\n");
            return 1;}
        if ((int)nyFlagg > 8) {printf("Ugyldig flaggverdi!");
            return 1;}
        else {
            rutere[findIndexById(id)]->flagg = nyFlagg+verdi;
        }
        return 0;
    }

    if (strstr(command, "finnes")){
        int result = DFS(findIndexById(id), id2);
        switch (result)
        {
            case 1 : printf("Det finnes en kobling mellom ruterne");
                break;
            default : printf("Det finnes ikke en kobling");
        }
    }

    if (strstr(command, "slett")){
        //traversere? full dfs
    }

    if(strstr(command, "kobling")){
        int n = ruter1->antallPekere;
        ruter1->pekere[n+1] = rutere[findIndexById(id2)];
        ruter1->antallPekere++;
        ruter1->endringsnummer++;
        return 0;
    }
    else {
        printf("Det er ikke en kommando");
        return 1;
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
        //freeAllRuter();
        exit(EXIT_FAILURE);
    }

    char *linje;
    size_t lengde = 0;
    ssize_t read;

    while ((read = getline(&linje, &lengde, fil)) != -1)
    {
        //les en og en linje og send hele til doCommand funksjon
        if (doCommand(linje) == 0) break;
    }

    fclose(fil);

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
    DFS(rutere[findIndexById(983)], 530);
    freeAllRuter();
    return 0;
}