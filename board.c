#include <stdio.h>
#include <stdlib.h>
#include "board.h"


void board_init(board_t *board_, size_t col, size_t row){
  board_->rows = row;
  board_->cols = col;
  board_->board = malloc(sizeof(char*) * row);
  board_->nextGen = malloc(sizeof(char*) * row);
  for (size_t i = 0; i < row; i++){
      board_->board[i] = malloc(sizeof(char) * (col + 1));
      board_->nextGen[i] = malloc(sizeof(char) * (col + 1));
  }
}


void board_init_def(board_t *board_, size_t col, size_t row, char def){
  board_->rows = row;
  board_->cols = col;
  board_->board = malloc(sizeof(char*) * row);
  board_->nextGen = NULL;
  for (size_t i = 0; i < row; i++){
      board_->board[i] = malloc(sizeof(char) * (col + 1));
      for (size_t j = 0; j < col; j++){
      board_->board[i][j] = def;
    }
  }
} 

void board_load(board_t *board_, FILE *inputFile){
  for (size_t i = 0; i < board_->rows; i++) {
    fscanf(inputFile, "%s", board_->board[i]);
  }
}

void board_save(board_t *board_, FILE *outputFile){
  for (size_t i = 0; i < board_->rows; i++) {
    fprintf(outputFile, "%s\n", board_->board[i]);
  }
}

void board_show(board_t *board_){
  for (int i = 0; i < board_->rows; i++){
    for(int j = 0; j < board_->cols; j++){
      printf("|%c", board_->board[i][j]);
    }
    printf("|\n");
  }
} 


void board_destroy(board_t *board_){
  for (int i = 0; i < board_->rows; i++){
    free(board_->board[i]);
    free(board_->nextGen[i]);
  }
  free(board_->board);
  free(board_->nextGen);
  free(board_);
}