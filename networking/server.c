#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

#include "networking.h"

static void sighandler(int signo){
  if(signo == SIGINT){
    removeShmem();
  }
  else if (signo == SIGUSR1) {
    printf("ppid: %d\n", getppid());
  }
}

void process( char * s );
  void sub_server( int sd, char buffer[], int sbuff, char *shmem[] );
int createShmem();
int removeShmem();

int main() {

  umask(0000);

  int consd[2];
  int conconnection[2];
  int conclient[2];
  char numplayers[10];

  int shmid = createShmem();

  char* shmem[MESSAGE_BUFFER_SIZE];

  consd[0] = 0;
  consd[1] = 0;
  conconnection[0] = 0;
  conconnection[1] = 0;
  conclient[0] = 0;
  conclient[1] = 0;

  while(1){
    printf("how many players are playing? (1/2): \n");
    fgets( numplayers, sizeof(numplayers), stdin );
    char *nt = strchr(numplayers, '\n');
    *nt = 0;
    short numplz = atoi(numplayers);
    if(numplz == 1){
      printf("Initiating 1 player mode. Please connect a controlling device.\n");
      consd[0] = server_setup(9000);
      conconnection[0] = server_connect( consd[0] );
      printf("Player controller connected!\n");
      break;
    }
    else if(numplz == 2){
      printf("Initiating 2 player mode. Please connect a controlling device.\n");
      consd[0] = server_setup(9000);
      conconnection[0] = server_connect( consd[0] );
      printf("Player 1 controller connected!\n");
      consd[1] = server_setup(9001);
      conconnection[1] = server_connect( consd[1] );
      printf("Player 2 controller connected!\n");
      break;
    }
    else{
      printf("Please Re-enter an allowed number.\n");
    }
  }//specify amount of players and initiate connections.
    
  printf("Initiating display connection\n");
  fflush(stdout);
  int sd[2];
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

  sd[0] = server_setup(9002);
  connection[0] = server_connect( sd[0] );
  printf("display 1 connected\n");
  sd[1] = server_setup(9003);
  connection[1] = server_connect( sd[1] );
  printf("display 2 connected\n");
   

  clients[0] = fork();
  if(clients[0]){
    clients[1] = fork();
  }
  
  if ( clients[0] == 0 ) {
    //      close(sd[0]);
    while(1){
      sleep(2);
      sub_server( connection[0], buffer, sizeof(buffer), shmem );
      sleep(10);
      close(sd[0]);
      exit(0);
    }
  }
  if ( clients[1] == 0 ) {      
    //      close(sd[1]);
    while(1){
      sleep(2);
      sub_server( connection[1], buffer, sizeof(buffer), shmem );
      sleep(10);
      close(sd[1]);
      exit(0);
    }
  }
  if(clients[0]){
    close( connection[0] );
    close( connection[1] );
    while (1) {
      
      read(consd[0], buffer, sizeof(buffer));
      /* printf("enter message: "); */
      /* fgets( buffer, sizeof(buffer), stdin ); */
      /* char *p = strchr(buffer, '\n'); */
      /* *p = 0; */
      strcpy(*shmem, buffer);
      
    }    
    
      /* sleep(20); */
    
      /* sleep(10); */
      /* exit(0); */
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

  return 0;
}


  void sub_server( int sd, char buffer[], int sbuff, char * shmem[] ) {
  printf("GOT HERE\n");
  fflush(stdout);
  write( sd, *shmem, sbuff);    
  printf( "received: %s\n", buffer );
  fflush(stdout);
}
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}

int createShmem(){
  int key = ftok("makefile", 22);
  int shmid;
  //Creates Shmem
  shmid = shmget(key, 4, IPC_CREAT | 0644);
  printf("shmem created %d\n", shmid);
  return shmid;
}

int removeShmem(){
  int key = ftok("makefile", 22);
  int shmid = shmget(key, 0, 0);
  struct shmid_ds d;
  shmctl(shmid, IPC_RMID, &d);
  printf("shared memory removed: %d\n", shmid);
  return shmid;
}
