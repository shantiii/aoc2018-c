IDIR =.
CC=gcc
CFLAGS=-I$(IDIR) -O3 -Wall -Wextra -pedantic -std=c11

ODIR=obj
BINDIR=bin

_DEPS = $(wildcard *.h)
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

CSRCS = $(wildcard *.c)

_OBJ = $(patsubst %.c, %.o, ${CSRCS})
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(ODIR) $(BINDIR) $(BINDIR)/aoc

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
