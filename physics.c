#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "physics.h"

#define PLAYER_HEIGHT 5
#define GRAVITY 9.8
//struct arrow has initial velocity and angle, and current coordinates

/*Params:
 p: player who is shooting the arrow
 v: initial velocity positive and negative
 Returns:
 1-Hit
 0-Miss
 */
char shoot(player *shooter, player *target, float v, float theta){
  float arrow_x=shooter->xcor;
  float arrow_y = PLAYER_HEIGHT;
  float vx = v*cos(theta);
  float vy = v*sin(theta);
  while(arrow_y>=0){
    //Checks for collisions and if it has hit player
    if(v>0){
      if(arrow_x>=target->xcor){
	if(arrow_y<=PLAYER_HEIGHT){
	  kill(target);
	  return 1;
	}else{
	  overshoot(arrow_y);
	  return 0;
	}
      }
    }else{
      if(arrow_x<=target->xcor){
	if(arrow_y<=PLAYER_HEIGHT){
	  kill(target);
	  return 1;
	}else{
	  return 0;
	}
      }
    }
    //Movements
    arrow_x+=vx;
    arrow_y+=vy;
    vy-=GRAVITY;
  }
  undershoot(target->xcor-arrow_x);
  return 0;
}

void processCoor(float x, float y){
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
  float theta = M_PI/2;
  while(!going){
    printf("Theta: %f\n",theta);
    going=shoot(&p1, &p2, 40, theta);
    theta-=.01;
  }
  return 0;
}