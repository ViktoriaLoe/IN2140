
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

// struct tall
// {
//     int tall;
// };

// void  constructTall(struct tall *n, unsigned int tall)
// {
//     n->tall = tall;
    
// }

// int main(void)
// {
//     struct tall *en = malloc(sizeof(struct tall));;
//     constructTall(en, 5);
//     printf("tall: %d\n", en->tall);
// }

typedef struct bil
{
    char *merke;
    char *plateSTED;
    int plateNR;

} bil ;

void fyllMedBiler(bil **biler)
{
    int cnt = 1000;
    for (int i = 0; i < 4; i++)
    {
        bil b; 
        b.plateNR = cnt+=1; //hvordan akkessere hver peker 
        *biler = &b;
        
        //printf("%d\n", b.plateNR);
        biler++; 
        
    }
}
int main(void)
{
    bil stian;
    stian.merke = "Honda";
    printf("%s\n", stian.merke); 

    bil **biler = malloc(sizeof(bil) * 4);
    fyllMedBiler(biler); // sender med adressen til 2x*

    while(*biler)
    {
        printf("%d\n", (**biler).plateNR);
        biler++;
    }
}