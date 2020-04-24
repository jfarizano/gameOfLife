#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "board.h"

/******************************************************************************/
/* Representamos las células vivas como 'O' y las muertas como 'X' */
#define ALIVE 'O'
#define DEAD 'X'
/******************************************************************************/
/* La estructura de un juego es simplemente un tablero y la cantidad de veces
que se va a iterar */
typedef struct _game {
  board_t *board;
  unsigned int cycles;
} game_t;

typedef struct _threadInfo {
  game_t* game;
  size_t from, to;
  int id;
} threadInfo_t;


/******************************************************************************/

/* Cargamos el juego desde un archivo */
game_t *loadGame(const char *filename);

/* Guardamos el tablero 'board' en el archivo 'filename' */
void writeBoard(board_t *board, const char *filename);

/* Simulamos el Juego de la Vida de Conway con tablero 'board' la cantidad de
ciclos indicados en 'cycles' en 'nuprocs' unidades de procesamiento*/
void conwayGoL(game_t* game, const int nuproc);

/* Cuenta la cantidad de vecinos vivos para la posición i,j */
size_t neighborsCount(board_t *board, size_t i, size_t j);

/* Modifica el estado de una célula según las reglas del juego */
void newCellState(board_t *board, size_t i, size_t j);

/* Modifica todas las células del tablero usando newCellState */
void *nextGen(void *arg);
