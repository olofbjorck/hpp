CC = gcc
CFLAGS = -Wall #-O3 -march=native
#CFLAGS += -g
LDFLAGS = -L/opt/X11/lib -lX11 -lm

INCLUDES = -I/opt/X11/include -Igraphics

galsim: io.o main.o quadtree.o galsim.o graphics.o
	$(CC) galsim.o graphics.o main.o quadtree.o io.o -o galsim $(LDFLAGS)

galsim.o: galsim.c galsim.h
	$(CC) $(CFLAGS) $(INCLUDES) -c galsim.c

io.o: io.c io.h
	$(CC) $(CFLAGS) $(INCLUDES) -c io.c

quadtree.o: quadtree.c quadtree.h
	$(CC) $(CFLAGS) $(INCLUDES) -c quadtree.c

main.o: main.c modules.h
	$(CC) $(CFLAGS) $(INCLUDES) -c main.c

graphics.o: graphics/graphics.c graphics/graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c graphics/graphics.c

clean:
	rm -f galsim galsim.o main.o io.o quadtree.o graphics.o

clean-all:
	rm -f galsim galsim.o main.o io.o quadtree.o graphics.o result.gal
