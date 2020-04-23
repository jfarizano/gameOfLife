#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "board.h"
#include "game.h"

int main(){
  game_t *game = loadGame("entrada.txt");
  printf("%u %zu %zu\n", game->cycles, game->board->rows, game->board->cols);
  board_show(game->board);
  nextGen(game->board);
  printf("================================\n");
  board_show(game->board);
  board_destroy(game->board);
  free(game);
  // writeBoard(game->board, "salida.txt");

  return 0;
}