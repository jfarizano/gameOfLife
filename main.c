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
  if (argc != 3) {
    printf("Cantidad de argumentos incorrecta, finalizando programa\n");
  }
  game_t *game = loadGame(argv[1]);
  conwayGoL(game, get_nprocs());
  writeBoard(game->board, argv[2]);
  board_destroy(game->board);
  free(game);
  return 0;
}