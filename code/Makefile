CC=gcc
CFLAGS=-Wall -Werror -std=c99

debug: CFLAGS+=-g
debug: essig

release: CFLAGS+=-O3
release: essig

essig: main.o molecule.o simulation.o
	$(CC) $(CFLAGS) -o essig main.o molecule.o simulation.o

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)
