CC=gcc
FLAGS=-W -Wall -g
EXEC=test

all: $(EXEC)

dynamic: queue.o dynamic-test.o
	$(CC) -o $@ $^ -lpthread

test: queue.o queue-test.o
	$(CC) -o $@ $^ -lpthread

queue.o: queue.c
	$(CC) -o $@ -c $< $(FLAGS)

queue-test.o: queue-test.c queue.h
	$(CC) -o $@ -c $< $(FLAGS)

dynamic-test.o: dynamic-test.c queue.h
	$(CC) -o $@ -c $< $(FLAGS)

clean:
	rm -f *.o

remake: clean
	rm -f $(EXEC)
	make

mrproper: clean
	rm -f test
	rm -f dynamic