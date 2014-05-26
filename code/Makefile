CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic
LD=$(CC)
LDFLAGS=-lm
TARGET=essig
OBJECTS=main.o molecule.o simulation.o mock.o orca_calculate_energy.c

debug: CFLAGS+=-g
debug: essig

release: CFLAGS+=-O3 -Werror
release: LDFLAGS+=-s
release: essig

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $(TARGET) $+

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) *.o *~
