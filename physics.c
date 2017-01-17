#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "physics.h"

#define PLAYER_HEIGHT 5
#define PLAYER_WIDTH 3
#define GRAVITY 9.8
/*Params:
 shooter: pointer to the player who is shooting the arrow
 target: point to the player who is being shot at
 v: initial velocity positive and negative
 Returns:
 1-Hit
 0-Miss
 */
char shoot(player *shooter, player *target, float v, float theta){
  float arrow_x=shooter->xcor;
  float arrow_y = PLAYER_HEIGHT;
  float t = 0;
  while(arrow_y>=0){
    //Movements
    arrow_x=v*cos(theta)*t+shooter->xcor;
    arrow_y=PLAYER_HEIGHT+abs(v*sin(theta)*t)-.5*GRAVITY*t*t;
    processCor(arrow_x, arrow_y);
    //Checks for collisions and if it has hit player
    if(v>0){
      if(arrow_x>=target->xcor&&arrow_x<=target->xcor+PLAYER_WIDTH){
	if(arrow_y<=PLAYER_HEIGHT){
	  kill(target);
	  return 1;
	}
      }
    }else{
      if(arrow_x<=target->xcor&&arrow_x>=target->xcor-PLAYER_WIDTH){
	if(arrow_y<=PLAYER_HEIGHT){
	  kill(target);
	  return 1;
	}
      }
    }
    t+=.01;
  }
  if(arrow_x>target->xcor){
    overshoot(abs(arrow_x-target->xcor));
  }else{
    undershoot(abs(target->xcor-arrow_x));
  }
  return 0;
}

void processCor(float x, float y){
  printf("(%f,%f)\n",x,y);
}

void kill(player *p){
  p->health-=1;
  printf("HIT! new health: %d\n",p->health);
}


void overshoot(int height){
  printf("MISSED! overshot by %d\n",height);
}

void undershoot(int distance){
  printf("MISSED! undershot by %d\n",distance);
}
int main(){
  player p1;
  p1.xcor = 0;
  p1.health = 1;
  player p2;
  p2.xcor = 100;
  p2.health = 1;
  int going = 0;
  char input[10];
  char* theta;
  char velocity[10];
  while(!going){
    printf("Enter Shot:\n");
    fgets(input, 10, stdin);
    *strstr(input, "\n")=0;
    theta=input;
    strcpy(velocity,strsep(&theta, ","));
    printf("Velocity: %s\nTheta: %s\n",velocity, theta);
    char c = 0;
    if(c){
      going=shoot(&p1, &p2, atof(velocity), atof(theta)*M_PI/180);
      c=1;
    }else{
      going=shoot(&p2, &p1, atof(velocity), atof(theta)*M_PI/180);
      c=0;
    }
  }
  return 0;
}
