CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-result -pedantic -lpthread -O3 
BOARD = board.o
GAME = game.o

main: main.c $(BOARD) $(GAME)
	$(CC) main.c $(BOARD) $(GAME) -o simulador $(CFLAGS)

$(BOARD): board.c
	$(CC) board.c -c -o $(BOARD) $(CFLAGS)

$(GAME): game.c
	$(CC) game.c -c -o $(GAME) $(CFLAGS)

memleak: main
	valgrind --tool=memcheck --leak-check=full --track-origins=yes -s ./simulador entrada.game

ramusage: main
	valgrind --tool=massif --massif-out-file=massif.out ./simulador entrada.game
	massif-visualizer massif.out

.PHONY: clean
clean:
	rm -f simulador
	rm -rf *.o
	rm -rf *.out
	rm -rf *.final