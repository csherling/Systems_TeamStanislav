SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CC := gcc

game: game.c
	$(CC) physics.c game.c -o game -lm

physics: physics.c
	$(CC) physics.c physics.o -lm

gfx-sdl: gfx-sdl.c
	$(CC) gfx-sdl.c $(SDL_CFLAGS) $(SDL_LDFLAGS) -lm
