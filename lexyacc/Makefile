CC = gcc -Wall -Wextra -pedantic

all: lineno nothing wc

.SUFFIXES: .c .l

.l.c:
	lex -f -o $@ $<
