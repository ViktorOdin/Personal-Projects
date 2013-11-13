#
# Jeux_de_plateau
# 
# \author Oudin Victor
#

CC=g++
FLAGS=-W -Wall
EXEC=main

all: $(EXEC)

main: piecechess.o mines.o chess.o minesweeper.o play.o main.o
	$(CC) -o $@ $^ -g

main.o: main.cpp play.h
	$(CC) -o $@ -c $< -g $(FLAGS)

play.o: play.cpp chess.h minesweeper.h
	$(CC) -o $@ -c $< -g $(FLAGS)

chess.o: chess.cpp piecechess.h
	$(CC) -o $@ -c $< -g $(FLAGS)

minesweeper.o: minesweeper.cpp mines.h
	$(CC) -o $@ -c $< -g $(FLAGS)

piecechess.o: piecechess.cpp
	$(CC) -o $@ -c $< -g $(FLAGS)

mines.o: mines.cpp
	$(CC) -o $@ -c $< -g $(FLAGS)

clean:
	rm -rf *.o

remake: clean
	rm -rf $(EXEC)
	make

mrproper: clean
	rm -f $(EXEC)