#include "render.h"
#include <ncurses.h>
#include <stdio.h>

#define OFFSET_X 8
#define OFFSET_Y -1

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
        init_pair(1, COLOR_CYAN, -1);
        init_pair(2, COLOR_YELLOW, -1);
        init_pair(3, COLOR_MAGENTA, -1);
        init_pair(4, COLOR_GREEN, -1);
        init_pair(5, COLOR_RED, -1);
        init_pair(6, COLOR_BLUE, -1);
        init_pair(7, COLOR_WHITE, -1);
        init_pair(8, COLOR_WHITE, -1);
        init_pair(9, COLOR_BLACK, COLOR_WHITE);
    }
}

void render_cleanup(void) {
    endwin();
}

void render_board_border(int offset_x, int offset_y) {
    int bx = offset_x + OFFSET_X;
    int by = offset_y + OFFSET_Y + 1;
    
    attron(COLOR_PAIR(8) | A_BOLD);
    
    // 标题
    mvprintw(by - 1, bx * 2 + 3, "╔═══ TETRIS ═══╗");
    
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
    
    attroff(COLOR_PAIR(8) | A_BOLD);
}

void render_ghost(Tetromino* t, int offset_x, int offset_y) {
    attron(COLOR_PAIR(8));
    attron(A_DIM);
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->shape[i][j]) {
                int x = t->x + j;
                int y = t->y + i;
                if (y >= 0 && y < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                    int screen_x = x + offset_x + OFFSET_X;
                    int screen_y = y + offset_y + OFFSET_Y + 1;
                    if (screen_y >= 0) {
                        mvaddstr(screen_y, screen_x * 2, "░░");
                    }
                }
            }
        }
    }
    
    attroff(A_DIM);
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

void render_board(Board* board) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board->cells[y][x]) {
                int screen_x = x + OFFSET_X;
                int screen_y = y + OFFSET_Y + 1;
                if (screen_y >= 0) {
                    attron(COLOR_PAIR(board->cells[y][x]));
                    mvaddch(screen_y, screen_x * 2, ACS_CKBOARD);
                    mvaddch(screen_y, screen_x * 2 + 1, ACS_CKBOARD);
                    attroff(COLOR_PAIR(board->cells[y][x]));
                }
            }
        }
    }
}

void render_next_piece(Tetromino* t, int offset_x, int offset_y) {
    int sx = offset_x + OFFSET_X;
    int sy = offset_y + OFFSET_Y + 1;
    
    attron(A_BOLD);
    mvprintw(sy, sx * 2, "NEXT:");
    attroff(A_BOLD);
    
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
    
    attron(A_BOLD);
    mvprintw(sy, sx * 2, "HOLD:");
    attroff(A_BOLD);
    
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
    
    attron(A_BOLD);
    mvprintw(sy, sx * 2, "SCORE:");
    attroff(A_BOLD);
    mvprintw(sy + 1, sx * 2, "%d", score);
    
    mvprintw(sy + 3, sx * 2, "LEVEL:");
    mvprintw(sy + 4, sx * 2, "%d", level);
    
    mvprintw(sy + 6, sx * 2, "LINES:");
    mvprintw(sy + 7, sx * 2, "%d", lines);
}

void render_controls(int offset_x, int offset_y) {
    int sx = offset_x + OFFSET_X;
    int sy = offset_y + OFFSET_Y + 1;
    
    attron(A_BOLD);
    mvprintw(sy, sx * 2, "CONTROLS:");
    attroff(A_BOLD);
    mvprintw(sy + 1, sx * 2, "<-/-> Move");
    mvprintw(sy + 2, sx * 2, "Up   Rotate");
    mvprintw(sy + 3, sx * 2, "Down Soft");
    mvprintw(sy + 4, sx * 2, "Space Hard");
    mvprintw(sy + 5, sx * 2, "C   Hold");
    mvprintw(sy + 6, sx * 2, "P   Pause");
    mvprintw(sy + 7, sx * 2, "Q   Quit");
}

void render_game(Game* game) {
    clear();
    
    // 绘制边框
    render_board_border(0, 0);
    
    // 绘制影子方块（在最底层）
    render_ghost(&game->ghost, 0, 1);
    
    // 绘制已固定的方块
    render_board(&game->board);
    
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
        attron(A_REVERSE | A_BOLD);
        int sy = BOARD_HEIGHT / 2 + OFFSET_Y;
        if (sy >= 0) {
            mvprintw(sy, 3 + OFFSET_X * 2, "╔══ GAME OVER ══╗");
            mvprintw(sy + 1, 3 + OFFSET_X * 2, "║ Score: %-6d ║", game->score);
            mvprintw(sy + 2, 3 + OFFSET_X * 2, "║ R-Restart    ║");
            mvprintw(sy + 3, 3 + OFFSET_X * 2, "╚══════════════╝");
        }
        attroff(A_REVERSE | A_BOLD);
    }
    
    // 暂停提示
    if (game->paused) {
        attron(A_REVERSE | A_BOLD);
        int sy = BOARD_HEIGHT / 2 + OFFSET_Y;
        if (sy >= 0) {
            mvprintw(sy, 6 + OFFSET_X * 2, "╔═══ PAUSED ═══╗");
            mvprintw(sy + 1, 6 + OFFSET_X * 2, "║ P-Resume    ║");
            mvprintw(sy + 2, 6 + OFFSET_X * 2, "╚═════════════╝");
        }
        attroff(A_REVERSE | A_BOLD);
    }
    
    refresh();
}
