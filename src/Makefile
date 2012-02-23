
CC       = gcc
CFLAGS   = -Wall -O3 `pkg-config --cflags gobject-2.0`
LDFLAGS  =
LIBS     = `pkg-config --libs gobject-2.0`

LIB  = libgopencl.a

all : lib $(EXEC)

lib : gopencl-platform.o
	ar -r -s $(LIB) gopencl-platform.o

gopencl-platform.o : gopencl-platform.c gopencl-platform.h
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f $(EXEC) *.o *~ $(LIB)
