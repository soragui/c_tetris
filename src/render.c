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
        use_default_colors();
        init_pair(1, COLOR_CYAN, -1);    // I
        init_pair(2, COLOR_YELLOW, -1);  // O
        init_pair(3, COLOR_MAGENTA, -1); // T
        init_pair(4, COLOR_GREEN, -1);   // S
        init_pair(5, COLOR_RED, -1);     // Z
        init_pair(6, COLOR_BLUE, -1);    // J
        init_pair(7, COLOR_WHITE, -1);   // L
        init_pair(8, COLOR_WHITE, -1);   // 边框
        init_pair(9, COLOR_BLACK, COLOR_WHITE); // 游戏结束背景
    }
}

void render_cleanup(void) {
    endwin();
}

void render_block(int x, int y, int color) {
    if (y < 0 || y >= BOARD_HEIGHT) return;
    
    if (color > 0 && color <= 7) {
        attron(COLOR_PAIR(color));
        mvaddch(y, x * 2, ACS_CKBOARD);
        mvaddch(y, x * 2 + 1, ACS_CKBOARD);
        attroff(COLOR_PAIR(color));
    } else {
        mvaddstr(y, x * 2, "  ");
    }
}

void render_board_border(int offset_x, int offset_y) {
    attron(COLOR_PAIR(8));
    
    // 上边框
    mvaddch(offset_y, offset_x * 2 - 2, ACS_ULCORNER);
    for (int i = 0; i < BOARD_WIDTH; i++) {
        mvaddch(offset_y, offset_x * 2 + i * 2, ACS_HLINE);
        mvaddch(offset_y, offset_x * 2 + i * 2 + 1, ACS_HLINE);
    }
    mvaddch(offset_y, offset_x * 2 + BOARD_WIDTH * 2, ACS_URCORNER);
    
    // 下边框
    mvaddch(offset_y + BOARD_HEIGHT + 1, offset_x * 2 - 2, ACS_LLCORNER);
    for (int i = 0; i < BOARD_WIDTH; i++) {
        mvaddch(offset_y + BOARD_HEIGHT + 1, offset_x * 2 + i * 2, ACS_HLINE);
        mvaddch(offset_y + BOARD_HEIGHT + 1, offset_x * 2 + i * 2 + 1, ACS_HLINE);
    }
    mvaddch(offset_y + BOARD_HEIGHT + 1, offset_x * 2 + BOARD_WIDTH * 2, ACS_LRCORNER);
    
    // 左右边框
    for (int i = 1; i <= BOARD_HEIGHT; i++) {
        mvaddch(offset_y + i, offset_x * 2 - 2, ACS_VLINE);
        mvaddch(offset_y + i, offset_x * 2 + BOARD_WIDTH * 2, ACS_VLINE);
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
                    attron(COLOR_PAIR(t->color));
                    mvaddch(y + offset_y, (x + offset_x) * 2, ACS_CKBOARD);
                    mvaddch(y + offset_y, (x + offset_x) * 2 + 1, ACS_CKBOARD);
                    attroff(COLOR_PAIR(t->color));
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
                mvaddch(offset_y + 1 + i, (offset_x + j) * 2, ACS_CKBOARD);
                mvaddch(offset_y + 1 + i, (offset_x + j) * 2 + 1, ACS_CKBOARD);
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
                mvaddch(offset_y + 1 + i, (offset_x + j) * 2, ACS_CKBOARD);
                mvaddch(offset_y + 1 + i, (offset_x + j) * 2 + 1, ACS_CKBOARD);
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
    mvprintw(offset_y + 1, offset_x * 2, "<-/-> Move");
    mvprintw(offset_y + 2, offset_x * 2, "Up   Rotate");
    mvprintw(offset_y + 3, offset_x * 2, "Down Soft Drop");
    mvprintw(offset_y + 4, offset_x * 2, "Space Hard Drop");
    mvprintw(offset_y + 5, offset_x * 2, "C   Hold");
    mvprintw(offset_y + 6, offset_x * 2, "P   Pause");
    mvprintw(offset_y + 7, offset_x * 2, "Q   Quit");
}

void render_game(Game* game) {
    clear();
    
    // 绘制游戏区域边框
    render_board_border(0, 0);
    
    // 绘制已固定的方块
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (game->board.cells[y][x]) {
                attron(COLOR_PAIR(game->board.cells[y][x]));
                mvaddch(y + 1, x * 2, ACS_CKBOARD);
                mvaddch(y + 1, x * 2 + 1, ACS_CKBOARD);
                attroff(COLOR_PAIR(game->board.cells[y][x]));
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
        mvprintw(BOARD_HEIGHT / 2, 5, " GAME OVER ");
        mvprintw(BOARD_HEIGHT / 2 + 1, 5, " Score: %d ", game->score);
        mvprintw(BOARD_HEIGHT / 2 + 2, 5, " R-Restart Q-Quit ");
        attroff(A_REVERSE);
    }
    
    // 暂停提示
    if (game->paused) {
        attron(A_REVERSE);
        mvprintw(BOARD_HEIGHT / 2, 8, " PAUSED ");
        mvprintw(BOARD_HEIGHT / 2 + 1, 8, " P-Resume ");
        attroff(A_REVERSE);
    }
    
    refresh();
}
