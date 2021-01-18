#include <stdio.h>

int main(void){
	int i = 5;
	printf("This is a number %d\n",10);
	printf("Hello World!\n%d\n%#x\n",i, i);
	printf("%zu int size\n%zunsigned int\n",sizeof(int), sizeof(unsigned int));
	printf("%zu char", sizeof(char));
	return 0;
}
