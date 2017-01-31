#ifndef SOURCE_H_INCLUDED
#define SOURCE_H_INCLUDED

#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <time.h>
#include <stdbool.h>

typedef struct cell{
    int lt_x;
    int lt_y;
    bool filled;
    Uint32* color;
    bool moving;
    bool fix;
    bool backup; //this will help with the rotation, u can pack them on each other.
    bool rotated;
    struct cell* right;
    struct cell* left;
    struct cell* down;
    struct cell* up;
    }cell;

typedef enum direction{
    LEFT,
    RIGHT,
    DOWN,
}direction;

void drawfield(SDL_Surface* output);//this shall draw the playfield

void gimmeashape(cell* given, bool play);// this builds up a random shape around the given cell

Uint32 settimer(Uint32 ms, void *param); // this shall set the timer

void refresh(SDL_Surface* output,/*int i,*/int y,cell given[/*i*/][y]);// this refreshes the screen

void movingfalse(int j, cell given[][j]);//this sets all cell.moving to false

bool canimove(int j,cell given[][j],direction d);// returns false when a cell cannot move to direction 'd'

void mshape(int j,cell given[][j],direction d);//moves a shape towards direction 'd'

int filledline(int j, cell given[][j]); // this is true when there is a filled line.

void rmline(int j, int r, cell given[][j]);//removes a filled line and move the ones above it down

bool rotatable(int j,cell[][j]);//this shall decide whether the moving shapes are rotatable

void rotateshape(SDL_Surface* output,int j,cell[][j]);// this function rotates a shape clockwise



#endif // SOURCE_H_INCLUDED
