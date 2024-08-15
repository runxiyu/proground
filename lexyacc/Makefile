CC = gcc -Wall -Wextra -pedantic

all: lineno

.SUFFIXES: .c .l

.l.c:
	lex -f -o $@ $<
