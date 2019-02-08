CC = gcc
CFLAGS = -Wall -O3
#CFLAGS += -g
LDFLAGS = -L/opt/X11/lib -lX11 -lm

INCLUDES = -I/opt/X11/include -Igraphics

galsim: galsim.o graphics.o
	$(CC) galsim.o graphics.o -o galsim $(LDFLAGS)

galsim.o: galsim.c
	$(CC) $(CFLAGS) $(INCLUDES) -c galsim.c

graphics.o: graphics/graphics.c graphics/graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c graphics/graphics.c

compare_cal_files: compare_gal_files.c
	$(CC) -o compare_gal_files compare_gal_files.c -lm

clean:
	rm -f galsim galsim.o graphics.o compare_gal_files
