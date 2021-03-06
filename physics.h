#ifndef PHYSICS_H
#define PHYSICS_H
#define PLAYER_HEIGHT 5
#define PLAYER_WIDTH 3
#define GRAVITY 9.8
#define DISTANCE 100
#define START_HEALTH 3
typedef struct player{char health; double xcor;}player;

typedef struct arrow {
    double x;
    double y;
    double vx;
    double vy;
} arrow;

typedef struct seed{
  double s1;
  double s2;
  double s3;
  double s4;
} seed;

typedef struct gamedata {
    arrow ar;
    seed terrain_seed;
    player player1;
    player player2;
    int display_index;
} gamedata;

double signum(double x);
arrow make_arrow(double v, double angle);
char shoot(player *shooter, player *target, arrow *arrow, seed s);
int shootStep(player *shooter, player *target, arrow *arrow, seed s, gamedata *data);
void move(player *shooter, double distance);
void processCor(double x, double y, double theta);
void killplayer(player *p);
void overshoot(int distance);
void undershoot(int distance);
void setSeeds(seed *s);
double getTerrain(double x, seed s1);
void processTerrain(double x, double y);
void printdata(gamedata g);
void display(char comp, char numcoms, seed s);

#endif
