
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
        // struct tall mittTall;
        // mittTall.tall = 5;
//     printf("tall: %d\n", en->tall);
// }

typedef struct bil
{
    char *merke;
    char *plateSTED;
    int plateNR;

} bil ;

// void init_cars(struct bil arr[], int length) {
//     for (size_t i = 0; i < length; i++) {
//         arr[i].hjul = 4;
//         arr[i].kilometerstand = 0;
//         arr[i].merke = "Toyota";
//     }
// }

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

//EVENT
struct event {
    char *navn;
    int prioritet;
    char beskrivelse[20];
};

void print_event(struct event * e){
    printf("\n********EVENT********\n");
    printf("Navn:        %s\n", e->navn);
    printf("Prioritet:   %d\n", e->prioritet);
    printf("Beskrivelse: %s\n", e->beskrivelse);
    printf("*********************\n\n");
}

struct event *create_event(char *navn, int prioritet, char *desc) {
    struct event *e = malloc( sizeof(struct event) );
    e->navn = navn;
    e->prioritet = prioritet;
    int lengde = (( strlen(desc) >= 19 ) ? 19 : strlen(desc) + 1 );
    strncpy(e->beskrivelse, desc, lengde);
    e->beskrivelse[19] = '\0';
    return e;
}

int main() {
    struct event *cbra, *plenum;

    cbra = create_event("Cbra", 10, "Ukens hoydepunkt!");
    plenum = create_event("Plenum", 9, "Gjennomgang av viktige temaer");

    print_event(cbra); // -> "Navn: Cbra. Prioritet: 10. Beskrivelse: Ukens hoydepunkt!"
    print_event(plenum); // -> "Navn: Plenum. Prioritet: 9. Beskrivelse: Gjennomgang av vikt"

    free(cbra);
    free(plenum);
    return 0;
}