#include <stdio.h>
#include <fcntl.h>  
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include "header.h"


void freeRute(int id)
{
    int index = findIndexById(id);
    if (routers[index] != NULL) {
        free(routers[index]->model);
        free(routers[index]->pointers);
        free(routers[index]);
    }
}

void freeAllRuter()
{
    for (int i = 0; i < numberOfRutere; i++)
    {
        if (routers[i] != NULL)
        {
            free(routers[i]->model);
            free(routers[i]->pointers);
            free(routers[i]);
        }
    }
    free(routers);
}

void printFlagg(struct Router *r)
{
    printf("...\n Er aktiv: ");
    if (r->flag & (1<<0)) {printf("Ja \n"); }
    else {printf("Nei\n");}

    printf("...\n Er trådløs: ");
    if (r->flag & (1<<1)) {printf("Ja \n"); }
    else {printf("Nei\n");}
    
    printf("...\n 5GHz: ");
    if (r->flag & (1<<2)) {printf("Ja \n"); }
    else {printf("Nei\n");}

    printf("...\n  Endringsnummer: ");
    printf("%d\n", r->flag  ); 
}

void printOneRute(int i)
{
    if (routers[i] == NULL)
    {
        return;
    }
    printf("-----------------------\n");
    printf("ID: %d, model: %s\nKoblet med: \n", routers[i]->id, routers[i]->model);
    printFlagg(routers[i]);
    for (int j = 0; j < routers[i]->numerOfPointers; j++)
    {
        if (routers[i]->numerOfPointers == 0)
        {
            printf("No connections\n");
            break;
        }
        printf("    ID: %d \n", routers[i]->pointers[j]->id);
    }
    printf("-----------------------\n");
}

void printAllInfo()
{
    for (int i = 0; i < numberOfRutere; i++)
    {
        printOneRute(i);
    }
}

void makeSpaceForInfo(int N)
{
    printf("%d\n", N);
    routers = malloc(sizeof(struct Router *) * N);
}

int findIndexById(int idL)
{
    for (int i = 0; i < numberOfRutere; i++)
    {
        if (routers[i]->id == idL)
        {
            return i;
        }
    }
    return 1;
}

void createRouter(int id, unsigned char flag, char *modelnavn, int index)
{

    Router *router = malloc(sizeof(struct Router));
    router->pointers = malloc(sizeof(struct Router *) * 10);

    routers[index] = router;

    if (router == NULL)
    {
        fprintf(stderr, "mallloc failed, out of memory?\n");
        freeAllRuter();
        exit(EXIT_FAILURE);
    }

    router->model = strdup(modelnavn);
    router->numerOfPointers = 0;
    router->id = id;
    router->flag = flag;
    router->visited = 0;
}

void checkResult(int res, FILE *f)
{
    if (res == 0 && ferror(f))
    {
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
        exit(EXIT_FAILURE);
    }

    int rc;
    int N;

    rc = fread(&N, sizeof(int), 1, fil);
    checkResult(rc, fil);
    makeSpaceForInfo(N);
    numberOfRutere = N;

    //LESE FIL
    for (int i = 0; i < N; i++)
    {
        unsigned int ruteID;      //4 etterfølgene bytes
        unsigned char flag;       //1 byte
        unsigned char lengdeChar; //hvor lang produsent char * er
        char produsent[249];

        rc = fread(&ruteID, sizeof(int), 1, fil);
        checkResult(rc, fil);

        rc = fread(&flag, sizeof(char), 1, fil);
        checkResult(rc, fil);

        rc = fread(&lengdeChar, sizeof(char), 1, fil);
        checkResult(rc, fil);

        rc = fread(&produsent, sizeof(char), sizeof(char) * lengdeChar, fil);
        produsent[lengdeChar] = 0;

        checkResult(rc, fil);
        createRouter(ruteID, flag, produsent, i); 

        fseek(fil, 1, SEEK_CUR);
    }

    //LES KOBLINGER
    int R1;
    int R2;
    while (fread(&R1, 4, 1, fil))
    {
        fread(&R2, 4, 1, fil);

        for (int i = 0; i < N; i++)
        {
            if (routers[i]->id == R1) //Finner R1
            {
                for (int j = 0; j < N; j++)
                {
                    if (routers[j]->id == R2) // Finner R2
                    {
                        int plass = routers[i]->numerOfPointers;
                        routers[i]->pointers[plass] = routers[j];
                        routers[i]->numerOfPointers++;
                        break;
                    }
                }
            }
        }
    }
    fclose(fil);
}

// WRITE TO FILE
int writeToFile()
{
    FILE *newFile;
    newFile = fopen("newTopology.dat", "wb");
    if (newFile == NULL)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fwrite(&numberOfRutere, sizeof(int), 1, newFile);

    for (int i = 0; i < numberOfRutere; i++)
    {
        if (routers[i] != NULL)
        {
            // char *produsent = routers[i]->model;
            fwrite(&routers[i]->id, sizeof(int), 1, newFile);
            fwrite(&routers[i]->flag, sizeof(char), 1, newFile);

            char lengdeChar = strlen(routers[i]->model); //hvor lang produsent char * er, usikker på denne
            fwrite(&lengdeChar, sizeof(char), 1, newFile);
            //fwrite(produsent, (sizeof(char)*lengdeChar +1), 1, newFile);
            fwrite(routers[i]->model, (sizeof(char) * lengdeChar + 1), 1, newFile);
        }
    }

    //skrive kobling info
    for (int i = 0; i < numberOfRutere; i++)
    {
        if (routers[i] == NULL)
            break;
        for (int j = 0; j < routers[i]->numerOfPointers; j++)
        {
            if (routers[i]->pointers[j] != NULL)
            {
                fwrite(&routers[i]->id, sizeof(int), 1, newFile);
                fwrite(&routers[i]->pointers[j]->id, sizeof(int), 1, newFile);
            }
        }
    }
    fclose(newFile);
    return EXIT_SUCCESS;
}

#define BUFFERSIZE 100
#define EXPECTED_C 2

//MAIN
int main(int argc, char *argv[])
{

    if (argc < EXPECTED_C)
    {
        printf("useage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *fil1 = argv[1];
    readRuterFile(fil1);

    char *fil2 = argv[2];
    readCommandsFile(fil2);
    writeToFile();
    freeAllRuter();
    return 0;
}
