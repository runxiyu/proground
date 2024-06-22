/*
 * Stupid status bar.
 * Set `status_command runxibar` in sway-bar.
 * Written by Runxi Yu. Public domain.
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#define LEN 10

time_t t;
char buf[LEN];
struct tm td;
int fd;
int i;

int main()
{
	fd = open("/sys/class/power_supply/macsmc-battery/capacity", O_RDONLY);
	for (;;) {
		pread(fd, buf, LEN, 0);
		for (i = 0; i < LEN; ++i) {
			if (buf[i] == '\n') {
				buf[i] = '\0';
				break;
			}
		}
		t = time(NULL);
		td = *localtime(&t);
		dprintf(STDOUT_FILENO, "%s%% %d-%02d-%02d %02d:%02d:%02d\n",
			buf,
			td.tm_year + 1900,
			td.tm_mon + 1,
			td.tm_mday, td.tm_hour, td.tm_min, td.tm_sec);
		sleep(1);
	}
}
