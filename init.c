#include "init.h"
#include "navigate.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

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


void setInitValues(Robot *malloq){
    //skapar grid
    Grid grid;

    // init values
    grid.maxX = GRIDSIZE_X;
    grid.maxY = GRIDSIZE_Y;

    malloq->myHome.X = ROBOT_HOME_X;
    malloq->myHome.Y = ROBOT_HOME_Y;

    malloq->moves = 0;
    malloq->pos.X = malloq->myHome.X;
    malloq->pos.Y = malloq->myHome.Y;
    malloq->finish = false;
    malloq->uniqueMovesCounter = 0;
    malloq->overlapCounter = 0;
    clrscr();
    hideCursor();
    drawGrid(&grid);
    
    showMe(malloq);
    move(0,0);
}
