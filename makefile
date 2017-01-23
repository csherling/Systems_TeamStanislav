SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CC := gcc
all: server client controller game

server: server.o networking.o
	gcc -o server server.o networking.o

client: client.o networking.o
	gcc -o client client.o networking.o

controller: controller.o networking.o
	gcc -o controller controller.o networking.o

server.o: server.c networking.h
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c

controller.o: controller.c networking.h
	gcc -c controller.c

networking.o: networking.c networking.h
	gcc -c networking.c

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
	rm *.o
	rm *~

