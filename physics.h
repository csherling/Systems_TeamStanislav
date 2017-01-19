#ifndef PHYSICS_H
#define PHYSICS_H

typedef struct player{char health; double xcor;}player;

char shoot(player *shooter, player *target, double v, double theta);
void processCor(double x, double y);
void kill(player *p);
void overshoot(int height);
void undershoot(int distance);

#endif
