#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

int main( int argc, char *argv[] ) {

  char *host;
  /* if (argc != 2 ) { */
  /*   printf("host not specified, conneting to 127.0.0.1\n"); */
  /*   host = "127.0.0.1"; */
  /* } */
  /* else */
  printf("memes\n");
  fflush(stdout);
  host = argv[1];
  
  printf("memes\n");
  fflush(stdout);
  int sd;

  printf("memes\n");
  fflush(stdout);
  sd = client_connect( host, atoi(argv[2]) );

  printf("memes\n");
  fflush(stdout);
  char buffer[MESSAGE_BUFFER_SIZE];
  
  printf("memes\n");
  fflush(stdout);
  
  int msgsize = 0;
  int i = 0;

  while (1) {
    printf("enter message: ");
    fflush(stdout);
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;
    /* for(msgsize = 0; buffer[msgsize]; msgsize++); */
    write( sd, buffer, 1 );
    printf("Sent: %s\n", buffer);
    /* for(i = 0; i < MESSAGE_BUFFER_SIZE; i++){ */
    /*   buffer[i] = 0; */
    /* } */
  }
  
  return 0;
}