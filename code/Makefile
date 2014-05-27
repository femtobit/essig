CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic
LD=$(CC)
LDFLAGS=-lm
TARGET=essig
OBJECTS=main.o molecule.o simulation.o mock.o orca_calculate_energy.o displacement.o rotate.o vector.o output.o

debug: CFLAGS+=-g
debug: $(TARGET)

release: CFLAGS+=-O3 -Werror
release: LDFLAGS+=-s
release: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $+

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) *.o *~
