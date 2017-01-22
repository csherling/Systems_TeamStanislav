#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define MAN_WIDTH 10
#define MAN_HEIGHT 30
#define MAN_R 0xAA
#define MAN_G 0x55
#define MAN_B 0x00

#define ARROW_LENGTH 18
#define ARROW_HEAD_ANGLE (M_PI * 0.2)
#define ARROW_HEAD_LEN 6

typedef struct {
    int x;
    int y;
} archer;

int clear(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
    SDL_RenderFillRect(renderer, NULL);
}

int draw_man(SDL_Renderer* renderer, int x, int y) {
    SDL_Rect rect;
    rect.x = x - MAN_WIDTH / 2;
    rect.y = y - MAN_HEIGHT;
    rect.w = MAN_WIDTH;
    rect.h = MAN_HEIGHT;
    SDL_SetRenderDrawColor(renderer, MAN_R, MAN_G, MAN_B, 255);
    SDL_RenderFillRect(renderer, &rect);
}

int draw_arrow(SDL_Renderer* renderer, int x, int y, double theta) {
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

int main(int argc, char* argv[]) {
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

    int quit = 0;
    SDL_Event e;
    int x = SCREEN_WIDTH / 2;
    int y = SCREEN_HEIGHT;

    printf("starting loop\n");
    while (!quit) {
        // Poll events until the event queue is empty (SDL_PollEvent returns
        // 0 when event queue is empty).
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_KEYDOWN) {
                SDL_Keysym sym = e.key.keysym;
                SDL_Keycode code = sym.sym;
                if (code == SDLK_LEFT) {
                    x -= 5;
                } else if (code == SDLK_RIGHT) {
                    x += 5;
                }
            }
        }
        clear(renderer);
        draw_man(renderer, x, y);
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        double angle_to_mouse = atan2((double) my - ((double) y - 20), mx - ((double) x - 20));
        draw_arrow(renderer, x - 20, y - 20, angle_to_mouse);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
}
