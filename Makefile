IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
BINDIR=bin

_DEPS = interval_set.h vector.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o interval.o interval_tree.o vector.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(BINDIR)/aoc3: $(OBJ)
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
