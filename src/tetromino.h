#ifndef TETROMINO_H
#define TETROMINO_H

#include <stdbool.h>

// 游戏板尺寸
#define BOARD_WIDTH   10
#define BOARD_HEIGHT  20

// 方块类型
typedef enum {
    TETRO_I,
    TETRO_O,
    TETRO_T,
    TETRO_S,
    TETRO_Z,
    TETRO_J,
    TETRO_L,
    TETRO_COUNT
} TetrominoType;

// 方块结构
typedef struct {
    TetrominoType type;
    int x, y;              // 位置
    int rotation;          // 0-3
    int color;             // 颜色对 (1-7)
    int shape[4][4];       // 4x4 形状矩阵
} Tetromino;

// 函数声明
void tetromino_init(Tetromino* t, TetrominoType type);
void tetromino_rotate(Tetromino* t);
void tetromino_move(Tetromino* t, int dx, int dy);
bool tetromino_check_collision(Tetromino* t, int board[BOARD_HEIGHT][BOARD_WIDTH]);

#endif
