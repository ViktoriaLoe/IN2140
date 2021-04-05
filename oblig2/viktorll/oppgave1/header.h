#ifndef HEADER_H_
#define HEADER_H_

struct Router **routers;
int numberOfRutere;

typedef struct Router
{
    int id;
    unsigned char flag;
    char *model;              
    struct Router **pointers; 
    int numerOfPointers, visited;
} Router;

void freeRute(int id);
void freeAllRuter();
void readCommandsFile(char *filen);

void printOneRute(int i);
void printAllInfo();


void readRuterFile(char *filen);
void checkResult(int res, FILE *f);
void createRouter(int id, unsigned char flag, char *modelnavn, int index);
int findIndexById(int idL);
void makeSpaceForInfo(int N);
int writeToFile();

void unvisitAll();     
void delete (int LookingFor);
int dFS(struct Router *router, int idLookingFor);
void printFlagBits(unsigned char flag);
int doCommand(char cmd[]);

#endif 
