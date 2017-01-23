SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CC := gcc

game: clean game.c
	$(CC) physics.c game.c -o game -lm

physics: clean physics.c
	$(CC) physics.c physics.o -lm

gfx: clean gfx-sdl.c
	$(CC) physics.c gfx-sdl.c $(SDL_CFLAGS) $(SDL_LDFLAGS) -lm -o gfx

clean:
	rm -f game
	rm -f gfx-sdl
	rm -f a.out
