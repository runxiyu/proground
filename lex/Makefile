CC = gcc -Wall -Wextra -pedantic -Wno-unused-function

.SUFFIXES: .c .l
.PHONY: clean

BINS = lineno nothing wc justprint identifiers

all: $(BINS)

.l.c:
	lex -f -o $@ $<

clean:
	rm -f *.c *.o $(BINS)
