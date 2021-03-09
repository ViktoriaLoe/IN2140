#include <stdio.h>
/* oppgaver om signed vs. unsigned

Finn først ut av forskjellen
 two's complement has the advantage that the 
 fundamental arithmetic operations of addition, subtraction, and 
 multiplication are identical to those for unsigned binary numbers 
Så start på oppgavene (2.1-2.3)*/

int main(void)
{
    char a = 0xff; // blir -1 når printet som int
    char c = 255; // hvorfor blir char verdien -1 som en int
    unsigned char b = 0xff;// 255 i decimal
    int aSum = a + a;
    int bSum = b + b;
    int kombiSum = a + b;

    printf("a = %d %d,b = %o %d, %d\n", c, aSum, b, bSum, kombiSum);
    return 0;
}