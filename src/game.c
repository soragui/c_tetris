#include "game.h"
#include "score.h"
#include <stdlib.h>
#include <time.h>

static TetrominoType random_tetromino(void) {
    return (TetrominoType)(rand() % TETRO_COUNT);
}

Game* game_create(void) {
    Game* game = malloc(sizeof(Game));
    if (!game) return NULL;
    return game;
}

void game_init(Game* game) {
    if (!game) return;
    
    srand(time(NULL));
    
    board_init(&game->board);
    tetromino_init(&game->current, random_tetromino());
    tetromino_init(&game->next, random_tetromino());
    tetromino_init(&game->hold, random_tetromino());
    
    game->has_held = false;
    game->score = 0;
    game->level = 0;
    game->lines_cleared = 0;
    game->lines_to_level = 10;
    game->drop_interval = 1000;
    game->running = true;
    game->paused = false;
    game->game_over = false;
}

void game_destroy(Game* game) {
    if (game) {
        free(game);
    }
}

void game_update(Game* game) {
    if (!game || game->paused || game->game_over) return;
    
    game->current.y++;
    
    if (board_check_collision(&game->board, &game->current)) {
        game->current.y--;
        board_lock_tetromino(&game->board, &game->current);
        
        int lines = board_clear_lines(&game->board);
        if (lines > 0) {
            score_add(&game->score, score_calculate_lines(lines, game->level));
            game->lines_cleared += lines;
            game->lines_to_level -= lines;
            
            if (game->lines_to_level <= 0) {
                game->level++;
                game->lines_to_level = 10;
                game->drop_interval = game->drop_interval * 8 / 10;
                if (game->drop_interval < 100) game->drop_interval = 100;
            }
        }
        
        game->current = game->next;
        tetromino_init(&game->next, random_tetromino());
        game->has_held = false;
        
        if (board_check_collision(&game->board, &game->current)) {
            game->game_over = true;
        }
    }
}

void game_drop(Game* game) {
    if (!game || game->paused || game->game_over) return;
    
    int drop_distance = 0;
    while (1) {
        game->current.y++;
        if (board_check_collision(&game->board, &game->current)) {
            game->current.y--;
            break;
        }
        drop_distance++;
    }
    
    score_add(&game->score, score_calculate_drop(drop_distance, false));
}

void game_hard_drop(Game* game) {
    if (!game || game->paused || game->game_over) return;
    
    int drop_distance = 0;
    while (1) {
        game->current.y++;
        if (board_check_collision(&game->board, &game->current)) {
            game->current.y--;
            break;
        }
        drop_distance++;
    }
    
    score_add(&game->score, score_calculate_drop(drop_distance, true));
}
