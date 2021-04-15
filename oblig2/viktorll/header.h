#ifndef HEADER_H_
#define HEADER_H_

struct Router **routers;
int numberOfRutere;

typedef struct Router
{
    int id;
    unsigned char flag;
    int endringsnummer;
    char *model;              
    struct Router **pointers; 
    int numerOfPointers, visited;
} Router;

void freeRute(int id);
void readCommandsFile(char *filen);
void printOneRute(int i);
int findIndexById(int idL);
int doCommand(char cmd[]);

#endif 
