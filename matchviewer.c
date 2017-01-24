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

int main(){

  while(1){
    char *z;
    char match[10];
    int i;
    for(i = 0; i < 10; i++){
      match[i] = 0;
    }
    printf("\nmatch num?: \n");
    fgets( match, sizeof(match), stdin );
    z = strchr(match, '\n');
    *z = 0;

    int fd;
    char filnam[23];
    strcpy(filnam, "matchrecords/match");
    strcat(filnam, match);
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
  }
}
