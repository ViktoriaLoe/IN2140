#include <stdio.h>

/*
 * Printer tall på binaerform
 *
 * void *n  - Adressen til tall-variabelen som skal printes
 * int size - Størrelsen til tall-variabelen
 */
void printbits(void *n, int size) {
    char *num = (char *)n;
    int i, j;

    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            printf("%c", (num[i] & (1 << j)) ? '1' : '0');
        }
        printf(" ");
    }
    printf("\n");
}

int main(void) {
    char c = 1 << 7;
    short s = c;
    int i = s;

    printbits(&c, sizeof(char));
    printbits(&s, sizeof(short));
    printbits(&i, sizeof(int));
    return 0;
}