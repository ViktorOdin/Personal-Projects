CC=gcc
CFLAGS=-W -Wall
LDFLAGS=
EXEC=colimacon

all: colimacon

colimacon: colimacon.o main.o
	$(CC) -o $@ $^ $(LDFLAGS)

test: test.o colimacon.o
	$(CC) -o $@ $^ $(LDFLAGS)
	./test

colimacon.o: colimacon.c colimacon.h
main.o: main.c colimacon.h
test.o: test.c colimacon.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:	
	rm -f *.o
	rm -f *.a

mrproper: clean
	rm -f $(EXEC)
	rm -f test

pkg: mrproper
	tar -czvf ../Victor_Oudin_Tp4.tgz ../src
