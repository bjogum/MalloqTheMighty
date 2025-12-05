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

bool isWallInFront(Robot *malloq){ 
    if ((malloq->myCurrentDir == RIGHT && WALL_RIGHT) ||
        (malloq->myCurrentDir == LEFT  && WALL_LEFT)  ||
        (malloq->myCurrentDir == UP    && WALL_UP)    ||
        (malloq->myCurrentDir == DOWN  && WALL_DOWN)){
    return true;
    }
    return false;
}

bool noWallToRight(Robot *malloq){
    if ((malloq->myCurrentDir == RIGHT && NO_WALL_DOWN)  ||
        (malloq->myCurrentDir == LEFT  && NO_WALL_UP)    ||
        (malloq->myCurrentDir == UP    && NO_WALL_RIGHT) ||
        (malloq->myCurrentDir == DOWN  && NO_WALL_LEFT)){
    return true;
    }
    return false;
}

bool beenHere(){
    return 0;
}

void keepWallOnRight(Robot *malloq){
    while (isWallInFront(malloq)){ // maybe: isWallInFront || isMyTraceInFront
        turnMeLeft(malloq); 
    } 
    if (noWallToRight(malloq)){ // maybe: noWallToRight || noTraceToRight
        turnMeRight(malloq); 
    }
}

void rememberThisPos(Robot *malloq){ // TODO: addera bara om positionen är "ny" / unik.
    // Allocate memory for the position-data
    if (!malloq->moves){
        malloq->historicPos = (Pos*)malloc(sizeof(malloq->historicPos));
    } else {
        malloq->historicPos = (Pos*)realloc(malloq->historicPos,sizeof(malloq->historicPos));
    }
    if (malloq->historicPos == NULL){
        printf("Error during memory allocation");
        return;
    }
    malloq->moves++;
    malloq->historicPos->X = malloq->pos.X;
    malloq->historicPos->Y = malloq->pos.Y;
}

// låt oss spatsera..
void letsWalk(Robot *malloq){
    // spara riktningen.. överflödig här?
    //malloq.myCurrentDir = dir;
    rememberThisPos(malloq);
    showMe(malloq);
    showMyTrace(malloq);
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