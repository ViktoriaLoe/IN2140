
#include <stdio.h>

int main(void) {
	
	char streng[] = "hei";

	char arr[4]; // Kun nyttig hvis du skal 
	//gjøre noe med et tomt array som ikke kan gjøres i en linje
	arr[0] = 'h';
	arr[1] = 'e';

	char streng2[] = "hei og haa";

	streng[5] = 0; 
	printf("%s\n", streng);	/* vil printe 'hei o' 
	fordi den vil tro strengen er over når den når 0
	Det er forskjell på 0 og '0'*/
	

	return 0;
}
