#ifndef BOARD
#define BOARD

#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/
/* Definición de la estructura de datos del tablero */

typedef struct _board{
  size_t rows, cols;
  char **board;
  char **nextGen;
}board_t;

/******************************************************************************/
/******************************************************************************/

/* Funciones sobre el tablero */

/* Creación del tablero */
void board_init(board_t *board_, size_t col, size_t row);

/* Creación del tablero con un elemento por default*/
void board_init_def(board_t *board_, size_t col, size_t row, char def);

/* Recibe el puntero al archivo apuntando al inicio del tablero
  y lo almacena en memoria */
void board_load(board_t *board, FILE *inputFile);

/* Guarda un tablero en un archivo */
void board_save(board_t *board, FILE *outputFile);

/* Función para mostrar el tablero */
void board_show(board_t *board_);

/* Destroy board */
void board_destroy(board_t *board_);
#endif
