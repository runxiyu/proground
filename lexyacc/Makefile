CC = gcc -Wall -Wextra -pedantic

all: lineno nothing

.SUFFIXES: .c .l

.l.c:
	lex -f -o $@ $<
