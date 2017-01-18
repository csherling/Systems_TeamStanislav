#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define MAN_WIDTH 10
#define MAN_HEIGHT 30
#define MAN_R 0xAA
#define MAN_G 0x55
#define MAN_B 0x00

int draw_man(SDL_Surface* screen, int x, int y) {
    SDL_Rect rect;
    rect.x = x - MAN_WIDTH / 2;
    rect.y = y - MAN_HEIGHT;
    rect.w = MAN_WIDTH;
    rect.h = MAN_HEIGHT;
    SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, MAN_R, MAN_G, MAN_B));
}

int main(int argc, char* argv[]) {
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
    SDL_Surface* screen = SDL_GetWindowSurface(window);

    //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Fill the surface white
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);

    int quit = 0;
    SDL_Event e;
    int x = SCREEN_WIDTH / 2;
    int y = SCREEN_HEIGHT;
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
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
        draw_man(screen, x, y);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
}
