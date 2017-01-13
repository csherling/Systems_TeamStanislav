#ifndef PHYSICS_H
#define PHYSICS_H
typedef struct arrow{float velocity; float theta; float x; float y;}arrow;
typedef struct player{char health; float xcor;}player;

char shoot(player *shooter, player *target, float v, float theta);
void processCoor(float x, float y);
void kill(player *p);
void overshoot(int height);
void undershoot(int distance);

#endif
