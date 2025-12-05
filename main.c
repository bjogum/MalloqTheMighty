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

    bool job2do = true;
    while(job2do){
        
        if (!beenHere()){
            keepWallOnRight(&malloq); // First lap -> run until "hit" / overlap - just first lap or extended....? edgeLap = true ?
        } else {
            // have been here ... overlap until next turn. -> then, hugMyTrace
        }

        // workStyles:  --> After first lap.
        //... a) hugMyTrace(); ska slimma senaste spår runt om.. 
        //... b) snakeSlither(); ska gå som en "orm" / fram-tillbax etc..

        letsWalk(&malloq);
        
        // TODO: add func 
        //bennHere();

        // TODO: battery check..

        if (!jobLeftPerc(&malloq)){
            job2do = false;
        }
    }

    // park malloq..
    printf("time to go home and sleep..zzz");
        
return 0;
}