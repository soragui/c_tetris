#ifndef BOARD_H
#define BOARD_H

#include "tetromino.h"

// 游戏板结构
typedef struct {
    int cells[BOARD_HEIGHT][BOARD_WIDTH];  // 0=空，1-7=颜色
    int filled_rows[BOARD_HEIGHT];         // 每行填充的方块数
} Board;

// 函数声明
void board_init(Board* board);
void board_lock_tetromino(Board* board, Tetromino* t);
int board_clear_lines(Board* board);
bool board_check_collision(Board* board, Tetromino* t);
void board_render(Board* board);

#endif
