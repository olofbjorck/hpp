CC = gcc
CFLAGS = -Wall -O3 -march=native 
#CFLAGS += -g
LDFLAGS = -L/opt/X11/lib -lX11 -lm

INCLUDES = -I/opt/X11/include -Igraphics

all: galsim compare_gal_files


galsim: galsim.o graphics.o
	$(CC) galsim.o graphics.o -o galsim $(LDFLAGS)

compare_gal_files: compare_gal_files.c
	$(CC) -o compare_gal_files compare_gal_files.c -lm

galsim.o: galsim.c
	$(CC) $(CFLAGS) $(INCLUDES) -c galsim.c

graphics.o: graphics/graphics.c graphics/graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c graphics/graphics.c

clean:
	rm -f galsim galsim.o graphics.o compare_gal_files
