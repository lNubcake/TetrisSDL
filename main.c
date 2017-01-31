#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include "source.c"

int main(int argc, char *argv[]){

cell field[10][20];
SDL_Surface* screen;
SDL_Event ev;
SDL_TimerID timer;
bool play = true;
bool shapetriger = true;

int i,j;

for(i = 0; i < 10; i++){
    for(j = 0; j < 20; j++){
        field[i][j].filled = false;
        field[i][j].color = NULL;
        field[i][j].lt_x = 15 + i * 30;
        field[i][j].lt_y = 15 + j * 30;
        field[i][j].right = (i==9) ? NULL : &field[i+1][j];
        field[i][j].left = (i==0) ? NULL : &field[i-1][j];
        field[i][j].down = (j==19) ? NULL : &field[i][j+1];
        field[i][j].up = (j == 0 ? NULL : &field[i][j-1]);
    }
}

SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

screen = SDL_SetVideoMode(330,630,0,SDL_ANYFORMAT);
timer = SDL_AddTimer(1000,settimer,NULL);

SDL_WM_SetCaption("Tetris","Tetris");

drawfield(screen);
refresh(screen,/*10,*/20,field);

srand(time(NULL));

while(play){
    direction d;
    if(shapetriger){//new shape occur
        int r;
        r = filledline(20,field);
        while(r != -1){
            rmline(20,r,field);
            r = filledline(20,field);
        }
        movingfalse(20,field);
        r = rand() % 8 +1 ;
        gimmeashape(&(field[r][0]),play);
        shapetriger = false;
        refresh(screen,20,field);
    }

    SDL_WaitEvent(&ev);
    switch(ev.type){
        case SDL_QUIT:
            play=false;
            break;
        case SDL_KEYDOWN:
            if(ev.key.keysym.sym == SDLK_SPACE){
                if(rotatable(20,field)){
                    rotateshape(screen,20,field);
                    refresh(screen,20,field);
                }
                break;
            }
            if(ev.key.keysym.sym == SDLK_LEFT){
                d = LEFT;
                if(!canimove(20,field,d))
                    break;
                mshape(20,field,d);
                refresh(screen,20,field);
                break;
            }
            if(ev.key.keysym.sym == SDLK_RIGHT){
                d = RIGHT;
                if(!canimove(20,field,d))
                    break;
                mshape(20,field,d);
                refresh(screen,20,field);
                break;
            }
            if(ev.key.keysym.sym == SDLK_DOWN){
                d = DOWN;
                if(!canimove(20,field,d)){
                    shapetriger = true;
                    break;
                    }
                mshape(20,field,d);
                refresh(screen,20,field);
                break;
            }
            break;
        case SDL_USEREVENT:
            d = DOWN;
            if(!canimove(20,field,d)){
                shapetriger = true;
                break;
            }
            mshape(20,field,d);
            refresh(screen,20,field);
            break;
    }
}
SDL_Quit();

return 0;}
