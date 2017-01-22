#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "physics.h"

void playGame(){
  //sets up the game
  player p1;
  p1.xcor = 0;
  p1.health = START_HEALTH;
  player p2;
  p2.xcor = DISTANCE;
  p2.health = START_HEALTH;
  double s1, s2, s3, s4;
  setSeeds(&s1, &s2, &s3, &s4);
  //input stuff
  char input[10];
  char* theta;
  char velocity[10];
  //end input stuff
  while(p1.health>0&&p2.health>0){
    //1 player shoots
    //input stuff
    printf("Enter Shot:\n");
    fgets(input, 10, stdin);
    *strstr(input, "\n")=0;
    theta=input;
    strcpy(velocity,strsep(&theta, ","));
    //end input stuff
    shoot(&p1, &p2, atof(velocity), atof(theta)*M_PI/180, 1, s1, s2, s3, s4);
    //2 player shoots
    //input stuff
    printf("Enter Shot:\n");
    fgets(input, 10, stdin);
    *strstr(input, "\n")=0;
    theta=input;
    strcpy(velocity,strsep(&theta, ","));
    //end input stuff
    shoot(&p2, &p1, atof(velocity), atof(theta)*M_PI/180, -1, s1, s2, s3, s4);
  }
}

int main(){
  playGame();
  return 0;
}
