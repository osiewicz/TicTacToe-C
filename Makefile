 CC=gcc
 CFLAGS=

ODIR=obj
LDI =../lib

LIBS=-lm

_OBJ = v0.1.0.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

Tic-Tac-Toe: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
