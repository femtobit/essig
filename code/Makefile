CC=gcc
CFLAGS=-Wall -std=gnu99 -lm

debug: CFLAGS+=-g
debug: essig

release: CFLAGS+=-O3 -Werror
release: essig

essig: main.o molecule.o simulation.o mock.o
	$(CC) $(CFLAGS) -o essig main.o molecule.o simulation.o mock.o

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm *.x *.o *~
