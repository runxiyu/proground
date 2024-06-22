#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdint.h>
#include <err.h>
#include <sys/mman.h>

#define LEN 5

int fd;
char buf[LEN];
struct stat sb;
void *ptr;

int main()
{
	buf[LEN - 1] = '\n';
	fd = open("/sys/class/power_supply/macsmc-battery/capacity", O_RDONLY);
	read(fd, buf, LEN);
	printf("%s", buf);
	ptr = mmap(NULL, LEN, PROT_READ, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap");
		_exit(0);
	}
	for (;;) {
		dprintf(1, "%p\n", ptr);
		// There's gotta be a better way to do this than polling the battery
		// file. I could use dbus to contact upower but I'm unfamiliar with
		// dbus interrupts.
		// Note that inotify won't work with /sys/class files.
		// And I'm not in the mood of handling errors today.
	}
}
