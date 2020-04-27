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
  char *outputFile;
  if (argc == 3) {
    outputFile = argv[2];
  } else if (argc == 2) {
    outputFile = malloc(sizeof(char) * strlen(argv[1]) + 2);
    strcpy(outputFile, argv[1]);
    outputFile[strlen(outputFile) - 3] = '\0';
    strcat(outputFile, "final");
  } else {
    printf("Cantidad de argumentos incorrecta, finalizando programa\n");
    return -1;
  }
  game_t *game = loadGame(argv[1]);
  conwayGoL(game, get_nprocs());
  writeBoard(game->board, outputFile);
  board_destroy(game->board);
  free(game);
  return 0;
}