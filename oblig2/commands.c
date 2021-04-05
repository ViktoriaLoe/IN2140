#include <stdio.h>
#include <fcntl.h>  
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include "header.h"

void unvisitAll()
{
    for (int i = 0; i < numberOfRutere; i++)
    {
        routers[i]->visited = 0;
    }
}

//DELETE router
void delete (int LookingFor)
{
    for (int i = 0; i < numberOfRutere; i++)
    {
        if (routers[i] == NULL)
            break;
        for (int j = 0; j < routers[i]->numerOfPointers; j++)
        {
            if (routers[i]->pointers[j] == NULL)
                break;
            if (routers[i]->pointers[j]->id == LookingFor)
            {
                routers[i]->pointers[j] = NULL;
                free(routers[i]->pointers[j]);
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
    for (int i = 0; i < router->numerOfPointers; i++)
    {
        if (router->pointers[i] != NULL)
        {
            if (router->pointers[i]->id == idLookingFor)
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

    struct Router *ruter1; 
    struct Router *ruter2;

    char *pch;
    pch = strtok(cmd, "  ");
    command = pch;
    printf("\nDoing command: %s\n", pch);

    cnt = 0;
    while (pch != NULL)
    {
        //lese hele modell navnet
        if (cnt == 1 && strstr(command, "model")){
            newModel =  malloc(sizeof(char)*249); 
            pch = strtok(NULL, "\n");
        }
        //lese ett og ett ord
        else    {
            pch = strtok(NULL, " ");
        }

        //hente id
        if (cnt == 0)
        {
            id = atoi(pch);
            ruter1 = routers[findIndexById(id)];
        }
        //hente flagg
        else if (cnt == 1 && strstr(command, "flag"))
        {
            newFlag = atoi(pch);
            printf("%d\n", newFlag);
            pch = strtok(NULL, " ");
            verdi = pch[0];
        }
        //hente modell
        else if (cnt == 1 && strstr(command, "model"))
        {
            strcpy(newModel, pch);
        }

        //hente id2
        else if (cnt == 1 && (strstr(command, "finnes") || strstr(command, "kobling")))
        {
            id2 = atoi(pch);
            ruter2 = routers[findIndexById(id2)];
            break;
        }
        cnt++;
    }

    //PRINT
    if (strstr(command, "print"))
    {
        printOneRute(findIndexById(id));
        return 0;
    }
    //MODELL
    else if (strstr(command, "model"))
    {
        free(ruter1->model);
        ruter1->model = newModel;
        printf("ny modelle er :%s\n", ruter1->model);
        return 0;
    }
    //FLAGG   Skjønner ikke helt
    else if (strstr(command, "flag"))
    { 
        if (newFlag == 3)
        {
            printf("Ugyldig flaggverdi!\n");
            return 1;
        }
        else if (newFlag > 7)
        {
            printf("Ugyldig flaggverdi!");
            return 1;
        }
        else
        {
            if (verdi == '0')
            {
                printf("flag %d, verdi %c %d\n", newFlag, verdi, ruter1->flag);
                printFlagBits(ruter1->flag);
                ruter1->flag = ruter1->flag & (~(1 << newFlag));
            }
            else
            { 
                printf("flag %c, verdi %c %d\n", newFlag, verdi, ruter1->flag);
                ruter1->flag |= 1 << newFlag;
            }
            printFlagBits(ruter1->flag);
        }
        return 0;
    }
    //FINNES
    else if (strstr(command, "finnes"))
    {
        int result = dFS(routers[findIndexById(id)], id2);
        unvisitAll();
        switch (result)
        {
        case 0:
            printf("Det finnes en kobling mellom ruterne\n");
            break;
        case 1:
            printf("Det finnes ikke en kobling\n");
            break;
        }
    }
    //DELETE
    else if (strstr(command, "slett"))
    {
        printf("forsøker å slette alle koblinger til id: %d\n", id);
        delete (id);
        numberOfRutere--;
    }
    //KOBLING
    else if (strstr(command, "kobling"))
    {
        ruter1->pointers[ruter1->numerOfPointers] = ruter2;
        printf("%d har nå kobling med %d\n", ruter1->id, ruter2->id);
        ruter1->numerOfPointers++;

        return 0;
    }
    else
    {
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
