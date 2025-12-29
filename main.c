#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "navigate.h"
#include "init.h"
#include "log.h"


int jobLeftPerc(Robot *malloq){
    int gridSteps = (GRIDSIZE_X-2) * (GRIDSIZE_Y-2); // 684
    return ceil(((gridSteps - malloq->uniqueMovesCounter) * 1000) / gridSteps);
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
        
        // TODO: battery check..

        if (jobLeftPerc(&malloq)){
            letsWalk(&malloq, true);    // move malloq
        } else {
            // park malloq..
            // job2do = false;
            }
            
        
    }
        
return 0;
}