#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include "board.h"
#include "game.h"

#define ARRIBA(i, rows) ((i - 1) == -1 ? rows - 1 : i - 1)
#define ABAJO(i, rows) ((i + 1) % rows)
#define IZQUIERDA(j, cols) ((j - 1) == -1 ? cols - 1 : j - 1)
#define DERECHA(j, cols) ((j + 1) % cols)

game_t *globalGame;

game_t *loadGame(const char *filename){
  game_t *game = malloc(sizeof(game_t));
  game->board = malloc(sizeof(board_t));
  FILE *fp = fopen(filename, "r");
  size_t r, c;
  fscanf(fp, "%u %zu %zu", &game->cycles, &r, &c);
  board_init(game->board, c, r);
  board_load(game->board, fp);
  fclose(fp);
  return game;
}

void writeBoard(board_t *board, const char *filename){
  FILE *fp = fopen(filename, "w");
  board_save(board, fp);
  fclose(fp);
}

void conwayGoL(game_t* game, const int nuproc) {
  globalGame = game;
  pthread_t threads[nuproc];

  for (int i = 0; i < nuproc; i++) {
    pthread_create(&threads[i], NULL, nextGen, &i);
  }

  for (int i = 0; i < 5; i++) {
    pthread_join(threads[i], NULL);
  }

  for (unsigned int i = 0; i < game->cycles; i++) {
    // SE PONE BARRERA
    char **temp = game->board->board;
    game->board->board = game->board->nextGen;
    game->board->nextGen = temp;
    // SE LEVANTA BARRERA
  }
  
}

size_t neighborsCount(board_t *board, size_t i, size_t j){
  size_t count = 0, rows = board->rows, cols = board->cols;

  // Vecino arriba.
  if (board->board[ARRIBA(i, rows)][j] == ALIVE) {  
    count++;
  }

  // Vecino abajo.
  if (board->board[ABAJO(i, rows)][j] == ALIVE) { 
    count++;
  }

  // Vecino derecha.
  if (board->board[i][DERECHA(j, cols)] == ALIVE) { 
    count++;
  }

  // Vecino izquierda.
  if (board->board[i][IZQUIERDA(j, cols)] == ALIVE) { 
    count++;
  }
  // Vecino arriba-izquierda.
  if (board->board[ARRIBA(i, rows)][IZQUIERDA(j, cols)] == ALIVE){
    count++;  
  }

  // Vecino arriba-derecha.
  if (board->board[ARRIBA(i, rows)][DERECHA(j, cols)] == ALIVE) { 
    count++;
  }

  // Vecino abajo-izquierda.
  if (board->board[ABAJO(i, rows)][IZQUIERDA(j, cols)] == ALIVE) { 
    count++;
  }
  
  // Vecino abajo-derecha.
  if (board->board[ABAJO(i, rows)][DERECHA(j, cols)] == ALIVE) { 
    count++;
  }

  return count;
}

void newCellState(board_t *board, size_t i, size_t j){
  size_t count = neighborsCount(board, i, j);
  if (board->board[i][j] == DEAD && count == 3) {
    board->nextGen[i][j] = ALIVE;
  } else if (board->board[i][j] == ALIVE && (count <= 1 || count >= 4)) {
   board->nextGen[i][j] = DEAD;
  } else {
    board->nextGen[i][j] = board->board[i][j];
  }
}

void *nextGen(void* arg){
  int threadId = *((int*) arg);
  char *temp;

  for (size_t i = 0; i < globalGame->board->rows; i++) {
    for (size_t j = 0; j < globalGame->board->cols; j++) {
      newCellState(globalGame->board, i, j);
    }
  }
  // ESPERO
  for (size_t i = 0; i < globalGame->board->rows; i++) {
    temp = globalGame->board->board[i];
    globalGame->board->board[i] = globalGame->board->nextGen[i];
    globalGame->board->nextGen[i] = temp;
  } 
  // ESPERO 
}