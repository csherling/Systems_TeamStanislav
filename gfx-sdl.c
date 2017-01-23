#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>

#include "physics.h"
#include "gfx-sdl.h"

typedef struct {
    int x;
    int y;
} archer;

int clear(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
    SDL_RenderFillRect(renderer, NULL);
}

void draw_man(SDL_Renderer* renderer, int x, int y) {
    SDL_Rect rect;
    rect.x = x - MAN_WIDTH / 2;
    rect.y = y - MAN_HEIGHT;
    rect.w = MAN_WIDTH;
    rect.h = MAN_HEIGHT;
    SDL_SetRenderDrawColor(renderer, MAN_R, MAN_G, MAN_B, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_arrow(SDL_Renderer* renderer, int x, int y, double theta) {
    // theta is relative to horizontal, clockwise is positive
    int xf = x + (int) (cos(theta) * ARROW_LENGTH);
    int yf = y + (int) (sin(theta) * ARROW_LENGTH);
    SDL_RenderDrawLine(renderer, x, y, xf, yf);
    // head_dx and head_dy are the x and y deltas form the arrow's head
    // to the end of the first line coming off of it.
    int head_dx = (int) (cos(theta - ARROW_HEAD_ANGLE) * ARROW_HEAD_LEN);
    int head_dy = (int) (sin(theta - ARROW_HEAD_ANGLE) * ARROW_HEAD_LEN);
    SDL_RenderDrawLine(renderer, xf, yf, xf - head_dx, yf - head_dy);
    // set head_dx and head_dy to deltas for other line coming off of arrow's head
    head_dx = (int) (cos(theta + ARROW_HEAD_ANGLE) * ARROW_HEAD_LEN);
    head_dy = (int) (sin(theta + ARROW_HEAD_ANGLE) * ARROW_HEAD_LEN);
    SDL_RenderDrawLine(renderer, xf, yf, xf - head_dx, yf - head_dy);
}

void draw_terrain_1(SDL_Renderer* renderer, int* terrain, int width) {
    SDL_SetRenderDrawColor(renderer, MAN_R, MAN_G, MAN_B, 255);
    int prevX = 0;
    int prevY = 0;
    int x;
    for (x = 0; x < width; x+=1) {
        //int y = (int) getTerrain((double) x, s);
        int y = terrain[x];
        printf("Terrain X,Y: %d, %d\n", x, y);
        SDL_RenderDrawLine(renderer, prevX, SCREEN_HEIGHT - prevY, x, SCREEN_HEIGHT - y);
        prevX = x;
        prevY = y;
    }
}

void draw_terrain_2(SDL_Renderer* renderer, seed s) {
    SDL_SetRenderDrawColor(renderer, MAN_R, MAN_G, MAN_B, 255);
    int prevX = 0;
    int prevY = 0;
    int x;
    for (x = 0; x < SCREEN_WIDTH; x+=1) {
        int y = (int) getTerrain((double) x, s);
        printf("Terrain X,Y: %d, %d\n", x, y);
        SDL_RenderDrawLine(renderer, prevX, SCREEN_HEIGHT - prevY, x, SCREEN_HEIGHT - y);
        prevX = x;
        prevY = y;
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    printf("atan2(1, 0) = %f.1\n", atan2(1, 0));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ERROR: SDL_Init failed: SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    //Create window
    SDL_Window* window =
        SDL_CreateWindow(
                "Archery in an Hour(TM)",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN);
    if( window == NULL ) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }

    // Get window surface
    //SDL_Surface* screen = SDL_GetWindowSurface(window);

    printf("about to create renderer\n");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    printf("created renderer\n");

    // Fill the window white
    clear(renderer);
    printf("cleared screen\n");
    SDL_UpdateWindowSurface(window);

    SDL_Event e;
    int x = SCREEN_WIDTH / 2;
    int y = SCREEN_HEIGHT;

    player jim;
    jim.xcor = 0;
    jim.health = START_HEALTH;
    player bob;
    bob.xcor = DISTANCE;
    bob.health = START_HEALTH;
    arrow ar;
    ar.x = x;
    ar.y = y;
    seed s;
    setSeeds(&s);

    int shot = 0;

    int quit = 0;

    // Draw the terrain (random slope method)
    int terrain[SCREEN_WIDTH];
    genTerrain(terrain, SCREEN_WIDTH);
    draw_terrain_1(renderer, terrain, SCREEN_WIDTH);
    // Draw the terrain (random trig method)
    //draw_terrain_2(renderer, s);

    // Update screen
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
    // Wait for user to click X button to quit
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    //printf("starting loop\n");
    //while (!quit) {
    //    int mx, my;
    //    SDL_GetMouseState(&mx, &my);
    //    double ar_angle = atan2(ar.vy, ar.vx);
    //    // Poll events until the event queue is empty (SDL_PollEvent returns
    //    // 0 when event queue is empty).
    //    while (SDL_PollEvent(&e) != 0) {
    //        if (e.type == SDL_QUIT) {
    //            quit = 1;
    //        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
    //            shot = 1;
    //            ar = make_arrow(0.2, atan2(my, mx));
    //        } else if (e.type == SDL_KEYDOWN) {
    //            SDL_Keysym sym = e.key.keysym;
    //            SDL_Keycode code = sym.sym;
    //            if (code == SDLK_LEFT) {
    //                x -= 5;
    //            } else if (code == SDLK_RIGHT) {
    //                x += 5;
    //            }
    //        }
    //    }
    //    if (shot) {
    //        printf("%f.1\n", ar.vx);
    //        shootStep(&bob, &ar, s);
    //    }
    //    clear(renderer);
    //    draw_man(renderer, x, y);
    //    draw_arrow(renderer, ar.x, ar.y, ar_angle);
    //    SDL_RenderPresent(renderer);
    //    SDL_UpdateWindowSurface(window);
    //}

    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
}
