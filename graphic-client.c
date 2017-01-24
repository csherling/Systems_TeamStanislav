#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <SDL.h>
//#include <SDL_keyboard.h>
//#include <SDL_keycode.h>

#include "constants.h"
#include "gfx-sdl.h"
#include "networking.h"
#include "physics.h"

int main( int argc, char *argv[] ) {

    char *host;
    host = argv[1];

    int sd;

    sd = client_connect( host, atoi(argv[2]) );

    gamedata buffer;

    SDL_Window* window;
    SDL_Renderer* renderer = init_SDL2(&window);
    SDL_Event e;

    int quit = 0;
    while(!quit){
        while(1) {
            if (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = 1;
                }
                break;
            }
            if (read( sd, &buffer, sizeof(buffer))){
                printf("recieved game update\n");
                sleep(0.5);
            }
        }
        clear(renderer);
        draw_terrain(renderer, buffer.terrain_seed);
        double p1_y;
        draw_man(renderer, buffer.player1.xcor, p1_y);
        double p2_y;
        draw_man(renderer, buffer.player2.xcor, p2_y);
        draw_arrow(renderer, buffer.ar.x, buffer.ar.y, atan2(buffer.ar.vy, buffer.ar.vx));
        // Update screen
        update_SDL2(window, renderer);
    }

    end_SDL2(window);

    return 0;
}
