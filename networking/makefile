all: server client controller

server: server.o networking.o
	gcc -o server server.o networking.o

client: client.o networking.o
	gcc -o client client.o networking.o

controller: controller.o networking.o
	gcc -o controller client.o networking.o

server.o: server.c networking.h
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c

controller.o: controller.c networking.h
	gcc -c controller.c


networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~
