#include <stdio.h>

void rc4(const char *plain, const char *rc4key, char *output);

int main() {
	char *plain = "plain";
	char *rc4key = "plain";
	char output[256];
	rc4(plain, rc4key, output);
	puts(output);
	return 0;
}
