#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "tetromino.h"
#include <stdbool.h>

// 游戏状态
typedef struct {
    Board board;
    Tetromino current;     // 当前方块
    Tetromino next;        // 下一个方块
    Tetromino hold;        // 保留的方块
    bool has_held;         // 本回合是否已使用保留
    int score;
    int level;
    int lines_cleared;
    int lines_to_level;    // 距离下一级还需消除的行数
    int drop_interval;     // 下落间隔 (ms)
    bool running;
    bool paused;
    bool game_over;
} Game;

// 函数声明
Game* game_create(void);
void game_init(Game* game);
void game_destroy(Game* game);
void game_update(Game* game);
void game_handle_input(Game* game, int key);
void game_drop(Game* game);
void game_hard_drop(Game* game);

#endif
