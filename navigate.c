#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "navigate.h"
#include "log.h"

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
/*
// if overlap; return the index (historicPos) | If NO overlap; return -1.
int getOverLapIndex(Robot *malloq){
    if (malloq->moves){
        for (int i = 0; i < (*malloq).uniqeMovesCounter; i++){
            if (malloq->pos.X == malloq->historicPos[i].X &&
                malloq->pos.Y == malloq->historicPos[i].Y){
                    //malloq->overlapCounter++;
                    return i;
            }
        }       
    }

    return -1;
}
*/


// Input: robot (for actual position) || robot + x , y (for selected position)
// if overlap; return the index (historicPos) | If NO overlap; return -1.
int getOverLapIndex(Robot *malloq, int xPos, int yPos){
    if (xPos == -1 && yPos == -1){
        xPos = malloq->pos.X;
        yPos = malloq->pos.Y;
    }

    if (malloq->moves){
        for (int i = 0; i < (*malloq).uniqeMovesCounter; i++){
            if (xPos == malloq->historicPos[i].X &&
                yPos == malloq->historicPos[i].Y){
                    return i;
            }
        }       
    }

    return -1;
}



// Håller roboten med väggen till höger. Gå moturs.
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
void fixOverLap(Robot *malloq){
    int overLapIndex = getOverLapIndex(malloq, -1, -1);

    if (overLapIndex != -1){
        
        //printf("< !overlap! I%d->y%d:x%d>", overLapIndex , malloq->pos.Y, malloq->pos.X);

        // 1. if turn ahead is to -> right
        if ( ((malloq->myCurrentDir == UP) && (malloq->historicPos[overLapIndex+1].X > malloq->pos.X))  ||   
        ((malloq->myCurrentDir == RIGHT) && (malloq->historicPos[overLapIndex+1].Y > malloq->pos.Y))    ||
        ((malloq->myCurrentDir == LEFT) && (malloq->historicPos[overLapIndex+1].Y < malloq->pos.Y))     ||
        ((malloq->myCurrentDir == DOWN) && (malloq->historicPos[overLapIndex+1].X < malloq->pos.X)) )
        {
            
            // for debug
            char debug[100];
            snprintf(debug, sizeof(debug), "Right(OverLapFx) -> PosX [%d] | PosY [%d] | HistPosX+1 [%d] | HistPosY+1 [%d]", malloq->pos.X, malloq->pos.Y, malloq->historicPos[overLapIndex+1].X, malloq->historicPos[overLapIndex+1].Y);
            saveStr(debug);

            turnMeRight(malloq);

        // 2. if turn ahead is to -> left
        } else if ( ((malloq->myCurrentDir == UP) && (malloq->historicPos[overLapIndex+2].X < malloq->pos.X))   ||
        ((malloq->myCurrentDir == RIGHT) && (malloq->historicPos[overLapIndex+2].Y < malloq->pos.Y))            ||
        ((malloq->myCurrentDir == LEFT) && (malloq->historicPos[overLapIndex+2].Y > malloq->pos.Y))             ||
        ((malloq->myCurrentDir == DOWN) && (malloq->historicPos[overLapIndex+2].X > malloq->pos.X)) )
        {
            // for debug
            char debug[100];
            snprintf(debug, sizeof(debug), "Left(OverLapFx) -> PosX [%d] | PosY [%d] | HistPosX+2 [%d] | HistPosY+2 [%d]", malloq->pos.X, malloq->pos.Y, malloq->historicPos[overLapIndex+2].X, malloq->historicPos[overLapIndex+2].Y);
            saveStr(debug);
            
            turnMeLeft(malloq);

        }
    }
}

void keepMyTrackOnRight(Robot *malloq){
    // om tidigare spår (till höger) går till vänster, sväng vänster nu. Om tidigare spår (till höger) går till höger sväng höger om 2 steg.
    // Behövs ej? (1. vilken index har historyPos-spåret till höger om mig nu?)
    // 2. kolla index+2 för potentiell vänstersväng (alltså rakt fram från mig) - träff? -> sväng vänster
    


    // 3. kolla index+1 för potentiell högersväng - träff? -> sväng höger
    // Behövs ej? (4. kolla så svängen inte har overlap)
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