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
bool beenHere(Robot *malloq){
    if (malloq->moves){
        for (int i = 0; i < sizeof(malloq->historicPos); i++){
            if (malloq->pos.X == malloq->historicPos[i].X &&
                malloq->pos.Y == malloq->historicPos[i].Y){
                    malloq->overlapCounter++;
                    break;
            }
        }       
    }

    if (malloq->overlapCounter > 2){
        malloq->overlapCounter = 0;
        return true;
        
    }
    return false;
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
        for (int i = 0; i < sizeof((*malloq)->historicPos); i++){
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
            (*malloq)->historicPos = (Pos*)malloc(sizeof((*malloq)->historicPos));
        } else {
            (*malloq)->historicPos = (Pos*)realloc((*malloq)->historicPos,sizeof((*malloq)->historicPos));
        }
        if ((*malloq)->historicPos == NULL){
            printf("Error during memory allocation");
            return;
        }

        // får tillbaka samma adress trots realloc.....
        printf("--> %p", (*malloq)->historicPos);

        // save the position and count moves.
        (*malloq)->historicPos->X = (*malloq)->pos.X;
        (*malloq)->historicPos->Y = (*malloq)->pos.Y;
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