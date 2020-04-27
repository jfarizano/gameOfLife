#include "game.h"

#define UP(i, rows) ((i - ((size_t) 1)) == ((size_t) -1) ? rows - ((size_t) 1) : i - ((size_t) 1))
#define DOWN(i, rows) ((i + ((size_t) 1)) % rows)
#define LEFT(j, cols) ((j - ((size_t) 1)) ==  ((size_t) -1) ? cols - ((size_t) 1) : j - ((size_t) 1))
#define RIGHT(j, cols) ((j + ((size_t) 1)) % cols)

pthread_barrier_t barrier;

game_t *loadGame(const char *filename){
  game_t *game = malloc(sizeof(game_t));
  game->board = malloc(sizeof(board_t));
  FILE *fp = fopen(filename, "r");
  size_t r, c;
  fscanf(fp, "%u %zu %zu", &game->cycles, &r, &c);
  board_init(game->board, r, c);
  board_load(game->board, fp);
  fclose(fp);
  
  return game;
}

void writeBoard(board_t *board, const char *filename){
  FILE *fp = fopen(filename, "w");
  board_save(board, fp);
  fclose(fp);
}

size_t neighborsCount(board_t *board, size_t i, size_t j){
  size_t count = 0, rows = board->rows, cols = board->cols;

  // Vecino arriba.
  if (board->board[UP(i, rows)][j] == ALIVE) {  
    count++;
  }

  // Vecino abajo.
  if (board->board[DOWN(i, rows)][j] == ALIVE) { 
    count++;
  }

  // Vecino derecha.
  if (board->board[i][RIGHT(j, cols)] == ALIVE) { 
    count++;
  }

  // Vecino izquierda.
  if (board->board[i][LEFT(j, cols)] == ALIVE) { 
    count++;
  }
  // Vecino arriba-izquierda.
  if (board->board[UP(i, rows)][LEFT(j, cols)] == ALIVE){
    count++;  
  }

  // Vecino arriba-derecha.
  if (board->board[UP(i, rows)][RIGHT(j, cols)] == ALIVE) { 
    count++;
  }

  // Vecino arriba-izquierda.
  if (board->board[DOWN(i, rows)][LEFT(j, cols)] == ALIVE) { 
    count++;
  }
  
  // Vecino abajo-derecha.
  if (board->board[DOWN(i, rows)][RIGHT(j, cols)] == ALIVE) { 
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
  char **temp;

  for (size_t i = 0; i < tInfo->game->cycles; i++) {
    for (size_t j = tInfo->from; j < tInfo->to; j++) {
      for (size_t k = 0; k < tInfo->game->board->cols; k++) {
        newCellState(tInfo->game->board, j, k);
      }
    }
    // Los hilos esperan a que los demás terminen de calcular el nuevo estado
    pthread_barrier_wait(&barrier);
    // Seleccionamos al thread 0 que siempre existirá, para que realice el
    // intercambio de tableros.
    if (tInfo->id == 0) {
      temp = tInfo->game->board->board;
      tInfo->game->board->board = tInfo->game->board->nextGen;
      tInfo->game->board->nextGen = temp;
    }
    // Los hilos esperan a que el thread 0 haga el intercambio de tableros.
    pthread_barrier_wait(&barrier);
  } 

  return NULL;
}

void conwayGoL(game_t* game, const int nuproc) {
  // Si hay mas hilos que filas usaremos la misma cantidad de hilos que de filas
  int nThreads = nuproc > (int)game->board->rows ? (int)game->board->rows : nuproc;
  pthread_t threads[nThreads];
  threadInfo_t *tInfo[nThreads];

  size_t n = game->board->rows / nThreads;
  size_t mod = game->board->rows % nThreads;
  size_t actual = 0;

  // Le asignamos a cada hilo sus filas
  for (int i = 0; i < nThreads; i++) {
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

  pthread_barrier_init(&barrier, NULL, nThreads);

  for (int i = 0; i < nThreads; i++) {
    pthread_create(&threads[i], NULL, nextGen, tInfo[i]);
  }

  for (int i = 0; i < nThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < nThreads; i++) {
    free(tInfo[i]);
  }
  
  pthread_barrier_destroy(&barrier);
}