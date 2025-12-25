#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "navigate.h"
#include "init.h"
#include "log.h"


int jobLeftPerc(Robot *malloq){
    int gridPoints = (GRIDSIZE_X-2) * (GRIDSIZE_Y-2);

    // for debug
    char debug[100];
    snprintf(debug, sizeof(debug), "[JOBLEFT: %f]", ceil(((gridPoints - malloq->moves) * 100) / gridPoints));
    saveStr(debug);

    return ceil(((gridPoints - malloq->moves) * 100) / gridPoints);

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
            // job2do = false;
        }
    }

    // park malloq..
        
return 0;
}