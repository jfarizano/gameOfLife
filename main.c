#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <string.h>
#include "board.h"
#include "game.h"

// TODO: 
// -comprobar si hay memory leaks, comentar en caso de que falte
// -hacer un readme que diga como compilar y ejecutar esto o ver si ponerlo en 
//  el pdf

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Cantidad de argumentos incorrecta, finalizando programa\n");
    return -1;
  }

  char *outputFile;
  outputFile = malloc(sizeof(char) * strlen(argv[1]) + 2);
  strcpy(outputFile, argv[1]);
  outputFile[strlen(outputFile) - 4] = '\0';
  strcat(outputFile, "final");

  game_t *game = loadGame(argv[1]);
  conwayGoL(game, get_nprocs());
  writeBoard(game->board, outputFile);
  
  board_destroy(game->board);
  free(game);
  free(outputFile);

  return 0;
}