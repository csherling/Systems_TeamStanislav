#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>

#include "physics.h"
#include "networking.h"
#include "constants.h"

void process( char * s );
void sub_server( int sd, int sbuff, gamedata * shmem );
int createShmemP1();
int createShmemP2();
int removeShmemP1();
int removeShmemP2();

static void sighandler(int signo){
  if(signo == SIGINT){
    removeShmemP1();
    removeShmemP2();
  }
  else if (signo == SIGUSR1) {
    printf("ppid: %d\n", getppid());
  }
}

int main() {

  umask(0000);

  //menu

  char choice[3];
  int pick;
  char * tmpy;
  printf("Welcome to Archery!\n");
  
  while(1){
    printf("What would you like to do?\n");
    printf("(1)Start Game\n");
    printf("(2)View Match Records\n");
    printf("(3)Quit\n");
    fgets( choice, 2, stdin );

    choice[1] = 0;
    choice[2] = 0;
    pick = atoi(choice);

    if(pick > 0 && pick < 4){
      break;
    }
    else{
      printf("Please reenter your choice.\n");
    }
  }
  
  if(pick == 3){
    return 0;
  }
  else if(pick == 2){
    while(1){
      char *p;
      char match[10];
      int q;
      for(q = 0; q < 10; q++){
	match[q] = 0;
      }
      printf("\nmatch num?: \n");
      fflush(stdout);
      fflush(stdin);
      scanf("%s", match);
      /* printf("%d\n", e); */
      
      int fd;
      char filnam[23];
      strcpy(filnam, "matchrecords/match");
      strcat(filnam, match);
      printf("%s\n", filnam);
      fd = open(filnam, O_RDWR);
      
      shot s;
      int ctr = 0;

      printf("player #:  velocity  angle     distance\n");
      printf("====================================\n");
      while(read(fd,&s,sizeof(s))){
	if(ctr % 2 == 0){
	  printf("player 1");
	}
	else{
	  printf("player 2");
	}
	printf(":  %lf  %lf  %lf\n", s.velocity, s.theta, s.distance);
	ctr++;
      }
      close(fd);
      char con[2];
      con[0] = 0;
      con[1] = 0;
      printf("Would you like to continue this? (y/n)\n");
      fflush(stdout);
      scanf("%s", con);
      if(con[0] = 'n'){
	break;
      }
      
    }
  }
    
  int rctrfd = open("matchctr", O_RDWR);

  char rctr[5];
  int y;
  for(y = 0; y<5; y++){
    rctr[y] = 0;
  }
  read(rctrfd, rctr, 4);
  lseek(rctrfd, 0, SEEK_SET);
  char newrctr[5];
  for(y = 0; y<5; y++){
    newrctr[y] = 0;
  }
  snprintf(newrctr, 5, "%d", atoi(rctr) + 1);
  write(rctrfd, newrctr,sizeof(newrctr));
  close(rctrfd);

  int recordfd;
  char filnam[23];
  strcpy(filnam, "matchrecords/match");
  strcat(filnam, rctr);
  recordfd = open(filnam, O_CREAT | O_RDWR, 0644);




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
  shot shot1;
  shot shot2;
  char tmp[10];
  char tmp1[10];
  char tmp2[10];
  char tmp3[10];
  char tmp4[10];
  char tmp5[10];

  int shmid1 = createShmemP1();
  int shmid2 = createShmemP2();

  gamedata *shmem1 = shmat(shmid1, 0, 0);
  gamedata *shmem2 = shmat(shmid2, 0, 0);

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
    } else if(numplz == 2){
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

  //if ( client == 0 ) {
  //    while(1){
  //        sleep(0.01);
  //        sub_server( connection[0], sizeof(double), shmem1);
  //        sub_server( connection[0], sizeof(double), shmem2);
  //        sub_server( connection[1], sizeof(double), shmem1);
  //        sub_server( connection[1], sizeof(double), shmem2);
  //    }
  //    printf("Exiting client writer\n");
  //    fflush(stdout);
  //    close(sd[0]);
  //    close(sd[1]);
  //    exit(0);
  //}
  //RUNS GAME
  player p1;
  p1.xcor = 50;
  p1.health = START_HEALTH;
  player p2;
  p2.xcor = 350;
  p2.health = START_HEALTH;
  seed s;
  gamedata currentdata;
  setSeeds(&s);
  currentdata.terrain_seed = s;
  printf("OY WHATS GOING ON\n");
  display(0,1,s);
  sub_server( connection[0], sizeof(gamedata), shmem1);
  sub_server( connection[1], sizeof(gamedata), shmem1);
  sub_server( connection[0], sizeof(gamedata), shmem2);
  sub_server( connection[1], sizeof(gamedata), shmem2);
  while (p1.health>0&&p2.health>0) {
    sleep(1);
    read(conconnection[0], &shot1, sizeof(shot1));
    printf("GOT: %lf, %lf, %lf\n", shot1.velocity, shot1.theta, shot1.distance);
    write(recordfd,&shot1,sizeof(shot1));
    arrow arrow1 = make_arrow(shot1.velocity, shot1.theta*M_PI/180);
    move(&p1, shot1.distance);
    //shoot(&p1, &p2, &arrow1, s);
    arrow1.x = p1.xcor;
    arrow1.y = PLAYER_HEIGHT+getTerrain(p1.xcor, s);
    while(arrow1.y>=0+getTerrain(arrow1.x, s)){
      shootStep(&p1,&p2, &arrow1, s, &currentdata);
      *shmem1 = currentdata;
      sub_server( connection[0], sizeof(gamedata), shmem1);
      sub_server( connection[1], sizeof(gamedata), shmem1);
      printdata(currentdata);
      sleep(0.03);
    }
    if(signum(arrow1.vx)*(p2.xcor - arrow1.x)<0){
      overshoot(fabs(arrow1.x - p2.xcor));
    }else{
      undershoot(fabs(p2.xcor - arrow1.x));
    }
    if (conconnection[1]) {
      read(conconnection[1], &shot2, sizeof(shot2));
      printf("GOT: %lf, %lf, %lf\n", shot2.velocity, shot2.theta, shot2.distance);
      write(recordfd,&shot2,sizeof(shot2));
      arrow arrow2 = make_arrow(shot2.velocity, (180 - shot2.theta) *M_PI/180);
      move(&p2, -shot2.distance);
      //shoot(&p2, &p2, &arrow2, s);
      arrow2.x = p2.xcor;
      arrow2.y = PLAYER_HEIGHT+getTerrain(p2.xcor, s);
      while(arrow2.y>=0+getTerrain(arrow2.x, s)){
	shootStep(&p2,&p1, &arrow2, s, &currentdata);
	*shmem2 = currentdata;
	sub_server( connection[0], sizeof(gamedata), shmem2);
	sub_server( connection[1], sizeof(gamedata), shmem2);
	sleep(0.03);
      }
      if(signum(arrow2.vx)*(p2.xcor - arrow2.x)<0){
	overshoot(fabs(arrow2.x - p1.xcor));
      }else{
	undershoot(fabs(p1.xcor - arrow2.x));
      }      
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


void sub_server( int sd, int sbuff, gamedata * shmem ) {
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
  shmid = shmget(key, sizeof(gamedata), IPC_CREAT | 0644);
  printf("shmem created %d\n", shmid);
  return shmid;
}

int createShmemP2(){
  int key = ftok("makefile", 44);
  int shmid;
  //Creates Shmem
  shmid = shmget(key, sizeof(gamedata), IPC_CREAT | 0644);
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
