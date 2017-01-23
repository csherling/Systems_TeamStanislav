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
  //To be adjusted with multiple computers
  display(0,10,s);
  //input stuff
  char input[10];
  char* distance;
  char velocity[10];
  char theta[10];
  //end input stuff
  while(p1.health>0&&p2.health>0){
    //1 player shoots
    //input stuff
    printf("Enter Shot:\n");
    fgets(input, 10, stdin);
    *strstr(input, "\n")=0;
    distance=input;
    strcpy(velocity,strsep(&distance, ","));
    strcpy(theta,strsep(&distance, ","));
    //end input stuff
    arrow arrow1 = make_arrow(atof(velocity), atof(theta)*M_PI/180);
    move(&p1, atof(distance));
    shoot(&p1, &p2, &arrow1, s);
    //2 player shoots
    //input stuff
    printf("Enter Shot:\n");
    fgets(input, 10, stdin);
    *strstr(input, "\n")=0;
    distance=input;
    strcpy(velocity,strsep(&distance, ","));
    strcpy(theta,strsep(&distance, ","));
    //end input stuff
    arrow arrow2 = make_arrow(atof(velocity), (180 - atof(theta)) *M_PI/180);
    move(&p2, -atof(distance));
    shoot(&p2, &p1, &arrow2, s);
  }
}

int main(){
  playGame();
  return 0;
}
