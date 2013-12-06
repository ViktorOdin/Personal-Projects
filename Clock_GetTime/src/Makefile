##
##	Victor Oudin
##	11/19/2013
##
##	Makefile
##

CC=gcc
CFLAGS=-W -Wall -g
LDFLAGS=-lpthread -lrt
EXEC=main
OBJ=main.o threadPipe.o threadSema.o thread.o forkPipe.o forkSema.o fork.o thread.h fork.h


all: $(EXEC)

main.o: main.c fork.h thread.h
	$(CC) $(CFLAGS) -o $@ -c $<

threadPipe.o: threadPipe.c thread.h
threadSema.o: threadSema.c thread.h
thread.o: thread.c thread.h
	$(CC) $(CFLAGS) -o $@ -c $<

forkPipe.o: forkPipe.c fork.h
forkSema.o: forkSema.c fork.h
fork.o: fork.c fork.h
	$(CC) $(CFLAGS) -o $@ -c $<

main: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

resolution.o: resolution.c
	$(CC) $(CFLAGS) -o $@ -c $<

resolution: resolution.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:	
	rm -f *.o
	rm -f *.a
	rm -f *~

mrproper: clean
	rm -f $(EXEC)
	rm -f resolution

pkg: mrproper
	tar -czvf ../Victor_Oudin_Tp3.tar.gz ../*

