#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd, char buffer[] );

int main() {

  int sd, connection;

  sd = server_setup();

  int clients[6];//list of client pids
  int i;
  for(i = 0; i < 6; i++){
    clients[i] = 0;
  }//sets all to zero
  
  while (1) {

    connection = server_connect( sd );

    char buffer[MESSAGE_BUFFER_SIZE];
    printf("enter message: ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;
  
    int f = fork();
    for(i = 0; i < 6; i++){
      if(!clients[i]){
	clients[i] = f;
	break;
      }
    }
    printf("got here");
    if ( f == 0 ) {
    printf("got here");
      close(sd);
      sub_server( connection, buffer );

      exit(0);
    }
    else {
      close( connection );
    }
  }
  return 0;
}


void sub_server( int sd, char buffer[] ) {

  while (read( sd, buffer, sizeof(buffer) )) {
    write( sd, buffer, sizeof(buffer));    
  }
  
}
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}
