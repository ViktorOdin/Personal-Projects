##
##	Victor Oudin
##	21/12/2013
##
##	Makefile
##

CC=gcc
CFLAGS=-W -Wall -g
LDFLAGS=-lpthread
EXEC=main
OBJ=main.o


all: $(EXEC)

main.o: main.c
	$(CC) $(CFLAGS) -o $@ -c $<

main: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:	
	rm -f *.o
	rm -f *.a
	rm -f *~

mrproper: clean
	rm -f $(EXEC)

pkg: mrproper
	tar -czvf ../Victor_Oudin_Archive.tar.gz ../*

