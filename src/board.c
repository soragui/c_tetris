#include "board.h"
#include <string.h>

void board_init(Board* board) {
    memset(board->cells, 0, sizeof(board->cells));
    memset(board->filled_rows, 0, sizeof(board->filled_rows));
}

void board_lock_tetromino(Board* board, Tetromino* t) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (t->shape[i][j]) {
                int x = t->x + j;
                int y = t->y + i;
                if (y >= 0 && y < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH) {
                    board->cells[y][x] = t->color;
                }
            }
        }
    }
}

int board_clear_lines(Board* board) {
    int cleared = 0;
    
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool full = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (!board->cells[y][x]) {
                full = false;
                break;
            }
        }
        
        if (full) {
            cleared++;
            for (int yy = y; yy > 0; yy--) {
                memcpy(board->cells[yy], board->cells[yy-1], sizeof(board->cells[0]));
            }
            memset(board->cells[0], 0, sizeof(board->cells[0]));
            y++;
        }
    }
    
    return cleared;
}

bool board_check_collision(Board* board, Tetromino* t) {
    return tetromino_check_collision(t, board->cells);
}
