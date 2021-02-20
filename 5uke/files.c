#include <stdio.h>
#include <fcntl.h> // for å åpne fil
#include <stdlib.h>
#include <unistd.h> //for å skrive til fil man 2 write

int main(void)
{
    int fileDes, writeCount;


    //OPEN
    fileDes = open("Text.txt", O_WRONLY | O_TRUNC  /*| O_RDONLY | O_CREAT*/);   //writeonly | overskrive | lage ny fil
    if (fileDes == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }
    //WRITE
    writeCount = write(fileDes, "I love you", 8);
    if (writeCount == -1 )
    {
        perror("write");
        close(fileDes);
        return EXIT_FAILURE;
    }

    //READ
    //readCount() se cbra uke5


    //OPEN FILE IN BUFFERED MODE            man 3 fopen
    FILE *fil;
    char *msg = "Dette er buffered write";
    size_t fwriteCount;

    fil = fopen("ny_fil.txt", "w");
    if (fil == NULL)
    {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fwriteCount = fwrite(msg, sizeof(char), strlen(msg), fil);
    if (fwriteCount < strlen(msg))
    {
        perror("fwrite");
        return EXIT_FAILURE;
    }


    fclose(fil);
    close(fileDes);
    return EXIT_SUCCESS;
}