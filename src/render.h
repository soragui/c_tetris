#ifndef RENDER_H
#define RENDER_H

#include "game.h"

void render_init(void);
void render_cleanup(void);
void render_game(Game* game);
void render_board_border(int offset_x, int offset_y);
void render_tetromino(Tetromino* t, int offset_x, int offset_y);
void render_next_piece(Tetromino* t, int offset_x, int offset_y);
void render_hold_piece(Tetromino* t, int offset_x, int offset_y);
void render_score(int score, int level, int lines, int offset_x, int offset_y);
void render_controls(int offset_x, int offset_y);
void render_game_over(Game* game);
void render_pause(void);

// 兼容 game_render 调用
#define game_render render_game

#endif
