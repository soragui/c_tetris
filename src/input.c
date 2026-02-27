#include "input.h"
#include <ncurses.h>

void input_init(void) {
    // 在 render_init 中已初始化
}

int input_get_key(void) {
    return getch();
}

void input_handle_game(Game* game, int key) {
    if (!game) return;
    
    // 游戏结束时的输入
    if (game->game_over) {
        if (key == 'r' || key == 'R') {
            game_init(game);
        } else if (key == 'q' || key == 'Q') {
            game->running = false;
        }
        return;
    }
    
    // 暂停状态
    if (game->paused) {
        if (key == 'p' || key == 'P') {
            game->paused = false;
        } else if (key == 'q' || key == 'Q') {
            game->running = false;
        }
        return;
    }
    
    // 游戏控制
    switch (key) {
        case KEY_LEFT:
        case 'h':
            game->current.x--;
            if (board_check_collision(&game->board, &game->current)) {
                game->current.x++;
            }
            break;
            
        case KEY_RIGHT:
        case 'l':
            game->current.x++;
            if (board_check_collision(&game->board, &game->current)) {
                game->current.x--;
            }
            break;
            
        case KEY_DOWN:
        case 'j':
            game->current.y++;
            if (board_check_collision(&game->board, &game->current)) {
                game->current.y--;
            }
            break;
            
        case KEY_UP:
        case 'k':
        case 'w':
            {
                Tetromino temp = game->current;
                tetromino_rotate(&temp);
                if (!board_check_collision(&game->board, &temp)) {
                    game->current = temp;
                }
            }
            break;
            
        case ' ':
            game_hard_drop(game);
            break;
            
        case 'c':
        case 'C':
            if (!game->has_held) {
                Tetromino temp = game->hold;
                game->hold = game->current;
                tetromino_init(&game->current, temp.type);
                game->has_held = true;
            }
            break;
            
        case 'p':
        case 'P':
            game->paused = true;
            break;
            
        case 'q':
        case 'Q':
            game->running = false;
            break;
    }
}
