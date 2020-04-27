#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "board.h"
#include "game.h"

// TODO: 
// -arreglar warning del argc, 
// -visual me tira: identifier "pthread_barrier_t" is undefined
// -comprobar si hay memory leaks, comentar en caso de que falte
// -hacer un readme que diga como compilar y ejecutar esto o ver si ponerlo en 
//  el pdf

int main(int argc, char* argv[]) {
  game_t *game = loadGame(argv[1]);
  conwayGoL(game, get_nprocs());
  // conwayGoL(game, 1);
  writeBoard(game->board, "salida.txt");
  board_destroy(game->board);
  free(game);
  return 0;
}