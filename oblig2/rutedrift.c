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
    struct infoBlokk *R2;
    struct infoBlokk **pekere; //usikker her
} infoBlokk;



enum type
{
    TRENGER, JEG, ENUM
};

//global liste over alle rutere
struct infoBlokk **rutere = NULL;


void freeRute(infoBlokk *rute)
{
    //free struct funksjon
}



void freeAllRuter()
{
    //free all structs funksjon
}



void printAllInfo()
{
    //print info funksjoRuR
}



void createInfoBlokk(int id, int flagg, char *modell, int index)
{

    infoBlokk *ruter = malloc(sizeof(struct infoBlokk));
    rutere[index] = &ruter;
    if (ruter == NULL)
    {
        fprintf(stderr, "mallloc failed, out of memory?\n");
        //free_info;
        exit(EXIT_FAILURE);
    }

    ruter->modell = strdup(modell); // les antall bytes og navn til char * i main
    //sjekke feil?

    ruter->id = id; //gjør om fra bits til int i main
    ruter->flagg = flagg; // gjør om fra bits til int i main

}





//funksjon for å legge til koblinger
    /* Gå gjennom alle rutere og lete etter R1 og R2? 
 
   Finnes det en bedre måte å gjøre det kanskje? */



#define BUFFERSIZE 100
#define EXPECTED_C 2 

void makeSpaceForInfo(int N)
{
    printf("%d\n", N);
    rutere = malloc(sizeof(struct infoBlokk) * N);
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

    int readCount;
    unsigned char offset;
    int N;

    readCount = fread(&offset, 1, 1, fil);
    checkResult(readCount, fil);
    N = (int)offset;
    //allocer N plasser og opprett globalt array
    makeSpaceForInfo(N);

    readCount = fseek(fil, (int)offset, SEEK_SET);
    //while(fread(&offset, 1, 1, fil))     
    //while(offset) //fseek? 
    for (int i = 0; i < N;i++)
    {
        unsigned char ruteID; //4 etterfølgene bytes
        char flagg; //1 byte
        char lengdeChar; //hvor lang produsent char * er, usikker på denne
        char *produsent;
        //lese alt i bytes og overføre til 

        //DET ER NOE FEIL I INNLESNING
        fread(&ruteID, sizeof(int), 1, fil);
        checkResult(readCount, fil);
        printf("%d ID\n", ruteID);

        fread(&flagg, 1, 1, fil);
        printf("%d flagg\n", flagg);

        fread(&lengdeChar, 1, 1, fil);
        printf("%d lengdeChar\n", (int)lengdeChar);
        
        //fread(&produsent, 1, (int)lengdeChar, fil);
        printf("produsent: %s\n", produsent);
        createInfoBlokk(ruteID, flagg, produsent, i);
    }

    if (ferror(fil))
    {
        fprintf(stderr, "fread failed\b");
        freeAllRuter();
        exit(EXIT_FAILURE);
    }

    fclose(fil);
}


//funksjon som leser fil2 
void readCommandsFile(char *fil)
{

}


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
    readCommandsFile(fil2);
    
    
    
    return 0;
}