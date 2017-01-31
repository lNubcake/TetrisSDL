#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <stdbool.h>

#include "source.h"
#include "colors.h"

Uint32 settimer(Uint32 ms, void *param){
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

void drawfield(SDL_Surface* output){
    boxColor(output,0,0,330,630,0xafafafff);//grey background
    boxColor(output,15,15,315,615,0x000000ff);// black playground
    int vert,hor;
    for(vert = 1; vert < 20; vert++)
        lineColor(output,15,15+(vert*30),315,15+(vert*30),0xafafafff);
    for(hor = 1; hor < 10; hor++)
        lineColor(output,15+(hor*30),15,15+(hor*30),615,0xafafafff);
    SDL_Flip(output);
}

void gimmeashape(cell* given,bool play){

    srand(time(NULL));

    int r = rand() % 7 + 1;
    int r_color;
    if(given->filled == true || given->down->filled == true){
        play = false;
        return;
    }
    r_color = rand() % 8;

    given->filled = true; given->moving = true; given->color = &color[r_color];
    given->down->filled = true; given->down->moving = true; given->down->color = &color[r_color];
        switch(r){
                case 1:
                    if(given->down->down->filled == true || given->down->down->right->filled == true){
                        play = false;
                        break;
                    }
    /*   x      */      given->down->down->filled=true ; given->down->down->moving=true; given->down->down->color = &color[r_color]; given->down->down->fix = true;
    /*   x      */      given->down->down->right->filled=true;given->down->down->right->moving=true; given->down->down->right->color = &color[r_color];
    /*   ox     */      break;
                case 2:
                    if(given->down->down->filled == true || given->down->down->down->filled == true){
                        play = false;
                        break;
    /*    x    */   }
    /*    x    */   given->down->down->filled=true;given->down->down->moving=true; given->down->down->color=&color[r_color];
    /*    x    */   given->down->down->down->filled=true;given->down->down->down->moving=true;given->down->down->down->color = &color[r_color]; given->down->down->down->fix = true;
    /*    o    */   break;
                case 3:
                    if(given->down->down->filled == true || given->down->down->left->filled == true){
                        play = false;
                        break;
                    }
    /*    x    */   given->down->down->filled=true;given->down->down->moving=true;given->down->down->color = &color[r_color];given->down->down->fix = true;
    /*    x    */   given->down->down->left->filled=true;given->down->down->left->moving=true;given->down->down->left->color = &color[r_color];
    /*   xo    */   break;
                case 4:
                    if(given->right->filled == true || given->down->left->filled == true){
                        play = false;
                        break;
                    }
                    given->down->fix = true;
    /*     xx   */  given->right->filled=true;given->right->moving=true;given->right->color=&color[r_color];
    /*    xo    */  given->down->left->filled=true;given->down->left->moving=true;given->down->left->color=&color[r_color];
    /*          */  break;
                case 5:
                    if(given->left->filled == true || given->down->right->filled == true){
                        play = false;
                        break;
                    }
                    given->down->fix = true;
    /*   xx     */  given->left->filled=true;given->left->moving=true;given->left->color=&color[r_color];
    /*    ox    */  given->down->right->filled=true;given->down->right->moving=true;given->down->right->color=&color[r_color];
    /*          */  break;
                case 6:
                    if(given->right->filled == true || given->down->right->filled == true){
                        play = false;
                        break;
                    }
                    given->fix = true; given->down->fix = true; given->right->fix = true; given->down->right->fix = true;
    /*   oo */      given->right->filled=true;given->right->moving=true;given->right->color = &color[r_color];
    /*   oo */      given->down->right->filled=true;given->down->right->moving=true;given->down->right->color = &color[r_color];
    /*      */      break;
                case 7:
                    if(given->down->down->filled == true || given->down->right->filled == true){
                        play = false;
                        break;
                    }
                    given->down->fix = true;
    /*   x  */      given->down->down->filled=true;given->down->down->moving=true;given->down->down->color=&color[r_color];
    /*   ox */      given->down->right->filled=true;given->down->right->moving=true;given->down->right->color=&color[r_color];
    /*   x  */      break;
    }
}

void refresh(SDL_Surface* output,/*int i,*/int y, cell given[/*i*/][y]){
    int x;
    for (x = 0; x < 10; x++){
        for (y = 0; y < 20; y++){
            if(given[x][y].filled){
                boxColor(output,given[x][y].lt_x+1,given[x][y].lt_y+1,given[x][y].lt_x+29,given[x][y].lt_y+29,*(given[x][y].color));
                boxColor(output,given[x][y].lt_x+5,given[x][y].lt_y+5,given[x][y].lt_x+25,given[x][y].lt_y+25,0x000000ff);
            }
            else
                boxColor(output,given[x][y].lt_x+1,given[x][y].lt_y+1,given[x][y].lt_x+29,given[x][y].lt_y+29,0x000000ff);
        }
    }
    SDL_Flip(output);
}

void movingfalse(int j, cell given[][j]){
    int i;
    for(i = 0; i < 10; i++){
        for(j = 0; j < 20; j++){
            given[i][j].moving = false;
            given[i][j].fix = false;
            given[i][j].backup = false;
            given[i][j].rotated = false;
        }
    }
}

bool canimove(int j,cell given[][j],direction d){
    int i;
    switch(d){
        case LEFT:
            for(i = 0; i < 10; i++){
                for(j = 0; j < 20; j++){
                    if(given[i][j].moving && given[i][j].left == NULL)
                        return false;
                    if(given[i][j].moving && given[i][j].left->moving == false && given[i][j].left->filled == true)
                        return false;
                }
            }
            break;
        case RIGHT:
            for(i = 0; i < 10; i++){
                for(j = 0; j < 20; j++){
                    if(given[i][j].moving && given[i][j].right == NULL)
                        return false;
                    if(given[i][j].moving && given[i][j].right->moving == false && given[i][j].right->filled == true)
                        return false;
                }
            }
            break;
        case DOWN:
            for(i = 0; i < 10; i++){
                for(j = 0; j < 20; j++){
                    if(given[i][j].moving && given[i][j].down == NULL)
                        return false;
                    if(given[i][j].moving && given[i][j].down->moving == false && given[i][j].down->filled == true)
                        return false;
                }
            }
            break;
    }
    return true;
}

void mshape(int j,cell given[][j],direction d){
    int i;
    switch(d){
        case LEFT:
            for(i = 0; i < 10; i++){
                for(j = 0; j < 20; j++){
                    if(given[i][j].moving){
                        given[i][j].moving = false;
                        given[i][j].filled = false;
                        given[i][j].left->color = given[i][j].color;
                        given[i][j].color = NULL;
                        given[i][j].left->fix = given[i][j].fix;
                        given[i][j].fix = false;
                        given[i][j].left->moving = true;
                        given[i][j].left->filled = true;
                    }
                }
            }
            break;
        case RIGHT:
            for(i = 9; i >= 0; i--){
                for(j = 0; j < 20; j++){
                    if(given[i][j].moving){
                        given[i][j].moving = false;
                        given[i][j].filled = false;
                        given[i][j].right->color = given[i][j].color;
                        given[i][j].color = NULL;
                        given[i][j].right->fix = given[i][j].fix;
                        given[i][j].fix = false;
                        given[i][j].right->moving = true;
                        given[i][j].right->filled = true;
                    }
                }
            }
            break;
        case DOWN:
            for(j = 19; j >= 0; j--){
                for(i = 0; i < 10; i++){
                    if(given[i][j].moving){
                        given[i][j].moving = false;
                        given[i][j].filled = false;
                        given[i][j].down->color = given[i][j].color;
                        given[i][j].color = NULL;
                        given[i][j].down->fix = given[i][j].fix;
                        given[i][j].fix = false;
                        given[i][j].down->moving = true;
                        given[i][j].down->filled = true;
                    }
                }
            }
            break;
    }
}

int filledline(int j, cell given[][j]){
    for(j = 19; j >= 0 ; j--){
        if(given[0][j].filled && given[1][j].filled &&given[2][j].filled &&given[3][j].filled &&given[4][j].filled &&given[5][j].filled &&given[6][j].filled &&given[7][j].filled &&given[8][j].filled &&given[9][j].filled)
            return j;
    }
    return -1;
}

void rmline(int j,int r, cell given[][j]){
    int a, b;
    for(b = r-1; b>=0; b--){
        for(a = 0; a < 10; a++){
            given[a][b].down->filled = given[a][b].filled;
            given[a][b].down->color = given[a][b].color;
            if(b == 0){
                given[a][b].filled = false;
            }
        }
    }
}

bool rotatable(int j,cell given[][j]){
    int fix_i;
    int fix_j;
    int i;
    for(i = 0; i < 10; i++){
        for(j = 0; j > -20; j--){
            if(given[i][-1*j].fix){
                fix_i = i;
                fix_j = j;
            }
        }
    }
    for(i = 0; i < 10; i++){
        for(j = 0; j > -20; j--){
            if(given[i][-1*j].moving == true && given[i][-1*j].fix == false){
                int a; int b; int c; int d;
                a = i - fix_i;
                b = j - fix_j;
                c = a;
                d = b;
                a = (-1*d) + fix_i;
                b = c + fix_j;
                if(a < 0 || a > 9 || -1*b < 0 || -1*b > 19 || (given[a][-1*b].filled == true && given[a][-1*b].moving == false))
                    return false;
            }
        }
    }
    return true;
}

void rotateshape(SDL_Surface* output,int j, cell given[][j]){
    int fix_i;
    int fix_j;
    int i;
    for(i = 0; i < 10; i++){
        for(j = 0; j > -20; j--){
            if(given[i][-1*j].fix){
                fix_i = i;
                fix_j = j;
            }
        }
    }
    for(i = 0; i < 10; i++){
        for(j = 0; j > -20; j--){
            if(given[i][-1*j].moving == true && given[i][-1*j].fix == false && given[i][-1*j].rotated == false){
                int a; int b; int c; int d;
                a = i - fix_i;
                b = j - fix_j;
                c = a;
                d = b;
                a = (-1*d) + fix_i;
                b = c + fix_j;
                if(given[a][-b].moving == true && given[i][-j].backup == false){
                    given[a][-b].backup = true;
                    given[i][-j].filled = false;
                    given[i][-j].moving = false;
                    given[i][-j].color = NULL;
                    }
                else if(given[i][-j].backup == true){
                    given[i][-j].backup = false;
                    if(given[a][-b].moving == true){
                        given[a][-b].backup = true;
                    }
                    else{
                    given[a][-b].filled = true;
                    given[a][-b].moving = true;
                    given[a][-b].rotated = true;
                    given[a][-b].color = given[i][-j].color;
                    }
                }
                else if(given[a][-b].moving == false && given[i][-j].backup == false){
                given[a][-b].filled = true; given[i][-j].filled = false;
                given[a][-b].moving = true; given[i][-j].moving = false;
                given[a][-b].color = given[i][-j].color; given[i][-j].color = NULL;
                given[a][-b].rotated = true;
                }
                refresh(output,20,given);
            }
        }
    }
    for(i = 0; i < 10; i++)
        for(j = 0; j < 20; j++)
            given[i][j].rotated = false;
}
