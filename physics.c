#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h> //does time exist tho?
#include "physics.h"

/*Params:
 shooter: pointer to the player who is shooting the arrow
 target: point to the player who is being shot at
 v: initial velocity positive and negative
 Returns:
 1-Hit
 0-Miss
 */
char shoot(player *shooter, player *target, double v, double theta, char direction, double s1, double s2, double s3, double s4){
  printf("Random seeds %f, %f, %f, %f\n", s1, s2, s3, s4);
  double arrow_x=shooter->xcor;
  double arrow_y = PLAYER_HEIGHT/*+getTerrain(shooter->xcor, s1, s2, s3, s4)*/;
  double vx = direction*v*cos(theta);
  double vy = v*sin(theta);
  while(arrow_y>=0/*+getTerrain(arrow_x, s1, s2, s3, s4)*/){
    //Movements
    vy-=GRAVITY*.01;
    arrow_x+=vx*.01;
    arrow_y+=vy*.01;
    processCor(arrow_x, arrow_y,atan2(vy, vx));
    if(arrow_x>=target->xcor&&arrow_x<=target->xcor+PLAYER_WIDTH){
      if(arrow_y<=PLAYER_HEIGHT/*+getTerrain(target->xcor, s1, s2, s3, s4)*/){
	kill(target);
	return 1;
      }
    }
  }
  if(direction*(target->xcor-arrow_x)<0){
    overshoot(fabs(arrow_x-target->xcor));
  }else{
    undershoot(fabs(target->xcor-arrow_x));
  }
  return 0;
}

void processCor(double x, double y, double theta){
  printf("(%f,%f) Angle: %f\n",x,y,theta);
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

//sets the seeds to be used in getTerrain
void setSeeds(double *s1, double *s2, double *s3, double *s4){
  srand(time(NULL));//Note srand is applicable in function because this is called once per game
  *s1 = (double)rand()/(double)RAND_MAX;
  *s2 = (double)rand()/(double)RAND_MAX;
  *s3 = (double)rand()/(double)RAND_MAX;
  *s4 = (double)rand()/(double)RAND_MAX;
}

//Generates hilly terrain using trigonometric functions
//Uses sine and cosine, with coefficients generated by the four seed values
// between 0 and 1
double getTerrain(double x, double s1, double s2, double s3, double s4){
  double ret;
  ret+=(s1*10)*(sin(x*s2/10));
  s1 = 1-s1;
  s2 = 1-s2;
  ret+=(s1*10)*(sin(x*s2/10));
  ret+=(s3*10)*(cos(x*s4/10));
  s3 = 1-s3;
  s4 = 1-s4;
  ret+=(s3*10)*(cos(x*s4/10));
  return ret;
}
