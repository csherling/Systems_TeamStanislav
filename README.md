# Team Stanislav - Wilson Berkow Ely Sandine Christopher Sherling
## How to compile:
+ use the make file. "make" should compile all of the files
## How to use the project:
1. Select a terminal session to be the server. On this computer after compiling
    * run $ ./server 
    * follow the instructions on whether to play two player (recommended) or single player
    * enter the number of players
    * choose a port for the first "controller" socket
2. Select another computer or terminal to be the first "controllor." 
    * run $ ./controller <ip> <port>
    * <ip> is the ip of the computer running the server function
    * <port> is the port from step 1
3. On the server terminal enter a port for the second controller, and repeat step 2, this time using the new port
4. On the server terminal enter a port for the first display
5. Select another computer or terminal to be the first "client"
    * run ./graphic-client <ip> <port>
    * <ip> is the ip of the computer running the server function
    * <port> is the port from step 5
6. On the server terminal enter a port for the second client, and repeat step 5, this time using the new port
7. Enter commands into the "controller" terminals following the prompts. As a reminder the format for inputs is "<velocity>,<angle>,<distance>" where velocity and angle are in regard to the arrow, and distance is how far you wish the player to move.

# List of files:
+ .gitignore
+ DESIGN
+ README.md
+ Replay
+ Save
+ client.c
+ constants.h
+ controller.c
+ game.c
+ game.h
+ gfx-sdl.c
+ gfx-sdl.h
+ graphic-client.c
+ makefile
+ matchctr
+ matchviewer.c
+ networking.c
+ networking.h
+ physics.c
+ physics.h
+ server.c
