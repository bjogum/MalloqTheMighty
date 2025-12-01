#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define clrscr() printf("\e[1;1H\e[2J")
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_ORANGE  "\033[38;5;208m"
#define COLOR_BLUE "\033[38;5;117m"
#define COLOR_CYAN "\033[38;5;51m"
#define COLOR_YELLOW "\033[38;5;226m"
#define COLOR_L_YELLOW "\033[38;5;228m"
#define COLOR_RED "\033[38;5;196m"

#define NO_WALL_LEFT malloq.pos.X > (GRIDSIZE_X-GRIDSIZE_X)+2
#define NO_WALL_RIGHT malloq.pos.X < GRIDSIZE_X-1
#define NO_WALL_UP malloq.pos.Y > (GRIDSIZE_Y-GRIDSIZE_Y)+2
#define NO_WALL_DOWN malloq.pos.Y < GRIDSIZE_Y-1

#define GRIDSIZE_X 40
#define GRIDSIZE_Y 20
#define ROBOT_SPEED 100000
#define WALL '#'

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
        enum Dir myDirection;
        int battery;
        int moves;
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

        // spara riktningen
        malloq.myDirection = dir;

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
        switch (malloq.myDirection)
        {
        case UP:
            //to RIGHT
            break;
        case DOWN:
            //to LEFT
            break;
        case LEFT:
            //to UP
            break;
        case RIGHT:
            //to DOWN
            break;
        }
    }


int main(){

    // init values
    grid.maxX = GRIDSIZE_X;
    grid.maxY = GRIDSIZE_Y;
    
    pos.X = 0;
    pos.Y = 0;

    malloq.moves = 0;
    malloq.pos.X = 9;
    malloq.pos.Y = 9;
    malloq.finish = false;

    clrscr();
    hideCursor();
    drawGrid();

    bool job2do = true;

    while(job2do){
        showMe(malloq);
        move(0,0);

        findEdge();
        //letsWalk(keepWallOnRight());
                // keepWallOnRight(); --> BOOL, gå åt rätt håll... retunerar riktning. while det är vägg rakt fram, retunera "vänster" : annars "höger". slimma vägg (kolla att vägg är till höger..)
                // turnMeRight(); //- create func: vad är höger? -> beroende på min tidigare/senaste och nuvarande pos. -> call HugTheEdge
                // hugTheEdge(); - slimma yttekanten



        

        // && wallBesideMe() - check, if not - call-> turnMeRight..
        while (NO_WALL_RIGHT){
            letsWalk(RIGHT);
        } 
        while (NO_WALL_UP){
            letsWalk(UP);
        } 
        while (NO_WALL_LEFT){
            letsWalk(LEFT);
        } 
        while (NO_WALL_DOWN){
            letsWalk(DOWN);  
        }

        // TODO: add func 
        //bennHere();

        // TODO: battery check..

        if (!jobLeftPerc()){
            job2do = false;
        }

    // walkStyles:
    //... hugMyTrace(); ska slimma senaste spår runt om.. 
    //... snakeSlither(); ska gå som en orm spår

    }

    // park malloq..
    printf("time to go home and sleep..zzz");
        
return 0;
}