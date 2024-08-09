#include <stdlib.h>
#include <stdio.h>

int main() {
	void *p = malloc(4096);
	free(p);
	free(p);
}
