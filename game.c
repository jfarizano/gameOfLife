#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "game.h"

#define ARRIBA(i, rows) ((i - 1) == -1 ? rows - 1 : i - 1)
#define ABAJO(i, rows) ((i + 1) % rows)
#define IZQUIERDA(j, cols) ((j - 1) == -1 ? cols - 1 : j - 1)
#define DERECHA(j, cols) ((j + 1) % cols)

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

board_t *conwayGoL(board_t *board, unsigned int cycles, const int nuproc);

size_t neighborsCount(board_t *board, size_t i, size_t j){
  size_t count = 0, rows = board->rows, cols = board->cols, index1, index2;

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

void nextGen(board_t *board){
  board->nextGen = malloc(sizeof(char*) * board->rows);
  for (size_t i = 0; i < board->rows; i++){
      board->nextGen[i] = malloc(sizeof(char) * (board->cols + 1));
  }

  // PARALELIZAR ESTO
  for (size_t i = 0; i < board->rows; i++) {
    for (size_t j = 0; j < board->cols; j++) {
      newCellState(board, i, j);
    }
  }

  for (size_t i = 0; i < board->rows; i++) {
    free(board->board[i]);
  }
  free(board->board);

  board->board = board->nextGen;
  board->nextGen = NULL;
}

/*
  IDEA:
  Tenemos guardado el actual en board->board,
  le hacemos newCellState y lo nuevo está en board->nextGen,
  guardamos board->board en un temporal, 
  board->board = board->nextGen y luego board->nextGen = temp
*/