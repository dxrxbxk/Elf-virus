#include <stdio.h>


int main() {

	static int *i = NULL;

	printf("%p\n", i);

	return 0;
}

