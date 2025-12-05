#ifndef INIT_H
#define INIT_H
#include <stdbool.h>
#include "navigate.h"

typedef struct
{
    int maxX;
    int maxY; 
}Grid;

void hideCursor();
void drawGrid(Grid *grid);
void setInitValues(Robot *malloq);

#endif