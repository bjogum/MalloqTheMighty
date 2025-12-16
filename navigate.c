#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "navigate.h"

// flyttar makören till vald position
void move(int x, int y){
printf("\x1B[%d;%dH", y, x);
}

// ritar ut roboten på nuvarande position
void showMe(Robot *malloq){
    move(malloq->pos.X, malloq->pos.Y);
    printf(COLOR_RED"●\n"COLOR_RESET);
    usleep(ROBOT_SPEED);
}

// radera roboten på senaste position
void showMyTrace(Robot *malloq){
    move(malloq->pos.X,malloq->pos.Y);
    printf(COLOR_GREEN"o"COLOR_RESET);
}

// sväng till höger
void turnMeRight(Robot *malloq){
    switch (malloq->myCurrentDir)
    {
    case UP:
        malloq->myCurrentDir = RIGHT;
        break;
    case DOWN:
        malloq->myCurrentDir = LEFT;
        break;
    case LEFT:
        malloq->myCurrentDir = UP;
        break;
    case RIGHT:
        malloq->myCurrentDir = DOWN;
        break;
    }
}

// sväng till vänster
void turnMeLeft(Robot *malloq){
    switch (malloq->myCurrentDir)
    {
    case UP:
        malloq->myCurrentDir = LEFT;
        break;
    case DOWN:
        malloq->myCurrentDir = RIGHT;
        break;
    case LEFT:
        malloq->myCurrentDir = DOWN;
        break;
    case RIGHT:
        malloq->myCurrentDir = UP;
        break;
    }
}

//  kontrollera om det är vägg framför
bool isWallInFront(Robot *malloq){ 
    if ((malloq->myCurrentDir == RIGHT && WALL_RIGHT) ||
        (malloq->myCurrentDir == LEFT  && WALL_LEFT)  ||
        (malloq->myCurrentDir == UP    && WALL_UP)    ||
        (malloq->myCurrentDir == DOWN  && WALL_DOWN)){
    return true;
    }
    return false;
}

// kontrollera om vägg till höger saknas
bool noWallToRight(Robot *malloq){
    if ((malloq->myCurrentDir == RIGHT && NO_WALL_DOWN)  ||
        (malloq->myCurrentDir == LEFT  && NO_WALL_UP)    ||
        (malloq->myCurrentDir == UP    && NO_WALL_RIGHT) ||
        (malloq->myCurrentDir == DOWN  && NO_WALL_LEFT)){
    return true;
    }
    return false;
}

// if overlap more than 2 steps..
int beenHere(Robot *malloq){
    if (malloq->moves){
        for (int i = 0; i < (*malloq).uniqeMovesCounter; i++){
            if (malloq->pos.X == malloq->historicPos[i].X &&
                malloq->pos.Y == malloq->historicPos[i].Y){
                    //malloq->overlapCounter++;
                    return i;
            }
        }       
    }

    //if (malloq->overlapCounter > 2){
    //    malloq->overlapCounter = 0;
    //    return true;
        
    //}
    return -1;
}

// Håller roboten utmed högerväggen. Moturs.
void keepWallOnRight(Robot *malloq){
    while (isWallInFront(malloq)){ 
        turnMeLeft(malloq); 
    } 
    if (noWallToRight(malloq)){ 
        turnMeRight(malloq); 
    }
}

void rememberThisPos(Robot **malloq){  
    bool newUniqePos = true;

    // Add ONLY uniqe data
    if ((*malloq)->moves){
        for (int i = 0; i < (*malloq)->uniqeMovesCounter; i++){
            if ((*malloq)->pos.X == (*malloq)->historicPos[i].X &&
                (*malloq)->pos.Y == (*malloq)->historicPos[i].Y){
                newUniqePos = false;
                break;
            }
        }
    }

    if (newUniqePos){
        // Allocate memory for the position-data
        if (!(*malloq)->moves){
            (*malloq)->historicPos = (Pos*)malloc(sizeof(Pos) * 1);
        } else {
            (*malloq)->historicPos = (Pos*)realloc((*malloq)->historicPos, sizeof(Pos) * (*malloq)->uniqeMovesCounter+1);
        }
        if ((*malloq)->historicPos == NULL){
            printf("Error during memory allocation");
            return;
        }

        // add the new position, at the new index. Add +1 to uniqeMovesCounter.
        (*malloq)->historicPos[(*malloq)->uniqeMovesCounter].X = (*malloq)->pos.X;
        (*malloq)->historicPos[(*malloq)->uniqeMovesCounter].Y = (*malloq)->pos.Y;
        (*malloq)->uniqeMovesCounter++;
    }
}

// move robot to new pos
void letsWalk(Robot *malloq){

    rememberThisPos(&malloq);
    showMe(malloq);
    showMyTrace(malloq);

    // count robot total moves
    malloq->moves++;

    switch (malloq->myCurrentDir) {
    case UP:
            malloq->pos.Y--;
        break;
    case DOWN:
            malloq->pos.Y++;
        break;
    case LEFT:
            malloq->pos.X--;
        break;
    case RIGHT:
            malloq->pos.X++;
        break;
    }
}

// go to right edge.
void findEdge(Robot *malloq){
    while (NO_WALL_RIGHT){
        malloq->myCurrentDir = RIGHT;
        letsWalk(malloq);
    }
}

// if overlap-track turn right ahead, turn right 1 step earlier. if overlap-track turn left ahead, turn left 1 step later.
void avoidOverlap(Robot *malloq){
    int overLapIndex = beenHere(malloq);

    if (overLapIndex != -1){
        
        printf("<<< !overlap! index%d-y%d:x%d>>>", overLapIndex , malloq->pos.Y, malloq->pos.X);

        // 1. if up->right
        if ( ((malloq->myCurrentDir == UP) && (malloq->historicPos[overLapIndex+1].X > malloq->pos.X))  ||   
        ((malloq->myCurrentDir == RIGHT) && (malloq->historicPos[overLapIndex+1].Y > malloq->pos.Y))    ||
        ((malloq->myCurrentDir == LEFT) && (malloq->historicPos[overLapIndex+1].Y < malloq->pos.Y))) 
        {
            // turn right, one step later
            letsWalk(malloq);
            turnMeRight(malloq);


        // 2. if up->left
        } else if ( ((malloq->myCurrentDir == UP) && (malloq->historicPos[overLapIndex+1].X < malloq->pos.X))   ||
        ((malloq->myCurrentDir == RIGHT) && (malloq->historicPos[overLapIndex+1].Y < malloq->pos.Y))            ||
        ((malloq->myCurrentDir == LEFT) && (malloq->historicPos[overLapIndex+1].Y > malloq->pos.Y)))
        {
            // turn left now
            turnMeLeft(malloq);


        // 3. if down->right
        } else if  (malloq->myCurrentDir == DOWN && (malloq->historicPos[overLapIndex+1].X > malloq->pos.X))
        {
            // turn right now
            turnMeRight(malloq);


        // 4. if down->left
        } else if  (malloq->myCurrentDir == DOWN && (malloq->historicPos[overLapIndex+1].X < malloq->pos.X))
        {
            // turn left, one step later
            letsWalk(malloq);
            turnMeLeft(malloq);

        }
    }
}

void keepMyTrackOnRight(Robot *malloq){

}


void followRoutePattern(Robot *malloq, enum WorkStyle style){
    switch (style)
    {
    case CIRCLE:
        keepMyTrackOnRight(malloq);
            
        break;
    case SNAKE:
        //snakeSlither(malloq); // not finish
    break;
    
    default:
        break;
    }
}