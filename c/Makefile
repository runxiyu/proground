%: %.c
	gcc -Wall -Werror -pedantic $< -o $@ -largon2

power: power.c
	gcc -Wall -Werror -pedantic $(shell pkg-config --cflags --libs dbus-1) -lm power.c -o power
