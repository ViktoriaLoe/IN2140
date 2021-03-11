#include <stdio.h>
#include <stdlib.h>

/*
    Dette er et løsningsforslag på bonus-nøtt oppgaven fra Cbra uke 5.
    Prøv først på oppgaven selv, og se evt. på dette forslaget dersom du
    sitter fast :)
    Husk at det er mange måter å løse en slik oppgave på, dette er bare ett
    alternativ. F.eks kan man også lese alt inn i et stort nok buffer, og bruke
    offset som indeks i bufferet.
    Oppgaven finnes her:
    https://github.uio.no/IN2140v2/in2140-v21/tree/master/cbra/uke05-filer#bonus-n%C3%B8tt
*/

void check_result(int res, FILE *f) {
    if (res == 0 && ferror(f)) {
        fprintf(stderr, "Error performing read\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    unsigned char curr, offset;
    int rc;
    FILE *file;

    /*
    Open the file in "read bytes" mode. Also check the return value
    of fopen().
    */
    file = fopen("secret", "rb");
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    /*
    Read one byte into the offset variable from the top of the file
    NOTE: a char is only one byte long, so byte order is irrelevant here
    */
    rc = fread(&offset, 1, 1, file);
    check_result(rc, file);
    while (offset) {
        /*
        Seek to the offset in the file. SEEK_SET is a constant representing
        the top of the file, so the command is "go to offset bytes into the
        file, from SEEK_SET". See "man fseek" for more information.
        */
        rc = fseek(file, offset, SEEK_SET);
        if (rc == -1) {
            perror("fseek");
            fclose(file);
            return EXIT_FAILURE;
        }

        /*
        Read one character at the time from the file into the curr variable,
        and print it. Stop printing when a terminating null-byte is reached.
        */
        fread(&curr, 1, 1, file);
        check_result(rc, file);
        while (curr) {
            printf("%c", curr);
            fread(&curr, 1, 1, file);
            check_result(rc, file);
        }
        printf(" ");

        /*
        The offset of the next word in the sentence is located after the
        null-byte. If the offset is zero, the last word has been read.
        */
        fread(&offset, 1, 1, file);
        check_result(rc, file);
    }

    /*
    Always close the file when you are done ;)
    */
    fclose(file);
    printf("\n");
    return EXIT_SUCCESS;
}