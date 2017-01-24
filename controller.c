#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

int main( int argc, char *argv[] ) {

    char *host;
    host = argv[1];
    int sd;
    sd = client_connect( host, atoi(argv[2]) );
    char buffer[MESSAGE_BUFFER_SIZE];

    int msgsize = 0;
    int i = 0;

    char* distance;
    char velocity[10];
    char theta[10];

    while (1) {
        printf("enter message: ");
        fflush(stdout);
        fgets( buffer, sizeof(buffer), stdin );
        *strstr(buffer, "\n")=0;
        distance=buffer;
        strcpy(velocity,strsep(&distance, ","));
        strcpy(theta,strsep(&distance, ","));
        shot s;
        s.velocity=atof(velocity);
        s.theta=atof(theta);
        s.distance=atof(distance);
        write(sd, &s, sizeof(s));
        printf("%lf, %lf, %lf -- vel, theta, dist\n", s.velocity, s.theta, s.distance);
        printf("Stringy: %s, %s, %s -- vel, theta, dist\n", velocity, theta, distance);
    }

    return 0;
}
