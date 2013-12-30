##
##	Victor Oudin
##	21/12/2013
##
##	Makefile
##

CC=gcc
CFLAGS=-W -Wall -g
LDFLAGS=-lpthread	## Library -lrt
EXEC=main		## Exec
OBJ=main.o		## Objecte and Header


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

