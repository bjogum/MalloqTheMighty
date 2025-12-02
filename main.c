#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define GRIDSIZE_X 40
#define GRIDSIZE_Y 20
#define ROBOT_SPEED 100000
#define ROBOT_HOME_X 9
#define ROBOT_HOME_Y 9

#define clrscr() printf("\e[1;1H\e[2J")
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_ORANGE  "\033[38;5;208m"
#define COLOR_YELLOW "\033[38;5;226m"
#define COLOR_RED "\033[38;5;196m"

#define WALL '#'

#define NO_WALL_LEFT malloq.pos.X > (GRIDSIZE_X-GRIDSIZE_X)+2
#define NO_WALL_RIGHT malloq.pos.X < GRIDSIZE_X-1
#define NO_WALL_UP malloq.pos.Y > (GRIDSIZE_Y-GRIDSIZE_Y)+2
#define NO_WALL_DOWN malloq.pos.Y < GRIDSIZE_Y-1

#define WALL_LEFT malloq.pos.X == (GRIDSIZE_X-GRIDSIZE_X)+2
#define WALL_RIGHT malloq.pos.X == GRIDSIZE_X-1
#define WALL_UP malloq.pos.Y == (GRIDSIZE_Y-GRIDSIZE_Y)+2
#define WALL_DOWN malloq.pos.Y == GRIDSIZE_Y-1

    enum Dir {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    };

    // enum för Robot status
    enum Status {
    SLEEP,
    CHARGING,
    RUNNING,
    ERROR,
    };

    //skapa structar: Pos, Robot, Grid
    typedef struct
    {
        int maxX;
        int maxY; 
    }Grid;

    //skapar grid
    Grid grid;

    typedef struct
    {
        int X;
        int Y; 
    }Pos;

    //skapar pos
    Pos pos;

    typedef struct
    {
        enum Status status;

        //behövs denna ens?
        enum Dir myLastDir;

        enum Dir myCurrentDir;
        int battery;
        int moves;
        Pos myHome;
        Pos pos;
        Pos *historicPos;
        bool finish;
    }Robot;

    // skapar robot + init
    Robot malloq;

    // gömmer makören
    void hideCursor(){
    printf("\e[?25l");
    }

    // flyttar makören till vald position
    void move(int x, int y){
    printf("\x1B[%d;%dH", y, x);
    }

    // ritar ut roboten på nuvarande position
    void showMe(Robot malloq){
        move(malloq.pos.X, malloq.pos.Y);
        printf(COLOR_RED"●\n"COLOR_RESET);
        usleep(ROBOT_SPEED);
    }

    // radera roboten på senaste position
    void showMyTrace(Robot malloq){
        move(malloq.pos.X,malloq.pos.Y);
        printf(COLOR_GREEN"o"COLOR_RESET);
}
    
    // rita ut spelplanen
    void drawGrid(){
            // i varje rad, uppifårn och ner.
    for (int y = 0 ; y < grid.maxY ; y++){

            // i varje steg, från vänster till höger.
            for (int x = 0 ; x < grid.maxX ; x++){

                //om det är övre eller undre raden, skriv ut WALL
                if (y == grid.maxY-grid.maxY || y == grid.maxY-1 || x == grid.maxX-grid.maxX || x == grid.maxX-1){

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

    void rememberThisPos(){ // TODO: addera bara om positionen är "ny" / unik.

        // Allocate memory for the position-data
        if (!malloq.moves){
            malloq.historicPos = (Pos*)malloc(sizeof(malloq.historicPos));
        } else {
            malloq.historicPos = (Pos*)realloc(malloq.historicPos,sizeof(malloq.historicPos));
        }

        if (malloq.historicPos == NULL){
            printf("Error during memory allocation");
            return;
        }

        malloq.moves++;
        malloq.historicPos->X = malloq.pos.X;
        malloq.historicPos->Y = malloq.pos.Y;
    }


    // låt oss spatsera..
    void letsWalk(enum Dir dir){

        // spara riktningen.. överflödig här?
        //malloq.myCurrentDir = dir;

        rememberThisPos();
        showMe(malloq);
        showMyTrace(malloq);

        switch (dir) {
        case UP:
                malloq.pos.Y--;
            break;
        case DOWN:
                malloq.pos.Y++;
            break;
        case LEFT:
                malloq.pos.X--;
            break;

        case RIGHT:
                malloq.pos.X++;
            break;
        }
    }

    // go to right edge.
    void findEdge(){
        while (NO_WALL_RIGHT){
            letsWalk(RIGHT);
        }
        
    }

    int jobLeftPerc(){
        int gridPoints = (GRIDSIZE_X-2) * (GRIDSIZE_Y-2);
        return ceil((gridPoints - malloq.moves / gridPoints) * 100);
    }

    void turnMeRight(){
        switch (malloq.myCurrentDir)
        {
        case UP:
            malloq.myCurrentDir = RIGHT;
            break;
        case DOWN:
            malloq.myCurrentDir = LEFT;
            break;
        case LEFT:
            malloq.myCurrentDir = UP;
            break;
        case RIGHT:
            malloq.myCurrentDir = DOWN;
            break;
        }
    }

        void turnMeLeft(){
        switch (malloq.myCurrentDir)
        {
        case UP:
            malloq.myCurrentDir = LEFT;
            break;
        case DOWN:
            malloq.myCurrentDir = RIGHT;
            break;
        case LEFT:
            malloq.myCurrentDir = DOWN;
            break;
        case RIGHT:
            malloq.myCurrentDir = UP;
            break;
        }
    }

    bool isWallInFront(){ 
        if ((malloq.myCurrentDir == RIGHT && WALL_RIGHT) ||
            (malloq.myCurrentDir == LEFT  && WALL_LEFT)  ||
            (malloq.myCurrentDir == UP    && WALL_UP)    ||
            (malloq.myCurrentDir == DOWN  && WALL_DOWN)){
        return true;
        }
        return false;
    }

    bool noWallToRight(){
        if ((malloq.myCurrentDir == RIGHT && NO_WALL_DOWN)  ||
            (malloq.myCurrentDir == LEFT  && NO_WALL_UP)    ||
            (malloq.myCurrentDir == UP    && NO_WALL_RIGHT) ||
            (malloq.myCurrentDir == DOWN  && NO_WALL_LEFT)){
        return true;
        }
        return false;
    }

    bool beenHere(){
        return 0;
    }

    void keepWallOnRight(){
        while (isWallInFront()){ // maybe: isWallInFront || isMyTraceInFront
            turnMeLeft(); 
        } 

        if (noWallToRight()){ // maybe: noWallToRight || noTraceToRight
            turnMeRight(); 
        }
    }


int main(){

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
    drawGrid();
    bool job2do = true;
    showMe(malloq);
    move(0,0);


    // go to "start position"
    findEdge();

    while(job2do){
        
        if (!beenHere()){
            keepWallOnRight(); // First lap -> run until "hit" / overlap - just first lap or extended....? edgeLap = true ?
        } else {
            // have been here ... overlap until next turn. -> then, hugMyTrace
        }

        // workStyles:  --> After first lap.
        //... a) hugMyTrace(); ska slimma senaste spår runt om.. 
        //... b) snakeSlither(); ska gå som en "orm" / fram-tillbax etc..

        letsWalk(malloq.myCurrentDir);
        
        // TODO: add func 
        //bennHere();

        // TODO: battery check..

        if (!jobLeftPerc()){
            job2do = false;
        }
    }

    // park malloq..
    printf("time to go home and sleep..zzz");
        
return 0;
}