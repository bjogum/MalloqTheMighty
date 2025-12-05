#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "navigate.h"

//skapa structar: Pos, Robot, Grid
typedef struct
{
    int maxX;
    int maxY; 
}Grid;


// gömmer makören
void hideCursor(){
printf("\e[?25l");
}


// rita ut spelplanen
void drawGrid(Grid *grid){
        // i varje rad, uppifårn och ner.
for (int y = 0 ; y < grid->maxY ; y++){
        // i varje steg, från vänster till höger.
        for (int x = 0 ; x < grid->maxX ; x++){
            //om det är övre eller undre raden, skriv ut WALL
            if (y == grid->maxY-grid->maxY || y == grid->maxY-1 || x == grid->maxX-grid->maxX || x == grid->maxX-1){
                // print upper and bottom row (för playground)
                printf("%c", WALL);
            } else {
                printf(" ");
            }
        }
    //new line efter varje rad slut.
    printf("\n");   
    
    }
}

int jobLeftPerc(Robot *malloq){
    int gridPoints = (GRIDSIZE_X-2) * (GRIDSIZE_Y-2);
    return ceil((gridPoints - malloq->moves / gridPoints) * 100);
}

int main(){
    //skapar grid
    Grid grid;

    // skapar robot
    Robot malloq;

    // init values
    grid.maxX = GRIDSIZE_X;
    grid.maxY = GRIDSIZE_Y;

    malloq.myHome.X = ROBOT_HOME_X;
    malloq.myHome.Y = ROBOT_HOME_Y;

    malloq.moves = 0;
    malloq.pos.X = malloq.myHome.X;
    malloq.pos.Y = malloq.myHome.Y;
    malloq.finish = false;

    clrscr();
    hideCursor();
    drawGrid(&grid);
    bool job2do = true;
    showMe(&malloq);
    move(0,0);

    // go to "start position"
    findEdge(&malloq);

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