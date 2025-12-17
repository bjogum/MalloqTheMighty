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

        if (initLap && beenHere(&malloq) != -1){ // need "smarter" logic..
            initLap = false;
        }
    
        if (!initLap){
            avoidOverlap(&malloq); // if overlap-track turn right ahead, turn right 1 step earlier. if overlap-track turn left ahead, turn left 1 step later.
            followRoutePattern(&malloq, CIRCLE);
        }
    
        if (initLap)
        keepWallOnRight(&malloq);
        
        letsWalk(&malloq);
        
        // TODO: battery check..
        if (!jobLeftPerc(&malloq)){
            job2do = false;
        }
    }

    // park malloq..
    printf("time to go home and sleep..zzz");
        
return 0;
}