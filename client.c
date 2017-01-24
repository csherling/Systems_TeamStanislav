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

    double * buf;

    while(1){
        while(read( sd, buf, sizeof(double))){

            printf( "received: %lf\n", *buf);
            sleep(0.5);
        }
    }



    return 0;
}
