#include <stdio.h>
#include <stdlib.h>
#include "board.h"


void board_init(board_t *board_, size_t col, size_t row){
  board_->rows = row;
  board_->cols = col;
  board_->board = malloc(sizeof(char*) * row);
  for(size_t i = 0; i < row; i++){
      board_->board[i] = malloc(sizeof(char) * col);
  }
}


void board_init_def(board_t *board_, size_t col, size_t row, char def){
  board_init(board_, col, row);
  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
      board_->board[i][j] = def;
    }
  }
} 


char board_get(board_t *board_, size_t col, size_t row){
  return board_->board[row][col];
}


void board_set(board_t *board_, size_t col, size_t row, char val){
  board_->board[row][col] = val;
}


int board_load(board_t *board, char *str); // ?????????


void board_show(board_t *board_, char *res){
  for(int i = 0; i < board_->rows; i++){
    for(int j = 0; j < board_->cols; j++){
      printf("|%c", board_->board[i][j]);
    }
    printf("|\n");
  }
} 


void board_destroy(board_t *board_){
  for(int i = 0; i < board_->rows; i++){
    free(board_->board[i]);
  }
  free(board_->board);
  free(board_);
}