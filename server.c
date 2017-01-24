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
    removeShmemP1();
    removeShmemP2();
  }
  else if (signo == SIGUSR1) {
    printf("ppid: %d\n", getppid());
  }
}

void process( char * s );
void sub_server( int sd, int sbuff, double * shmem );
int createShmemP1();
int createShmemP2();
int removeShmemP1();
int removeShmemP2();

int main() {

  umask(0000);

  int consd[2];
  int conconnection[2];
  int conclient;
  char numplayers[10];
  char p1port[6];
  char p2port[6];
  char disp1port[6];
  char disp2port[6];
  char disp3port[6];
  char disp4port[6];
  char disp5port[6];
  char disp6port[6];
  char *z;
  shot p1;
  shot p2;
  char tmp[10];
  char tmp1[10];
  char tmp2[10];
  char tmp3[10];
  char tmp4[10];
  char tmp5[10];

  int shmid1 = createShmemP1();
  int shmid2 = createShmemP2();

  double *shmem1 = shmat(shmid1, 0, 0);
  *shmem1 = 1.0;

  double *shmem2 = shmat(shmid2, 0, 0);
  *shmem2 = 2.0;

  consd[0] = 0;
  consd[1] = 0;
  conconnection[0] = 0;
  conconnection[1] = 0;
  conclient = 1;

  while(1){
    printf("how many players are playing? (1/2): \n");
    fgets( numplayers, sizeof(numplayers), stdin );
    z = strchr(numplayers, '\n');
    *z = 0;
    short numplz = atoi(numplayers);
    if(numplz == 1){
      printf("Initiating 1 player mode. Please connect a controlling device.\n");
      printf("Specify P1 Port: \n");
      fgets( p1port, sizeof(p1port), stdin );
      z = strchr(p1port, '\n');
      *z = 0;
      int port1 = atoi(p1port);
      consd[0] = server_setup(port1);
      conconnection[0] = server_connect( consd[0] );
      printf("Player controller connected!\n");
      break;
    }
    else if(numplz == 2){
      printf("Initiating 2 player mode. Please connect a controlling device.\n");
      printf("Specify P1 Port: \n");
      fgets( p1port, sizeof(p1port), stdin );
      z = strchr(p1port, '\n');
      *z = 0;
      int port1 = atoi(p1port);
      printf("before connection\n");
      fflush(stdout);
      consd[0] = server_setup(port1);
      printf("after connection\n");
      fflush(stdout);
      conconnection[0] = server_connect( consd[0] );
      printf("Player 1 controller connected!\n");
      printf("Specify P2 Port: \n");
      fgets( p2port, sizeof(p2port), stdin );
      z = strchr(p2port, '\n');
      *z = 0;
      int port2 = atoi(p2port);
      consd[1] = server_setup(port2);
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

  int client = 1;//list of client pids

  char buffer[MESSAGE_BUFFER_SIZE];
  int i;
  for(i = 0; i < MESSAGE_BUFFER_SIZE; i++){
    buffer[i] = 0;
  }

  printf("Specify disp1 Port: \n");
  fgets( disp1port, sizeof(disp1port), stdin );
  z = strchr(disp1port, '\n');
  *z = 0;
  int dport1 = atoi(disp1port);
  sd[0] = server_setup(dport1);
  connection[0] = server_connect( sd[0] );
  printf("display 1 connected\n");
  printf("Specify disp2 Port: \n");
  fgets( disp2port, sizeof(disp2port), stdin );
  z = strchr(disp2port, '\n');
  *z = 0;
  int dport2 = atoi(disp2port);
  sd[1] = server_setup(dport2);
  connection[1] = server_connect( sd[1] );
  printf("display 2 connected\n");


  client = fork();
  /* if(client){ */
  /*   conclient = fork(); */
  /* } */

  if ( client == 0 ) {
    while(1){
      sleep(1);
      sub_server( connection[0], sizeof(double), shmem1);
      sub_server( connection[0], sizeof(double), shmem2);
      sub_server( connection[1], sizeof(double), shmem1);
      sub_server( connection[1], sizeof(double), shmem2);
    }
    printf("Exiting client writer\n");
    fflush(stdout);
    close(sd[0]);
    close(sd[1]);
    exit(0);
  }
    while (1) {
      sleep(1);
      read(conconnection[0], &p1, sizeof(p1));
      *shmem1 = p1.velocity;
      printf("GOT: %lf, %lf, %lf\n", p1.velocity, p1.theta, p1.distance);
      if (conconnection[1]) {
          read(conconnection[1], &p2, sizeof(p2));
          *shmem2 = p2.velocity;
          printf("GOT: %lf, %lf, %lf\n", p2.velocity, p2.theta, p2.distance);
      }
    }
    printf("Exiting controller reader\n");
    fflush(stdout);
    close(consd[0]);
    close(consd[1]);
    exit(0);
/* } */

  return 0;
}


void sub_server( int sd, int sbuff, double * shmem ) {
  printf("GOT HERE\n");
  printf("%lf\n", *shmem);
  fflush(stdout);
  write( sd, shmem, sbuff);
}
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}

int createShmemP1(){
  int key = ftok("makefile", 22);
  int shmid;
  //Creates Shmem
  shmid = shmget(key, 4, IPC_CREAT | 0644);
  printf("shmem created %d\n", shmid);
  return shmid;
}

int createShmemP2(){
  int key = ftok("makefile", 44);
  int shmid;
  //Creates Shmem
  shmid = shmget(key, 4, IPC_CREAT | 0644);
  printf("shmem created %d\n", shmid);
  return shmid;
}

int removeShmemP1(){
  int key = ftok("makefile", 22);
  int shmid = shmget(key, 0, 0);
  struct shmid_ds d;
  shmctl(shmid, IPC_RMID, &d);
  printf("shared memory removed: %d\n", shmid);
  return shmid;
}

int removeShmemP2(){
  int key = ftok("makefile", 44);
  int shmid = shmget(key, 0, 0);
  struct shmid_ds d;
  shmctl(shmid, IPC_RMID, &d);
  printf("shared memory removed: %d\n", shmid);
  return shmid;
}
