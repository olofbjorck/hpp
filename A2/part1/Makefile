FILENAME = triang
OBJFILES = $(FILENAME).o
HEADERS = $(FILENAME).h
EXE = $(FILENAME) 

CC = gcc
CFLAGS = -Wall -g
LFLAGS = -lm

$(EXE): $(OBJFILES)
	$(CC) $(OBJFILES) -o $(EXE) $(LFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $^

clean: 
	rm -f $(OBJFILES) $(EXE) $(HEADERS).gch
