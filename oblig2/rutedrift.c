#include <stdio.h>
#include <fcntl.h> // for å åpne fil
#include <unistd.h> //for å skrive til fil man 2 write
#include <string.h>
#include <stdlib.h>


typedef struct infoBlokk
{
    int id;
    unsigned char flagg;
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
    free(rutere[id]);
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
    if (rutere[i] == NULL) return;
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
        if (rutere[i] == NULL) break;
        printf("ID: %d, Flagg: %d, Modell: %s\nKoblet med: \n", rutere[i]->id, rutere[i]->flagg, rutere[i]->modell);
        for (int j = 0;j < rutere[i]->antallPekere; j++)
        {
            if (rutere[i]->pekere[j] == NULL) break;
                printf("    ID: %d \n", rutere[i]->pekere[j]->id);
        }
        printf("\n");
    }
}

void createInfoBlokk(int id, unsigned char flagg, unsigned char *modell, int index)
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
    ruter->flagg = flagg; 
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

int findIndexById(int idL)
{
    for (int i = 0; i < numberOfRutere;i++)
    {
        if (rutere[i]->id == idL)
        {
            return i;
        }
    }
    return 1;
}

void unvisitAll()
{
    for (int i = 0; i < numberOfRutere;i++)
    {
        rutere[i]->visited = 0;
    }
}

//FULL DFS
void Slett (int LookingFor)
{ 
    /*Forstår det sånn at den må kun frees en gang, fordi alt vil automatisk 
    peke på null etter det. */
    for (int i = 0; i < numberOfRutere; i++) {
        if (rutere[i] == NULL) break;
        for (int j = 0; j < rutere[i]->antallPekere;j++) 
        {
            if (rutere[i]->pekere[j] == NULL) break;
            if(rutere[i]->pekere[j]->id == LookingFor)
            {
                rutere[i]->antallPekere--;
            }

        }
    }
    printf("fjerner %d\n", findIndexById(LookingFor));
    free(rutere[findIndexById(LookingFor)]);
}
//DFS
int dFS(struct infoBlokk *ruter, int idLookingFor)
{
    ruter->visited = 1;
    for (int i = 0; i < ruter->antallPekere;i++) 
    {
        if (ruter->pekere[i] == NULL) break;
        if(ruter->pekere[i]->id == idLookingFor)
        {
            printf("Det finnes en kobling\n");
            return 0;
        }
        if (ruter->pekere[i]->visited == 0)
        {
            dFS(ruter->pekere[i], idLookingFor);
            
        }
    }
    
    return 1;
}

//EXECUTE COMMAND
int doCommand(char cmd[])
{
    char *command;
    int id, id2;
    int cnt;
    char *nyModell;
    char verdi, nyFlagg;

    char * pch;
    pch = strtok(cmd, "  ");
    command = pch;
    printf("\nDoing command: %s\n", pch);

    cnt = 0;
    while (pch != NULL)
    {
        pch = strtok(NULL," ,");
        printf("%d : %s\n", cnt, pch);

        if (cnt == 0) {id = atoi(pch);}
        else if (cnt == 1 && strstr(command, "flagg")){ 
            nyFlagg = pch[0];
            pch = strtok(NULL, " ");
            verdi = pch[0];
        }
        else if (cnt == 1 && strstr(command, "modell")) {
            nyModell = pch;
            pch = strtok(NULL, "  ");
            char * tmp = strdup(pch);
            strcat(nyModell, " ");
            strcat(nyModell, tmp);
            printf("modell %s %d\n", nyModell, cnt);
        }
        else if(cnt == 2 && strstr(command, "modell") && pch != NULL) {
            char * tmp = strdup(pch);
            strcat(nyModell, " ");
            strcat(nyModell, tmp);
            break;
        }

        else if (cnt == 1 && (strstr(command, "finnes") || strstr(command, "kobling"))) {
            id2 = atoi(pch); break;}

        cnt++;
    }
    
    struct infoBlokk *ruter1 = rutere[findIndexById(id)];
    struct infoBlokk *ruter2 = rutere[findIndexById(id2)];
    
    if (strstr(command, "print")){
        printOneRute(findIndexById(id));
        return 0;
    }

    else if (strstr(command, "modell")){ //ikke ferdig
        ruter1->modell = nyModell;//fix
        printf("ny modelle er :%s\n", ruter1->modell);
        return 0;
    }

    else if (strstr(command, "flagg")){ //ikke ferdig
        if (nyFlagg == '3') {printf("Ugyldig flaggverdi!\n");
            return 1;}
        else if ((int)nyFlagg < 7) {printf("Ugyldig flaggverdi!");
            return 1;}
        else {
            ruter1->flagg = ruter1->flagg & (1 << verdi);
            printf("ny flagg er : %x, %c\n", ruter1->flagg, verdi);
        }
        return 0;
    }

    else if (strstr(command, "finnes")){ // ikke ferdig
        int result = dFS(rutere[findIndexById(id)], id2);
        unvisitAll();
        switch (result)
        {
            case 0 : printf("Det finnes en kobling mellom ruterne");
                break;
            default : printf("Det finnes ikke en kobling\n");
        }
    }

    else if (strstr(command, "slett")){
        printf("forsøker å slette alle koblinger til id: %d\n", id);
        Slett(id);
        numberOfRutere--;
    }

    else if(strstr(command, "kobling")){
        printf("Sjekker alle koblinger.\n");
        int n = ruter1->antallPekere;
        ruter1->pekere[n+1] = ruter2;
        ruter1->antallPekere++;
        ruter1->endringsnummer++;
        return 1;
    }
    else {
        return 1;
    }
    return 0;
}


// READ COMMAND FILE
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

    char linje[100];

    while (fgets(linje, sizeof(linje), fil))
    {
        strtok(linje, "\n");
        doCommand(linje);
    }
    fclose(fil);

    printf("Done with commands\n");
}


// WRITE TO FILE
void writeToFile() //nesten ferdig
{
    FILE *newFile;
    newFile = fopen("newTopology.dat", "wb");

    fwrite (&numberOfRutere, sizeof(numberOfRutere), 1, newFile);

    for (int i = 0; i < numberOfRutere; i++)
    {
        if (rutere[i] == NULL) break;
        int ruteID = rutere[i]->id; //4 etterfølgene bytes
        unsigned char flagg = rutere[i]->id; //1 byte
        char lengdeChar = strlen(rutere[i]->modell); //hvor lang produsent char * er, usikker på denne
        char *produsent = rutere[i]->modell;

        printf("%d id: %s\n",i, produsent);
        
        fwrite (&ruteID, sizeof(ruteID), 1, newFile);
        fwrite (&flagg, sizeof(char), 1, newFile);
        fwrite (&lengdeChar, sizeof(char), 1, newFile);
        fwrite(produsent, sizeof(char)*lengdeChar, 1, newFile);
    }

    //skrive kobling info
    int R1;
    int R2;
    for (int i = 0; i < numberOfRutere; i++)
    {
        R1 = rutere[i]->id;
        for (int j = 0; j < rutere[i]->antallPekere;j++)
        {
            fwrite((const void*) &R1, sizeof(R1),1, newFile);
            R2 = rutere[i]->pekere[j]->id;
            fwrite((const void*) &R2, sizeof(R2),1, newFile);
        }
    }
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
    //char str[] = "modell 705 Space evo";
    //char str[] = "slett 705";
    //doCommand(str);
    //printOneRute(findIndexById(705));
    //writeToFile();

    return 0;
}


/* Igjen å gjøre:
- få kommando lesning til å fungere alltid
    - flagg med flere verdier
    - navn med mellomrom
- skrive data til å være helt riktig
- teorioppgaver*/