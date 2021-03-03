

int main(void) {
	char c = 'a';
	char *p = &c;
	*p += 2;

	printf("Som desimal: %d\n", c);
	printf("Som char %c\n", c);
	printf("Som peker %p\n", c); //dette gir en warning
 
	return 0;
}

