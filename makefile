SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)
CC := gcc
all: server client controller game graphic-client

server: server.o networking.o physics.o
	gcc -o server server.o networking.o physics.o -lm

client: client.o networking.o
	gcc -o client client.o networking.o

graphic-client: networking.o physics.o gfx-sdl.o graphic-client.o
	gcc -o graphic-client networking.o physics.o gfx-sdl.o graphic-client.o $(SDL_CFLAGS) $(SDL_LDFLAGS) -lm

controller: controller.o networking.o
	gcc -o controller controller.o networking.o

server.o: server.c networking.h
	gcc -c server.c

graphic-client.o: client.c networking.h
	gcc -c graphic-client.c $(SDL_CFLAGS) $(SDL_LDFLAGS) -lm

controller.o: controller.c networking.h
	gcc -c controller.c

networking.o: networking.c networking.h
	gcc -c networking.c

game: game.c
	$(CC) physics.c game.c -o game -lm

physics.o: physics.c
	$(CC) -o physics.o -c physics.c $(SDL_CFLAGS) $(SDL_LDFLAGS) -lm

gfx-sdl.o: gfx-sdl.c
	$(CC) -c gfx-sdl.c $(SDL_CFLAGS) $(SDL_LDFLAGS) -lm

clean:
	rm -f game
	rm -f gfx-sdl
	rm -f a.out
	rm -f *.o
	rm -f *~

