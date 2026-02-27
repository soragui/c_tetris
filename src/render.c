#include "render.h"
#include <ncurses.h>
#include <stdio.h>

// 全局偏移量 - 让界面向右和向上移动
#define OFFSET_X 8    // 向右移动 8 列
#define OFFSET_Y -2   // 向上移动 2 行

void render_init(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE);
    
    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(1, COLOR_CYAN, -1);    // I
        init_pair(2, COLOR_YELLOW, -1);  // O
        init_pair(3, COLOR_MAGENTA, -1); // T
        init_pair(4, COLOR_GREEN, -1);   // S
        init_pair(5, COLOR_RED, -1);     // Z
        init_pair(6, COLOR_BLUE, -1);    // J
        init_pair(7, COLOR_WHITE, -1);   // L
        init_pair(8, COLOR_WHITE, -1);   // 边框
    }
}

void render_cleanup(void) {
    endwin();
}

void render_block(int x, int y, int color) {
    if (y < 0 || y >= BOARD_HEIGHT) return;
    
    int screen_x = x + OFFSET_X;
    int screen_y = y + OFFSET_Y + 1;  // +1 因为边框从 y=1 开始
    
    if (screen_y < 0) return;  // 超出屏幕上边界
    
    if (color > 0 && color <= 7) {
        attron(COLOR_PAIR(color));
        mvaddch(screen_y, screen_x * 2, ACS_CKBOARD);
        mvaddch(screen_y, screen_x * 2 + 1, ACS_CKBOARD);
        attroff(COLOR_PAIR(color));
    } else {
        mvaddstr(screen_y, screen_x * 2, "  ");
    }
}

void render_board_border(int offset_x, int offset_y) {
    int bx = offset_x + OFFSET_X;
    int by = offset_y + OFFSET_Y + 1;
    
    attron(COLOR_PAIR(8));
    
    // 上边框
    mvaddch(by, bx * 2 - 2, ACS_ULCORNER);
    for (int i = 0; i < BOARD_WIDTH; i++) {
        mvaddch(by, bx * 2 + i * 2, ACS_HLINE);
        mvaddch(by, bx * 2 + i * 2 + 1, ACS_HLINE);
    }
    mvaddch(by, bx * 2 + BOARD_WIDTH * 2, ACS_URCORNER);
    
    // 下边框
    mvaddch(by + BOARD_HEIGHT + 1, bx * 2 - 2, ACS_LLCORNER);
    for (int i = 0; i < BOARD_WIDTH; i++) {
        mvaddch(by + BOARD_HEIGHT + 1, bx * 2 + i * 2, ACS_HLINE);
        mvaddch(by + BOARD_HEIGHT + 1, bx * 2 + i * 2 + 1, ACS_HLINE);
    }
    mvaddch(by + BOARD_HEIGHT + 1, bx * 2 + BOARD_WIDTH * 2, ACS_LRCORNER);
    
    // 左右边框
    for (int i = 1; i <= BOARD_HEIGHT; i++) {
        if (by + i >= 0) {
            mvaddch(by + i, bx * 2 - 2, ACS_VLINE);
            mvaddch(by + i, bx * 2 + BOARD_WIDTH * 2, ACS_VLINE);
        }
    }
    
    attroff(COLOR_PAIR(8));
}

void render_tetromino(Tetromino* t, int offset_x, int offset_y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->shape[i][j]) {
                int x = t->x + j;
                int y = t->y + i;
                if (y >= 0 && y < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                    int screen_x = x + offset_x + OFFSET_X;
                    int screen_y = y + offset_y + OFFSET_Y + 1;
                    if (screen_y >= 0) {
                        attron(COLOR_PAIR(t->color));
                        mvaddch(screen_y, screen_x * 2, ACS_CKBOARD);
                        mvaddch(screen_y, screen_x * 2 + 1, ACS_CKBOARD);
                        attroff(COLOR_PAIR(t->color));
                    }
                }
            }
        }
    }
}

void render_next_piece(Tetromino* t, int offset_x, int offset_y) {
    int sx = offset_x + OFFSET_X;
    int sy = offset_y + OFFSET_Y + 1;
    
    mvprintw(sy, sx * 2, "NEXT:");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->shape[i][j]) {
                attron(COLOR_PAIR(t->color));
                mvaddch(sy + 1 + i, (sx + j) * 2, ACS_CKBOARD);
                mvaddch(sy + 1 + i, (sx + j) * 2 + 1, ACS_CKBOARD);
                attroff(COLOR_PAIR(t->color));
            }
        }
    }
}

void render_hold_piece(Tetromino* t, int offset_x, int offset_y) {
    int sx = offset_x + OFFSET_X;
    int sy = offset_y + OFFSET_Y + 1;
    
    mvprintw(sy, sx * 2, "HOLD:");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->shape[i][j]) {
                attron(COLOR_PAIR(t->color));
                mvaddch(sy + 1 + i, (sx + j) * 2, ACS_CKBOARD);
                mvaddch(sy + 1 + i, (sx + j) * 2 + 1, ACS_CKBOARD);
                attroff(COLOR_PAIR(t->color));
            }
        }
    }
}

void render_score(int score, int level, int lines, int offset_x, int offset_y) {
    int sx = offset_x + OFFSET_X;
    int sy = offset_y + OFFSET_Y + 1;
    
    mvprintw(sy, sx * 2, "SCORE:");
    mvprintw(sy + 1, sx * 2, "%d", score);
    mvprintw(sy + 3, sx * 2, "LEVEL:");
    mvprintw(sy + 4, sx * 2, "%d", level);
    mvprintw(sy + 6, sx * 2, "LINES:");
    mvprintw(sy + 7, sx * 2, "%d", lines);
}

void render_controls(int offset_x, int offset_y) {
    int sx = offset_x + OFFSET_X;
    int sy = offset_y + OFFSET_Y + 1;
    
    mvprintw(sy, sx * 2, "CONTROLS:");
    mvprintw(sy + 1, sx * 2, "<-/-> Move");
    mvprintw(sy + 2, sx * 2, "Up   Rotate");
    mvprintw(sy + 3, sx * 2, "Down Soft Drop");
    mvprintw(sy + 4, sx * 2, "Space Hard Drop");
    mvprintw(sy + 5, sx * 2, "C   Hold");
    mvprintw(sy + 6, sx * 2, "P   Pause");
    mvprintw(sy + 7, sx * 2, "Q   Quit");
}

void render_game(Game* game) {
    clear();
    
    // 绘制游戏区域边框
    render_board_border(0, 0);
    
    // 绘制已固定的方块
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (game->board.cells[y][x]) {
                int screen_x = x + OFFSET_X;
                int screen_y = y + OFFSET_Y + 1;
                if (screen_y >= 0) {
                    attron(COLOR_PAIR(game->board.cells[y][x]));
                    mvaddch(screen_y, screen_x * 2, ACS_CKBOARD);
                    mvaddch(screen_y, screen_x * 2 + 1, ACS_CKBOARD);
                    attroff(COLOR_PAIR(game->board.cells[y][x]));
                }
            }
        }
    }
    
    // 绘制当前方块
    render_tetromino(&game->current, 0, 1);
    
    // 绘制下一个方块
    render_next_piece(&game->next, BOARD_WIDTH + 2, 1);
    
    // 绘制保留方块
    render_hold_piece(&game->hold, BOARD_WIDTH + 2, 7);
    
    // 绘制分数
    render_score(game->score, game->level, game->lines_cleared, BOARD_WIDTH + 2, 13);
    
    // 绘制控制说明
    render_controls(BOARD_WIDTH + 2, 20);
    
    // 游戏结束提示
    if (game->game_over) {
        attron(A_REVERSE);
        int sy = BOARD_HEIGHT / 2 + OFFSET_Y;
        if (sy >= 0) {
            mvprintw(sy, 5 + OFFSET_X * 2, " GAME OVER ");
            mvprintw(sy + 1, 5 + OFFSET_X * 2, " Score: %d ", game->score);
            mvprintw(sy + 2, 5 + OFFSET_X * 2, " R-Restart Q-Quit ");
        }
        attroff(A_REVERSE);
    }
    
    // 暂停提示
    if (game->paused) {
        attron(A_REVERSE);
        int sy = BOARD_HEIGHT / 2 + OFFSET_Y;
        if (sy >= 0) {
            mvprintw(sy, 8 + OFFSET_X * 2, " PAUSED ");
            mvprintw(sy + 1, 8 + OFFSET_X * 2, " P-Resume ");
        }
        attroff(A_REVERSE);
    }
    
    refresh();
}
