#include <stdlib.h>
#include <stdio.h>

int main()
{
	void *p;
	char *c;

	for (int i = 0;; ++i) {
		p = malloc(409600000);
		printf("%d %p\n", i, p);
		c = (char *)p;
		c = 'a';
	}

	for (;;) {
	}
}
