#ifndef INPUT_H
#define INPUT_H

#include "game.h"

// 输入处理函数声明
void input_init(void);
int input_get_key(void);
void input_handle_game(Game* game, int key);

// 按键定义
#define KEY_SOFT_DROP   'v'
#define KEY_HARD_DROP   ' '
#define KEY_ROTATE      KEY_UP
#define KEY_MOVE_LEFT   KEY_LEFT
#define KEY_MOVE_RIGHT  KEY_RIGHT
#define KEY_HOLD        'c'
#define KEY_PAUSE       'p'
#define KEY_QUIT        'q'
#define KEY_START       'r'

#endif
