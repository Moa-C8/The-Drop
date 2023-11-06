#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "obstacles.h"
#include "func.h"

#define WIDTH  800
#define HEIGHT 900
#define PlayerWidth 60
#define PlayerHeight 60
#define LimitFps 16
#define MAX_ACTIVE_OBSTACLES 6



// GAMEPLAY FUNCTIONS

void StartingGame(SDL_Renderer *ren, int *playerX, int *playerY){
    *playerX = 40;
    drawPlayer(ren,*playerX,*playerY);

}

void limit_FPS(unsigned int limit){
    unsigned int ticks = SDL_GetTicks();

    if(limit < ticks)
        return;
    else if(limit > ticks + 16){
        SDL_Delay(16);
    }
    else{
        SDL_Delay(limit-ticks);
    }
}

void eraseGamingField(SDL_Renderer *ren){
    if(SDL_SetRenderDrawColor(ren, 0,0,0, SDL_ALPHA_OPAQUE) != 0){
        SDL_ExitWithError("change color");
    }

    SDL_Rect Field;
    Field.x = 0;
    Field.y = 0;
    Field.h = HEIGHT;
    Field.w = WIDTH;

    if(SDL_RenderFillRect(ren, &Field) != 0){ 
        SDL_ExitWithError("can't draw player");
    }

}


void drawWalls(SDL_Renderer *ren, int wallY, int wallH){
    if(SDL_SetRenderDrawColor(ren, 200,200,200, SDL_ALPHA_OPAQUE) != 0){
        SDL_ExitWithError("change color");
    }
    SDL_Rect leftWall;
    SDL_Rect rightWall;

    leftWall.x = 0;
    rightWall.x = WIDTH-40;

    leftWall.y = wallY;
    rightWall.y = wallY;

    leftWall.w = 40;
    rightWall.w = 40;

    leftWall.h = wallH;
    rightWall.h = wallH;

    if(SDL_RenderFillRect(ren, &leftWall) != 0){ 
                SDL_ExitWithError("can't draw left wall");
            }
    if(SDL_RenderFillRect(ren, &rightWall) != 0){ 
                SDL_ExitWithError("can't draw right wall");
            }
}

void drawPlayer(SDL_Renderer *ren, int playerX, int playerY){
    if(SDL_SetRenderDrawColor(ren, 0,200,0, SDL_ALPHA_OPAQUE) != 0){
        SDL_ExitWithError("change color");
    }

    SDL_Rect player;
    player.y = playerY;
    player.x = playerX;
    player.w = PlayerWidth;
    player.h = PlayerHeight;
    
    if(SDL_RenderFillRect(ren, &player) != 0){ 
                SDL_ExitWithError("can't draw player");
    }
}

void moveRight(SDL_Renderer *ren,int *playerX, int *playerY){
    *playerX += 12;
    drawPlayer(ren,*playerX,*playerY);
}

void moveLeft(SDL_Renderer *ren,int *playerX, int *playerY){
    *playerX -= 12;
    drawPlayer(ren,*playerX,*playerY);
}

void drawObstacle(SDL_Renderer* ren, Obstacle obstacle) {
    SDL_SetRenderDrawColor(ren, 50, 0, 200, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < 2; i++) {
        SDL_RenderFillRect(ren, &obstacle.rects[i]);
    }
}

// DYNAMIC LIST FUNCTIONS


// OTHERS FUNCTIONS

void SDL_ExitWithError(const char *errorMsg) {
    SDL_Log("Error ; %s > %s\n", errorMsg, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void SDL_DestroyAndExit(SDL_Window *win,SDL_Renderer *ren,const char *errorMsg) {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_ExitWithError(errorMsg);
}

int rng(int max) {
    int k;
    k = rand() % (max+1);
    return k;
}

int rngXPos() {
    int k;
    int xPosTab[12] = {40,100,160,220,280,340,400,460,520,580,640,700};
    k = xPosTab[rng(11)];
    return(k);
}
