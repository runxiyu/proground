%: %.c
	gcc -oz -Wall -Wextra -pedantic $< haxstring_utils.o -o $@

power: power.c
	gcc -Wall -Werror -pedantic $(shell pkg-config --cflags --libs dbus-1) -lm power.c -o power
