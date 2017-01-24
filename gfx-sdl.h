#ifndef GFX_SDL_H

#include "physics.h"

#define GFX_SDL_H

#define MAN_WIDTH 10
#define MAN_HEIGHT 30
#define MAN_R 0xAA
#define MAN_G 0x55
#define MAN_B 0x00

#define ARROW_LENGTH 18
#define ARROW_HEAD_ANGLE (M_PI * 0.2)
#define ARROW_HEAD_LEN 6

#endif

int clear(SDL_Renderer* renderer);

void draw_man(SDL_Renderer* renderer, int x, int y);
void draw_arrow(SDL_Renderer* renderer, int x, int y, double theta);

void draw_terrain(SDL_Renderer* renderer, seed s);
SDL_Renderer* init_SDL2(SDL_Window **window);
void end_SDL2(SDL_Window* window);
void update_SDL2(SDL_Window* window, SDL_Renderer* renderer);
int test_terrain();
