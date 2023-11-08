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
#define PlayerXSpawnPoint 15



// GAMEPLAY FUNCTIONS

void StartingGame(SDL_Renderer *ren,int *playerX, int *playerY,int *wallY, int *wallH){
    *wallY = HEIGHT/4;
    *wallH = HEIGHT-HEIGHT/4;
    *playerY = HEIGHT/4 - 62;
    *playerX = PlayerXSpawnPoint;

    drawPlayer(ren,*playerX,*playerY);
    drawWalls(ren,wallY,wallH);
}

int startPlaying(SDL_Renderer *ren, int *playerX, int *playerY){
    *playerX = 40;
    drawPlayer(ren,*playerX,*playerY);
    return 1;
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


void drawWalls(SDL_Renderer *ren, int *wallY, int *wallH){
    if(SDL_SetRenderDrawColor(ren, 200,200,200, SDL_ALPHA_OPAQUE) != 0){
        SDL_ExitWithError("change color");
    }
    SDL_Rect leftWall;
    SDL_Rect rightWall;

    leftWall.x = 0;
    rightWall.x = WIDTH-40;

    leftWall.y = *wallY;
    rightWall.y = *wallY;

    leftWall.w = 40;
    rightWall.w = 40;

    leftWall.h = *wallH;
    rightWall.h = *wallH;

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
    if (*playerX >= WIDTH-40-PlayerWidth)
    {
        *playerX = WIDTH-40-PlayerWidth;
    }
    else{
        *playerX += 12;
    } 
    drawPlayer(ren,*playerX,*playerY);
}

void moveLeft(SDL_Renderer *ren,int *playerX, int *playerY){
    if (*playerX <= 40)
    {
        *playerX = 40;
    }
    else{
        *playerX -= 12;
    } 
    drawPlayer(ren,*playerX,*playerY);
}

void drawObstacle(SDL_Renderer* ren, Obstacle obstacle,int color[]) {
    SDL_SetRenderDrawColor(ren, color[0], color[1], color[2], SDL_ALPHA_OPAQUE);
    for (int i = 0; i < 2; i++) {
        SDL_RenderFillRect(ren, &obstacle.rects[i]);
    }
}

int checkCollisionObs(SDL_Renderer* ren,int playerX, int playerY){
    int pixel0[2] = {playerX + (PlayerWidth/4),playerY+(PlayerHeight/4)} ;
    int pixel1[2] = {playerX + (PlayerWidth-(PlayerWidth/4)),playerY+(PlayerHeight/4)} ;
    int pixel2[2] = {playerX + (PlayerWidth/4),playerY+(PlayerHeight-(PlayerHeight/4))} ;
    int pixel3[2] = {playerX + (PlayerWidth-(PlayerWidth/4)),playerY+(PlayerHeight-(PlayerHeight/4))} ;

    if(isPixelBlack(ren,pixel0[0],pixel0[1]) == 0 ||
        isPixelBlack(ren,pixel1[0],pixel1[1]) == 0 ||
        isPixelBlack(ren,pixel2[0],pixel2[1]) == 0 ||
        isPixelBlack(ren,pixel3[0],pixel3[1]) == 0 ){
            return 1;
        }
    else {
        return 0;
    }

}

// DYNAMIC LIST FUNCTIONS

void addObstaclesToEnd(ObstaclesNode** start, ObstaclesNode** end, Obstacle obstacle) {
    ObstaclesNode* newObstacleNode = (ObstaclesNode*)malloc(sizeof(ObstaclesNode));
    int rngx = rngXPos();
    obstacle.rects[0].x = rngx;
    if(obstacle.rects[1].x > obstacle.rects[0].x)
        obstacle.rects[1].x = rngx+15;
    newObstacleNode->obstacle = obstacle;
    newObstacleNode->next = NULL;

    if (*start == NULL) {
        *start = newObstacleNode;
        *end = newObstacleNode;
    } else {
        (*end)->next = newObstacleNode;
        *end = newObstacleNode;
    }
}

void drawObstacleList(SDL_Renderer *ren,ObstaclesNode* start,int color[]){
    ObstaclesNode* currentObstacle = start;
    while (currentObstacle != NULL) {
        drawObstacle(ren, currentObstacle->obstacle,color);      
        currentObstacle = currentObstacle->next;
            }
}

void upObstacleList(ObstaclesNode** start, ObstaclesNode** end) {
    ObstaclesNode* currentObstacle = *start;
    ObstaclesNode* previousObstacle = NULL;
    int obstacleH,obstacleY;
    
    while (currentObstacle != NULL) {
        obstacleY = currentObstacle->obstacle.rects[0].y;
        if(currentObstacle->obstacle.rects[0].h >= currentObstacle->obstacle.rects[1].h){
            obstacleH = currentObstacle->obstacle.rects[0].h;
        }
        else{
            obstacleH = currentObstacle->obstacle.rects[1].h;
        }
        currentObstacle->obstacle.rects[0].y -= 1;
        currentObstacle->obstacle.rects[1].y -= 1; 

        if ( (obstacleY + obstacleH) == 0) {
            ObstaclesNode* obstacleToRemove = currentObstacle;
            if (previousObstacle != NULL) {
                previousObstacle->next = currentObstacle->next;
            } 
            else {
                *start = currentObstacle->next;
            }
            if (currentObstacle == *end) {
                *end = previousObstacle;
            }

            free(obstacleToRemove);
            currentObstacle = currentObstacle->next;
        } else {
            previousObstacle = currentObstacle;
            currentObstacle = currentObstacle->next;
        }
    }
}

void removeAllObstacles(ObstaclesNode** start, ObstaclesNode** end) {
    ObstaclesNode* currentObstacle = *start;
    while (currentObstacle != NULL) {
        ObstaclesNode* nextObstacle = currentObstacle->next;
        free(currentObstacle); 
        currentObstacle = nextObstacle; 
    }
    
    *start = NULL;
    *end = NULL;
}

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

void changeColorSDL(int color[],int R, int G, int B){
    color[0] = R;
    color[1] = G;
    color[2] = B;
}

int isPixelBlack(SDL_Renderer *ren,int x, int y){
    // Créez une surface temporaire d'1x1 pixel pour lire la couleur du pixel.
    SDL_Surface* pixelSurface = SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0);

    // Utilisez SDL_RenderReadPixels pour lire la couleur du pixel à la position (x, y).
    SDL_Rect pixel;
    pixel.x = x;
    pixel.y = y;
    pixel.w = 1;
    pixel.h = 1;
    SDL_RenderReadPixels(ren, &pixel, SDL_PIXELFORMAT_ARGB8888, pixelSurface->pixels, pixelSurface->pitch);

    // Obtenez la couleur du pixel à partir de la surface temporaire.
    Uint32 pixelColor = *(Uint32*)pixelSurface->pixels;

    // Extrayez les composantes de couleur (rouge, vert, bleu).
    Uint8 r, g, b;
    SDL_GetRGB(pixelColor, pixelSurface->format, &r, &g, &b);

    // Libérez la mémoire de la surface temporaire.
    SDL_FreeSurface(pixelSurface);

    // Vérifiez si les composantes de couleur sont à 0 (noir).
    return ((r == 0 && g == 0 && b == 0));
    
}
