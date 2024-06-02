#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdint.h>
#include <err.h>


int fd;
char buf[4];
struct stat sb;

int main() {
	buf[3] = '\n';
	for (;;) {
		// There's gotta be a better way to do this than polling the battery
		// file. I could use dbus to contact upower but I'm unfamiliar with
		// dbus interrupts.
		// Note that inotify won't work  with /sys/class files.
		// And I'm not in the mood of handling errors today.
		fd = open("/sys/class/power_supply/macsmc-battery/capacity", O_RDONLY);
		fstat(fd, &sb);
		printf("%llu\n", (unsigned long long)(sb.st_size));
		_exit(1);
		read(fd, buf, sb.st_size);
		lseek(fd, 0, SEEK_SET);
		write(1, buf, sb.st_size);
		close(fd);
	}
}
