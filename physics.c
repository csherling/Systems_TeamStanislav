#include <stdio.h>
#include <stdlib.h>
#include "physics.h"

#define PLAYER_HEIGHT 5
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
  while(arrow_y>=0){
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
  return 0;
}
