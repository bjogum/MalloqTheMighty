#ifndef NAVIGATE_H
#define NAVIGATE_H

#define WALL '#'
#define GRIDSIZE_X 40
#define GRIDSIZE_Y 20

#define ROBOT_SPEED 200000 // Fast=300 000
#define ROBOT_HOME_X 16
#define ROBOT_HOME_Y 5

#define clrscr() printf("\e[1;1H\e[2J")
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_ORANGE  "\033[38;5;208m"
#define COLOR_YELLOW "\033[38;5;226m"
#define COLOR_RED "\033[38;5;196m"
#define NO_WALL_LEFT malloq->pos.X > (GRIDSIZE_X-GRIDSIZE_X)+2
#define NO_WALL_RIGHT malloq->pos.X < GRIDSIZE_X-1
#define NO_WALL_UP malloq->pos.Y > (GRIDSIZE_Y-GRIDSIZE_Y)+2
#define NO_WALL_DOWN malloq->pos.Y < GRIDSIZE_Y-1
#define WALL_LEFT malloq->pos.X == (GRIDSIZE_X-GRIDSIZE_X)+2
#define WALL_RIGHT malloq->pos.X == GRIDSIZE_X-1
#define WALL_UP malloq->pos.Y == (GRIDSIZE_Y-GRIDSIZE_Y)+2
#define WALL_DOWN malloq->pos.Y == GRIDSIZE_Y-1

enum WorkStyle {
CIRCLE,
SNAKE
};

// enum för Robot status
enum Status {
SLEEP,
CHARGING,
RUNNING,
ERROR,
};


enum Dir {
UP,
DOWN,
LEFT,
RIGHT,
};

typedef struct
{
    int X;
    int Y; 
}Pos;

typedef struct
{
    enum Status status;
    enum Dir myCurrentDir;
    int battery;
    int moves;
    Pos myHome;
    Pos pos;
    Pos *historicPos;
    int uniqueMovesCounter;
    int overlapCounter;
    bool finish;
}Robot;


void turnMeRight(Robot *malloq);
void turnMeLeft(Robot *malloq);
bool isWallInFront(Robot *malloq);
bool noWallToRight(Robot *malloq);
int getOverLapIndex(Robot *malloq, int x, int y);
void keepWallOnRight(Robot *malloq);
void rememberThisPos(Robot **malloq);
void letsWalk(Robot *malloq, bool atTrack);
void findEdge(Robot *malloq);
void move(int x, int y);
void showMe(Robot *malloq);
void showMyTrace(Robot *malloq);
void fixOverLap(Robot *malloq);
void keepMyTrackOnRight(Robot *malloq);
void followRoutePattern(Robot *malloq, enum WorkStyle style);

#endif