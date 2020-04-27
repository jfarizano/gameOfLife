#include "game.h"

#define ARRIBA(i, rows) ((i - ((size_t) 1)) == ((size_t) -1) ? rows - ((size_t) 1) : i - ((size_t) 1))
#define ABAJO(i, rows) ((i + ((size_t) 1)) % rows)
#define IZQUIERDA(j, cols) ((j - ((size_t) 1)) ==  ((size_t) -1) ? cols - ((size_t) 1) : j - ((size_t) 1))
#define DERECHA(j, cols) ((j + ((size_t) 1)) % cols)

pthread_barrier_t barrier;

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
  // Si hay mas hilos que filas usaremos la misma cantidad de hilos que de filas
  int cantHilos = nuproc > (int)game->board->rows ? (int)game->board->rows : nuproc;
  pthread_t threads[cantHilos];
  threadInfo_t *tInfo[cantHilos];

  size_t n = game->board->rows / cantHilos;
  size_t mod = game->board->rows % cantHilos;
  size_t actual = 0;

  // Le asignamos a cada hilo sus filas
  for (int i = 0; i < cantHilos; i++) {
    tInfo[i] = malloc(sizeof(threadInfo_t));
    tInfo[i]->game = game;
    tInfo[i]->id = i;
    tInfo[i]->from = actual;
    actual += n;
    if (mod) {
      actual++;
      mod--;
    }
    tInfo[i]->to = actual;
  }

  pthread_barrier_init(&barrier, NULL, cantHilos);

  for (int i = 0; i < cantHilos; i++) {
    pthread_create(&threads[i], NULL, nextGen, tInfo[i]);
  }

  for (int i = 0; i < cantHilos; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < cantHilos; i++) {
    free(tInfo[i]);
  }
  
  pthread_barrier_destroy(&barrier);
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

void *nextGen(void* arg) {
  threadInfo_t *tInfo = (threadInfo_t*) arg;
  char *temp;

  for (size_t i = 0; i < tInfo->game->cycles; i++) {
    for (size_t j = tInfo->from; j < tInfo->to; j++) {
      for (size_t k = 0; k < tInfo->game->board->cols; k++) {
        newCellState(tInfo->game->board, j, k);
      }
    }
    // El hilo espera a que los demás terminen de calcular el nuevo estado
    pthread_barrier_wait(&barrier);
    // Para copiar las filas al tablero nuevo, simplemente intercambiamos los 
    // punteros de las mismas, así nos evitamos recorrer el tablero entero.
    for (size_t j = tInfo->from; j < tInfo->to; j++) {
      temp = tInfo->game->board->board[j];
      tInfo->game->board->board[j] = tInfo->game->board->nextGen[j];
      tInfo->game->board->nextGen[j] = temp;
    }
    // El hilo espera a que los demás terminen de copiar sus filas.
    pthread_barrier_wait(&barrier);
  } 

  return NULL;
}