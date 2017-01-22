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
  seed s;
  setSeeds(&s);
  display(1,3,s);
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
    arrow arrow1 = make_arrow(atof(velocity), atof(theta)*M_PI/180);
    shoot(&p1, &p2, &arrow1, s);
    //2 player shoots
    //input stuff
    printf("Enter Shot:\n");
    fgets(input, 10, stdin);
    *strstr(input, "\n")=0;
    theta=input;
    strcpy(velocity,strsep(&theta, ","));
    //end input stuff
    arrow arrow2 = make_arrow(atof(velocity), (180 - atof(theta)) *M_PI/180);
    shoot(&p2, &p1, &arrow2, s);
  }
}

int main(){
  playGame();
  return 0;
}
