#include "render.h"
#include <ncurses.h>
#include <stdio.h>

void render_init(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);
    
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);    // I
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // O
        init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // T
        init_pair(4, COLOR_GREEN, COLOR_BLACK);   // S
        init_pair(5, COLOR_RED, COLOR_BLACK);     // Z
        init_pair(6, COLOR_BLUE, COLOR_BLACK);    // J
        init_pair(7, COLOR_WHITE, COLOR_BLACK);   // L
        init_pair(8, COLOR_WHITE, COLOR_BLACK);   // 边框
    }
}

void render_cleanup(void) {
    endwin();
}

void render_block(int x, int y, int color) {
    if (color > 0) {
        attron(COLOR_PAIR(color));
        mvaddstr(y, x * 2, "██");
        attroff(COLOR_PAIR(color));
    } else {
        mvaddstr(y, x * 2, "  ");
    }
}

void render_board_border(int offset_x, int offset_y) {
    attron(COLOR_PAIR(8));
    
    // 上边框
    mvaddch(offset_y, offset_x * 2 - 2, '+');
    for (int i = 0; i < BOARD_WIDTH; i++) {
        mvaddch(offset_y, offset_x * 2 + i * 2, '-');
        mvaddch(offset_y, offset_x * 2 + i * 2 + 1, '-');
    }
    mvaddch(offset_y, offset_x * 2 + BOARD_WIDTH * 2, '+');
    
    // 下边框
    mvaddch(offset_y + BOARD_HEIGHT + 1, offset_x * 2 - 2, '+');
    for (int i = 0; i < BOARD_WIDTH; i++) {
        mvaddch(offset_y + BOARD_HEIGHT + 1, offset_x * 2 + i * 2, '-');
        mvaddch(offset_y + BOARD_HEIGHT + 1, offset_x * 2 + i * 2 + 1, '-');
    }
    mvaddch(offset_y + BOARD_HEIGHT + 1, offset_x * 2 + BOARD_WIDTH * 2, '+');
    
    // 左右边框
    for (int i = 0; i <= BOARD_HEIGHT; i++) {
        mvaddch(offset_y + i, offset_x * 2 - 2, '|');
        mvaddch(offset_y + i, offset_x * 2 + BOARD_WIDTH * 2, '|');
    }
    
    attroff(COLOR_PAIR(8));
}

void render_tetromino(Tetromino* t, int offset_x, int offset_y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->shape[i][j]) {
                int x = t->x + j;
                int y = t->y + i + offset_y;
                if (y >= offset_y) {
                    render_block(x + offset_x, y, t->color);
                }
            }
        }
    }
}

void render_next_piece(Tetromino* t, int offset_x, int offset_y) {
    mvprintw(offset_y, offset_x * 2, "NEXT:");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->shape[i][j]) {
                attron(COLOR_PAIR(t->color));
                mvaddstr(offset_y + 1 + i, offset_x * 2 + j * 2, "██");
                attroff(COLOR_PAIR(t->color));
            }
        }
    }
}

void render_hold_piece(Tetromino* t, int offset_x, int offset_y) {
    mvprintw(offset_y, offset_x * 2, "HOLD:");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->shape[i][j]) {
                attron(COLOR_PAIR(t->color));
                mvaddstr(offset_y + 1 + i, offset_x * 2 + j * 2, "██");
                attroff(COLOR_PAIR(t->color));
            }
        }
    }
}

void render_score(int score, int level, int lines, int offset_x, int offset_y) {
    mvprintw(offset_y, offset_x * 2, "SCORE:");
    mvprintw(offset_y + 1, offset_x * 2, "%d", score);
    mvprintw(offset_y + 3, offset_x * 2, "LEVEL:");
    mvprintw(offset_y + 4, offset_x * 2, "%d", level);
    mvprintw(offset_y + 6, offset_x * 2, "LINES:");
    mvprintw(offset_y + 7, offset_x * 2, "%d", lines);
}

void render_controls(int offset_x, int offset_y) {
    mvprintw(offset_y, offset_x * 2, "CONTROLS:");
    mvprintw(offset_y + 1, offset_x * 2, "←/→ Move");
    mvprintw(offset_y + 2, offset_x * 2, "↑   Rotate");
    mvprintw(offset_y + 3, offset_x * 2, "↓   Soft Drop");
    mvprintw(offset_y + 4, offset_x * 2, "Space Hard Drop");
    mvprintw(offset_y + 5, offset_x * 2, "C   Hold");
    mvprintw(offset_y + 6, offset_x * 2, "P   Pause");
    mvprintw(offset_y + 7, offset_x * 2, "Q   Quit");
}

void render_game(Game* game) {
    clear();
    
    // 绘制游戏区域
    render_board_border(0, 0);
    
    // 绘制已固定的方块
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (game->board.cells[y][x]) {
                render_block(x, y + 1, game->board.cells[y][x]);
            }
        }
    }
    
    // 绘制当前方块
    render_tetromino(&game->current, 0, 1);
    
    // 绘制下一个方块
    render_next_piece(&game->next, BOARD_WIDTH + 2, 1);
    
    // 绘制保留方块
    render_hold_piece(&game->hold, BOARD_WIDTH + 2, 8);
    
    // 绘制分数
    render_score(game->score, game->level, game->lines_cleared, BOARD_WIDTH + 2, 14);
    
    // 绘制控制说明
    render_controls(BOARD_WIDTH + 2, 20);
    
    // 游戏结束提示
    if (game->game_over) {
        render_game_over(game);
    }
    
    // 暂停提示
    if (game->paused) {
        render_pause();
    }
    
    refresh();
}

void render_game_over(Game* game) {
    attron(A_BOLD | A_REVERSE);
    mvprintw(BOARD_HEIGHT / 2, 10, " GAME OVER ");
    mvprintw(BOARD_HEIGHT / 2 + 1, 10, " Score: %d ", game->score);
    mvprintw(BOARD_HEIGHT / 2 + 2, 10, " R-Restart Q-Quit ");
    attroff(A_BOLD | A_REVERSE);
}

void render_pause(void) {
    attron(A_BOLD | A_REVERSE);
    mvprintw(BOARD_HEIGHT / 2, 15, " PAUSED ");
    mvprintw(BOARD_HEIGHT / 2 + 1, 15, " P-Resume ");
    attroff(A_BOLD | A_REVERSE);
}
