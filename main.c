#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "navigate.h"
#include "init.h"


int jobLeftPerc(Robot *malloq){
    int gridPoints = (GRIDSIZE_X-2) * (GRIDSIZE_Y-2);
    return ceil((gridPoints - malloq->moves / gridPoints) * 100);
}

int main(){
    //skapar robot
    Robot malloq;
    setInitValues(&malloq);
    
    // go to "start position"
    findEdge(&malloq);

    bool initLap = true;
    bool job2do = true;
    while(job2do){

        if (initLap && getOverLapIndex(&malloq, -1, -1) != -1){ // ..need "smarter" logic..
            initLap = false;
        }
    
        if (!initLap){
            fixOverLap(&malloq);
            followRoutePattern(&malloq, CIRCLE);
        }

        keepWallOnRight(&malloq);   // stay in grid
        letsWalk(&malloq, true);    // move malloq
        
        // TODO: battery check..
        if (!jobLeftPerc(&malloq)){
            job2do = false;
        }
    }

    // park malloq..
    printf("time to go home and sleep..zzz");
        
return 0;
}