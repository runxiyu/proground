#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int *e;
	e = mmap(NULL, 4096, PROT_READ | PROT_WRITE,
		 MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (e == MAP_FAILED) {
		perror("mmap");
		return 1;
	}

	write(1, e, 4096);
	munmap(e, 4096);
	e = malloc(4096);
	if (e == NULL) {
		perror("malloc");
		return 2;
	}
	write(1, e, 4096);
	return 0;
}
