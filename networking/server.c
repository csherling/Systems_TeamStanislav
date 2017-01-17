#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd, char buffer[], int sbuff );

int main() {

  int sd[0];
  int connection[2];

  sd[0] = 0;
  sd[1] = 0;
  connection[0] = 0;
  connection[1] = 0;
  
  int clients[6];//list of client pids
  int i;
  for(i = 0; i < 6; i++){
    clients[i] = 0;
  }//sets all to zero

  char buffer[MESSAGE_BUFFER_SIZE];
  for(i = 0; i < MESSAGE_BUFFER_SIZE; i++){
    buffer[i] = 0;
  }

  sd[0] = server_setup(9001);
  connection[0] = server_connect( sd[0] );
  sd[1] = server_setup(9002);
  connection[1] = server_connect( sd[1] );

    
  while (1) {
    
    printf("enter message: ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;
    
    clients[0] = fork();
    if(clients[0]){
      clients[1] = fork();
    }

    if ( clients[0] == 0 ) {
      //      close(sd[0]);
      sub_server( connection[0], buffer, sizeof(buffer) );
      sleep(10);
      exit(0);
    }
    if ( clients[0] == 0 ) {      
      //      close(sd[1]);
      sub_server( connection[1], buffer, sizeof(buffer) );
      sleep(10);
      exit(0);
    }
    else {
      sleep(10);
      close( connection[0] );
      close( connection[1] );
      sleep(10);
      exit(0);
    }
    

    /* for(i = 0; i < 6; i++){ */
    /*   printf("Client Num: %d\n", i); */
    /*   if(!clients[i]){ */
    /* 	printf("%d\n", i); */
    /* 	clients[i] = f; */
    /* 	break; */
    /*   } */
    /* } */
    /* printf("2\n"); */
    /* fflush(stdout); */
    /* if ( f == 0 ) { */
    /*   printf("3\n"); */
    /*   fflush(stdout); */
    /*   close(sd); */
    /*   printf("4\n"); */
    /*   fflush(stdout); */
    /*   if(buffer[0]){ */
    /* 	printf("%d\n", buffer[0]); */
    /* 	sub_server( connection, buffer ); */
    /*   } */
    /*   printf("5\n"); */
    /*   fflush(stdout); */
    /*   exit(0); */
    /* } */
    /* else { */
    /*   close( connection ); */
    /* } */
  }
  return 0;
}


void sub_server( int sd, char buffer[], int sbuff ) {
  printf("GOT HERE\n");
  fflush(stdout);
  write( sd, buffer, sbuff);    
  read( sd, buffer, sbuff);
  printf( "received: %s\n", buffer );
  fflush(stdout);
}
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}
