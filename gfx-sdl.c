#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>

#include "constants.h"
#include "physics.h"
#include "gfx-sdl.h"

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

void draw_terrain(SDL_Renderer* renderer, seed s) {
    SDL_SetRenderDrawColor(renderer, MAN_R, MAN_G, MAN_B, 255);
    int prevX = 0;
    int prevY = 0;
    int x;
    for (x = 0; x < SCREEN_WIDTH; x+=1) {
        int y = (int) getTerrain(((double) x) * 0.1, s) / 10 + 40;
        y = fmax(0.0, y);
        printf("Terrain X,Y: %d, %d\n", x, y);
        SDL_RenderDrawLine(renderer, prevX, SCREEN_HEIGHT - prevY, x, SCREEN_HEIGHT - y);
        prevX = x;
        prevY = y;
    }
}

SDL_Renderer* init_SDL2(SDL_Window **window) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ERROR: SDL_Init failed: SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Create window
    *window =
        SDL_CreateWindow(
                "Archery in an Hour(TM)",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN);
    if( *window == NULL ) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(*window, -1, 0);

    // Fill the window white
    clear(renderer);
    SDL_UpdateWindowSurface(*window);

    return renderer;
}

void end_SDL2(SDL_Window* window) {
    // Destroy window
    SDL_DestroyWindow(window);
    // Quit SDL subsystems
    SDL_Quit();
}

void update_SDL2(SDL_Window* window, SDL_Renderer* renderer) {
    // Update screen
    SDL_RenderPresent(renderer);
    // Update window to screen surface
    SDL_UpdateWindowSurface(window);
}

int test_terrain() {
    SDL_Window* window;
    SDL_Renderer* renderer = init_SDL2(&window);

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

    draw_terrain(renderer, s);

    // Update screen
    update_SDL2(window, renderer);

    int shot = 0;
    int quit = 0;
    // Wait for user to click X button to quit
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    end_SDL2(window);
}
