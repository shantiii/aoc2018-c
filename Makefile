IDIR =.
CC=gcc
CFLAGS=-I$(IDIR) -Wall -Wextra -pedantic -std=c99

ODIR=obj
BINDIR=bin

_DEPS = vector.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o vector.o problem3.o problem4.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(BINDIR)/aoc: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(ODIR)/:
	mkdir $(ODIR)

$(BINDIR)/:
	mkdir $(BINDIR)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ 
