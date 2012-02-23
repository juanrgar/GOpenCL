
CC       = gcc
CFLAGS   = -Wall -O3 `pkg-config --cflags gobject-2.0`
LDFLAGS  = `pkg-config --libs gobject-2.0`

LIBS =

EXEC = plat

all : $(EXEC)

plat : plat.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

plat.o : plat.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f $(EXEC) *.o *~


