CFLAGS += $(INCLUDEFLAGS) -D_REENTRANT -ggdb3 -Wall -pedantic -Wextra -Wsign-conversion -Wno-unused-parameter $(shell pkg-config ncurses --cflags) -std=gnu99

LDFLAGS = $(shell pkg-config ncurses --libs)

.PHONY: all clean


all: spmc

spmc: spmc.o
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) spmc
