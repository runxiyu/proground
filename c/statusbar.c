#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main()
{
	time_t t;
	struct tm td;
	for (;;) {
		t = time(NULL);
		td = *localtime(&t);
		printf("%d-%02d-%02d %02d:%02d:%02d\n", td.tm_year + 1900,
		       td.tm_mon + 1, td.tm_mday, td.tm_hour, td.tm_min,
		       td.tm_sec);
		sleep(1);
	}
}
