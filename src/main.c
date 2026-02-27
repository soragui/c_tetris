#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "game.h"
#include "render.h"
#include "input.h"
#include "score.h"

int main(void) {
    // 加载最高分记录
    load_high_scores();
    
    // 初始化渲染
    render_init();
    
    // 创建和初始化游戏
    Game* game = game_create();
    if (!game) {
        render_cleanup();
        fprintf(stderr, "Failed to create game!\n");
        return 1;
    }
    
    game_init(game);
    
    // 游戏主循环
    while (game->running) {
        int key = input_get_key();
        if (key != ERR) {
            input_handle_game(game, key);
        }
        
        game_update(game);
        game_render(game);
        
        napms(game->paused ? 100 : 50);
    }
    
    // 游戏结束，检查是否是新高分
    if (game->score > 0 && is_new_high_score(game->score)) {
        // 显示输入名字界面
        clear();
        mvprintw(5, 10, "╔══════════════════════════════╗");
        mvprintw(6, 10, "║    NEW HIGH SCORE!           ║");
        mvprintw(7, 10, "║    Score: %-6d              ║", game->score);
        mvprintw(8, 10, "║    Enter your name:          ║");
        mvprintw(9, 10, "║                              ║");
        mvprintw(10, 10, "╚══════════════════════════════╝");
        refresh();
        
        // 获取用户输入
        echo();
        curs_set(1);
        move(9, 35);
        
        char name[32] = {0};
        int i = 0;
        int ch;
        while ((ch = getch()) != '\n' && ch != KEY_ENTER && i < 31) {
            if (ch == 127 || ch == KEY_BACKSPACE || ch == 8) {
                if (i > 0) {
                    i--;
                    name[i] = '\0';
                    mvaddch(9, 35 + i, ' ');
                    move(9, 35 + i);
                }
            } else if (ch >= 32 && ch <= 126) {
                name[i] = ch;
                i++;
                addch(ch);
            }
            refresh();
        }
        name[i] = '\0';
        
        // 如果没有输入名字，使用默认名
        if (strlen(name) == 0) {
            strcpy(name, "Player");
        }
        
        // 保存最高分
        save_high_score(name, game->score, game->lines_cleared, game->level);
        
        noecho();
        curs_set(0);
    }
    
    // 显示最高分排行榜
    load_high_scores();
    clear();
    show_high_scores();
    mvprintw(15, 5, "Press any key to exit...");
    refresh();
    getch();
    
    // 清理
    game_destroy(game);
    render_cleanup();
    
    printf("Thanks for playing Tetris!\n");
    return 0;
}
