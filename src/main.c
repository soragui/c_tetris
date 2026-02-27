#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "game.h"
#include "render.h"
#include "input.h"

int main(void) {
    render_init();
    
    Game* game = game_create();
    if (!game) {
        render_cleanup();
        fprintf(stderr, "Failed to create game!\n");
        return 1;
    }
    
    game_init(game);
    
    while (game->running) {
        int key = input_get_key();
        if (key != ERR) {
            input_handle_game(game, key);
        }
        
        game_update(game);
        game_render(game);
        
        napms(game->paused ? 100 : 50);
    }
    
    game_destroy(game);
    render_cleanup();
    
    printf("Thanks for playing Tetris!\n");
    return 0;
}
