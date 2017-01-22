#ifndef PHYSICS_H
#define PHYSICS_H

#define PLAYER_HEIGHT 5
#define PLAYER_WIDTH 3
#define GRAVITY 9.8
#define DISTANCE 100
#define START_HEALTH 3

typedef struct player{char health; double xcor;}player;

char shoot(player *shooter, player *target, double v, double theta, char direction, double s1, double s2, double s3, double s4);
void processCor(double x, double y, double theta);
void kill(player *p);
void overshoot(int height);
void undershoot(int distance);
void setSeeds(double *s1, double *s2, double *s3, double *s4);
double getTerrain(double x, double s1, double s2, double s3, double s4);

#endif
