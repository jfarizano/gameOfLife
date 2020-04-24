#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "board.h"
#include "game.h"

int main() {
  game_t *game = loadGame("entrada.txt");
  conwayGoL(game, get_nprocs());
  writeBoard(game->board, "salida.txt");
  board_destroy(game->board);
  free(game);
  return 0;
}