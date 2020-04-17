#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "game.h"

int main(){
  game_t *game = loadGame("entrada.txt");
  printf("%u %zu %zu\n", game->cycles, game->board->rows, game->board->cols);
  board_show(game->board);
  // for (size_t i = 0; i < game->board->rows; i++) {
  //   for (size_t j = 0; j < game->board->cols; j++) {
  //     printf("(%zu, %zu) tiene %zu\n", i, j, neighborsCount(game->board, i, j));
  //   }
  // }
  nextGen(game->board);
  printf("================================\n");
  board_show(game->board);
  // writeBoard(game->board, "salida.txt");

  return 0;
}