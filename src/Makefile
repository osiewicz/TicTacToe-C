IDIR=../include
CC=gcc
CFLAGS=-I$(IDIR) -std=c99

ODIR=obj
LDI =../lib

LIBS=

_DEPS = game_strings.h tic_tac_toe.h
DEPS = $(patsubst $,$(IDIR)/%,$(_DEPS))

_OBJ =  game_strings.o tic_tac_toe.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c  
	$(CC) -c -o $@ $< $(CFLAGS)

Tic-Tac-Toe: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
