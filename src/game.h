#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "tetromino.h"
#include <stdbool.h>

typedef struct {
    Board board;
    Tetromino current;
    Tetromino next;
    Tetromino hold;
    Tetromino ghost;      // 影子方块
    bool has_held;
    int score;
    int level;
    int lines_cleared;
    int lines_to_level;
    int drop_interval;
    bool running;
    bool paused;
    bool game_over;
} Game;

Game* game_create(void);
void game_init(Game* game);
void game_destroy(Game* game);
void game_update(Game* game);
void game_handle_input(Game* game, int key);
void game_drop(Game* game);
void game_hard_drop(Game* game);
void game_calculate_ghost(Game* game);

#endif
