#include <stdio.h>
#include <fcntl.h> // for å åpne fil
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for å skrive til fil man 2 write

#define BUFFER_SIZE 100

// void replaceWord(FILE *fil, char *orig, char *new)
// {

// }
int main(void)
{
    FILE *fil, *fil2;
    //Opp 1.1 OPEN
    fil = fopen("Text.txt", "r");
    fil2 = fopen("Text2.txt", "w");
    if (fil == NULL || fil2 == NULL)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE];

    size_t readCount, writeCount;
    readCount = fread(buffer, sizeof(char), BUFFER_SIZE, fil);
    if (readCount <= 0){
        perror("fread");
        return EXIT_FAILURE;
    }

    printf("%s\n", buffer);
    //Opp 1.2 WRITE
    writeCount = fwrite(buffer, sizeof(char), strlen(buffer), fil2);
    if (writeCount < strlen(buffer))
    {
        perror("fwrite");
        return EXIT_FAILURE;
    }
    //replaceWord(fil2, "name", "Viktoria");
    char *name = "Van";
    int na;
 
    char *found = strstr(buffer, "name");
    if (found != NULL)
    {
        na = (found - buffer);
    } 
    memcpy(buffer+na, name, strlen(name));
    writeCount = fwrite(buffer, sizeof(char), strlen(buffer), fil2);
    

    printf("%s\n",  buffer);
    fclose(fil);
    fclose(fil2);
    return EXIT_SUCCESS;
}