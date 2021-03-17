#include <stdio.h>
#include <fcntl.h> // for å åpne fil
#include <unistd.h> //for å skrive til fil man 2 write
#include <string.h>
#include <stdlib.h>


typedef struct Router
{
    int id;
    unsigned char flag;
    char *model; //produsent
    struct Router **pointers;//usikker her
    int numerOfPointers, visited;
} Router;


//global liste over alle routers
struct Router **routers = NULL;
//globalt int om antall routers
int numberOfRutere = 0;

void freeRute(int id)
{
    free(routers[id]);
}

void freeAllRuter()
{
    for (int i = 0; i < numberOfRutere;i++)
    {
        if (routers[i] != NULL) break;
        free(routers[i]->model);
        free(routers[i]->pointers);
        free(routers[i]);
    }
    free(routers);
}

void printOneRute(int i)
{
    if (routers[i] == NULL) return;
    printf("-----------------------\n");
    printf("ID: %d, flag: %d, model: %s\nKoblet med: \n", routers[i]->id, routers[i]->flag, routers[i]->model);
    for (int j = 0;j < routers[i]->numerOfPointers; j++)
    {
        if (routers[i]->numerOfPointers == 0) {
            printf("No connections\n");
            break;
        }
        printf( "    ID: %d \n", routers[i]->pointers[j]->id);
    }
    printf("-----------------------\n");

}

void printAllInfo()
{
    for (int i = 0; i <numberOfRutere; i++)
    {
        if (routers[i] == NULL) break;
        printf("ID: %d, flag: %d, model: %s\nKoblet med: \n", routers[i]->id, routers[i]->flag, routers[i]->model);
        for (int j = 0;j < routers[i]->numerOfPointers; j++)
        {
            if (routers[i]->pointers[j] == NULL) break;
                printf("    ID: %d \n", routers[i]->pointers[j]->id);
        }
        printf("\n");
    }
}

void makeSpaceForInfo(int N)
{
    printf("%d\n", N);
    routers = malloc(sizeof(struct Router*) * N);
    //alloker nok plass til all structs 
    //og opprett et array med alle sammen som er globalt
}

int findIndexById(int idL)
{
    for (int i = 0; i < numberOfRutere;i++)
    {
        if (routers[i]->id == idL)
        {
            return i;
        }
    }
    return 1;
}

void createRouter(int id, unsigned char flag,  char *modelnavn, int index)
{

    Router *router = malloc(sizeof(struct Router));
    router->pointers  = malloc(sizeof(struct Router) * 10); 

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
        unsigned char flag; //1 byte
        unsigned char lengdeChar; //hvor lang produsent char * er, usikker på denne
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
        createRouter(ruteID, flag, produsent, i); //er det noe galt med å sende flag 

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

void unvisitAll()
{
    for (int i = 0; i < numberOfRutere;i++)
    {
        routers[i]->visited = 0;
    }
}

//DELETE router
void delete (int LookingFor)
{ 
    /*Forstår det sånn at den må kun frees en gang, fordi alt vil automatisk 
    peke på null etter det. */
    for (int i = 0; i < numberOfRutere; i++) {
        if (routers[i] == NULL) break;
        for (int j = 0; j < routers[i]->numerOfPointers;j++) 
        {
            if (routers[i]->pointers[j] == NULL) break;
            if(routers[i]->pointers[j]->id == LookingFor)
            {
                routers[i]->numerOfPointers--;
            }

        }
    }
    freeRute(LookingFor);
}
//DFS
int dFS(struct Router *router, int idLookingFor)
{
    router->visited = 1;
    for (int i = 0; i < router->numerOfPointers;i++) 
    {
        if (router->pointers[i] != NULL) 
        {
            if(router->pointers[i]->id == idLookingFor)
            {
                return 0;
            }
            else if (router->pointers[i]->visited == 0)
            {
                dFS(router->pointers[i], idLookingFor);
        
            }
        }
    }
    
    return 1;
}
void printFlagBits(unsigned char flag)
{
        int number = flag;
        for (int i = 8 - 1; i >= 0; i--)
        {
                int shifted_number = number >> i;

                if (shifted_number & 1)
                        printf("1");
                else
                        printf("0");
        }

        printf("\n");
}


//EXECUTE COMMAND
int doCommand(char cmd[])
{
    char *command;
    int id, id2;
    int cnt;
    int newFlag;
    char *newModel;
    unsigned char verdi;
   

    char * pch;
    pch = strtok(cmd, "  ");
    command = pch;
    printf("\nDoing command: %s\n", pch);

    cnt = 0;
    while (pch != NULL)
    {
        pch = strtok(NULL," ,");
        //printf("%d : %s\n", cnt, pch);

        if (cnt == 0) {id = atoi(pch);}
        else if (cnt == 1 && strstr(command, "flag")){ 
            newFlag = atoi(pch);
            printf("%d\n", newFlag);
            pch = strtok(NULL, " ");
            verdi = pch[0];
        }
        else if (cnt == 1 && strstr(command, "model")) {
            newModel = pch;
            pch = strtok(NULL, "  ");
            char * tmp = strdup(pch);
            strcat(newModel, " ");
            strcat(newModel, tmp);
            printf("model %s %d\n", newModel, cnt);
            free(tmp);
        }
        else if(cnt == 2 && strstr(command, "model") && pch != NULL) {
            char * tmp = strdup(pch);
            strcat(newModel, " ");
            strcat(newModel, tmp);
            free(tmp);
            break;
        }

        else if (cnt == 1 && (strstr(command, "finnes") || strstr(command, "kobling"))) {
            id2 = atoi(pch); break;}

        cnt++;
    }
    
    struct Router *ruter1 = routers[findIndexById(id)];
    struct Router *ruter2 = routers[findIndexById(id2)];
    //PRINT
    if (strstr(command, "print")){
        printOneRute(findIndexById(id));
        return 0;
    }
    //MODELL
    else if (strstr(command, "model")){ 
        ruter1->model = newModel;
        printf("ny modelle er :%s\n", ruter1->model);
        return 0;
    }
    //FLAGG
    else if (strstr(command, "flag")){ //ikke ferdig
        if (newFlag == 3) {printf("Ugyldig flaggverdi!\n");
            return 1;}
        else if (newFlag > 7) {printf("Ugyldig flaggverdi!");
            return 1;}
        else 
        {
            if (verdi == '0') { // biten skal bli 0
                printf("flag %d, verdi %c %d\n", newFlag, verdi, ruter1->flag);
                printFlagBits(ruter1->flag);
                ruter1->flag = ruter1->flag & (~(1 << newFlag)); 
            }
            else { // bitten skal bli 1
                printf("flag %c, verdi %c %d\n", newFlag, verdi, ruter1->flag);
                ruter1->flag |= 1 << newFlag;

            }
           printFlagBits(ruter1->flag); 
        }
        return 0;
    }
    //FINNES
    else if (strstr(command, "finnes")){ 
        int result = dFS(routers[findIndexById(id)], id2);
        unvisitAll();
        switch (result)
        {
            case 0 : printf("Det finnes en kobling mellom ruterne\n");
                break;
            case 1 : printf("Det finnes ikke en kobling\n");
                break;
        }
    }
    //DELETE
    else if (strstr(command, "delete")){
        printf("forsøker å slette alle koblinger til id: %d\n", id);
        delete(id);
        numberOfRutere--;
    }
    //KOBLING
    else if(strstr(command, "kobling")){
        ruter1->pointers[ruter1->numerOfPointers+1] = ruter2;
        printf("%d har nå kobling med %d\n", ruter1->id, ruter2->id);
        ruter1->numerOfPointers++;
        
        return 0;
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
int writeToFile() //nesten ferdig
{
    FILE *newFile;
    newFile = fopen("newTopology.dat", "wb");
    if (newFile == NULL)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fwrite (&numberOfRutere, sizeof(int), 1, newFile);

    for (int i = 0; i < numberOfRutere; i++)
    {
        if (routers[i] != NULL)
        {
            char *produsent = routers[i]->model;
            fwrite (&routers[i]->id, sizeof(int), 1, newFile);
            fwrite (&routers[i]->flag, sizeof(char), 1, newFile);
            
            char lengdeChar = strlen(routers[i]->model); //hvor lang produsent char * er, usikker på denne
            fwrite (&lengdeChar, sizeof(char), 1, newFile);
            //fwrite(produsent, (sizeof(char)*lengdeChar +1), 1, newFile);
            fwrite(routers[i]->model, (sizeof(char)*lengdeChar + 1), 1, newFile);
        }
    }

    //skrive kobling info
    for (int i = 0; i < numberOfRutere; i++)
    {
        if (routers[i] == NULL) break;
        for (int j = 0; j < routers[i]->numerOfPointers;j++)
        {
            if (routers[i]->pointers[j] == NULL) break;
            fwrite(&routers[i]->id, sizeof(int),1, newFile);
            fwrite(&routers[i]->pointers[j]->id, sizeof(int),1, newFile);
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
    //char str[] = "model 705 Space evo";
    //char str[] = "delete 705";
    //doCommand(str);
    //printOneRute(findIndexById(705));
    //writeToFile();
    freeAllRuter();
    return 0;
}


/* Igjen å gjøre:
- flag med flere verdier
- skrive data til å være helt riktig
- teorioppgaver*/