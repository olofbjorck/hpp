CC = gcc
CFLAGS = -Wall -O3 -march=native
#CFLAGS += -g
LDFLAGS = -L/opt/X11/lib -lX11 -lm

INCLUDES = -I/opt/X11/include -Igraphics

galsim: galsim.o graphics.o
	$(CC) galsim.o graphics.o -o galsim $(LDFLAGS)

galsim.o: galsim.c
	$(CC) $(CFLAGS) $(INCLUDES) -c galsim.c

graphics.o: graphics/graphics.c graphics/graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c graphics/graphics.c

clean:
	rm -f galsim galsim.o graphics.o
