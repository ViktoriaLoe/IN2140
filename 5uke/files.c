#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void)
{
    int fileDes;

    fileDes = open("Tedxt.txt", O_WRONLY);
    if (fileDes == -1)
    {
        perror("open");
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}