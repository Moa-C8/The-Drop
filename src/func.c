#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <time.h>

#include "obstacles.h"
#include "func.h"

#define WIDTH  800
#define HEIGHT 900
#define PlayerWidth 30
#define PlayerHeight 45
#define LimitFps 16
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
        *playerX += 10;
    } 
    drawPlayer(ren,*playerX,*playerY);
}

void moveLeft(SDL_Renderer *ren,int *playerX, int *playerY){
    if (*playerX <= 40)
    {
        *playerX = 40;
    }
    else{
        *playerX -= 10;
    } 
    drawPlayer(ren,*playerX,*playerY);
}

void drawObstacle(SDL_Renderer* ren, Obstacle obstacle,SDL_Color color) {
    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
    for (int i = 0; i < 2; i++) {
        SDL_RenderFillRect(ren, &obstacle.rects[i]);
    }
}

int checkColorCollision(SDL_Renderer* ren,int playerX, int playerY,SDL_Color drawColor){
    int pixel0[2] = {playerX+1,playerY+1};
    int pixel1[2] = {playerX-1 + PlayerWidth,playerY+1};
    int pixel2[2] = {playerX+1,playerY+PlayerHeight-1};
    int pixel3[2] = {playerX-1  + PlayerWidth,playerY+PlayerHeight-1};
    int pixel4[2] = {(playerX  + PlayerWidth/2),playerY+PlayerHeight};
    int pixel5[2] = {(playerX  + PlayerWidth/2),playerY};

    if(isPixelColor(ren,pixel0[0],pixel0[1],drawColor) != 0){
        return isPixelColor(ren,pixel0[0],pixel0[1],drawColor);
    }
    else if(isPixelColor(ren,pixel1[0],pixel1[1],drawColor) != 0){
        return isPixelColor(ren,pixel1[0],pixel1[1],drawColor);
    }
    else if(isPixelColor(ren,pixel2[0],pixel2[1],drawColor) != 0){
        return isPixelColor(ren,pixel2[0],pixel2[1],drawColor);
    }
    else if(isPixelColor(ren,pixel3[0],pixel3[1],drawColor) != 0){
        return isPixelColor(ren,pixel3[0],pixel3[1],drawColor);
    }
    else if(isPixelColor(ren,pixel4[0],pixel4[1],drawColor) != 0){
        return isPixelColor(ren,pixel4[0],pixel4[1],drawColor);
    }
    else if(isPixelColor(ren,pixel5[0],pixel5[1],drawColor) != 0){
        return isPixelColor(ren,pixel5[0],pixel5[1],drawColor);
    }

    return 0;

}

void writeScores(SDL_Renderer* ren,SDL_Color color, char* actScore, char* lastScore){

    TTF_Font *scoreFont = loadFont("src/assets/fonts/Roboto-Black.ttf",300);
    SDL_Color writingColor = {color.r,color.g,color.b,color.a};
    SDL_Surface* txtSurf = createTextSurf(scoreFont,actScore,writingColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, txtSurf);
    SDL_FreeSurface(txtSurf);
    SDL_Rect Texture;
    Texture.x = (WIDTH/2 - 60);
    Texture.y = (HEIGHT/8);
    Texture.w = 120;
    Texture.h = 120;
    writeLastScore(ren,lastScore);
    SDL_RenderCopy(ren,textTexture,NULL,&Texture);
    SDL_RenderPresent(ren);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(scoreFont);

}

void writeLastScore(SDL_Renderer* ren, char* score){

    TTF_Font *scoreFont = loadFont("src/assets/fonts/Roboto-Black.ttf",300);
    SDL_Color writingColor = {132,136,138,200};
    SDL_Surface* txtSurf = createTextSurf(scoreFont,score,writingColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, txtSurf);
    SDL_FreeSurface(txtSurf);
    SDL_Rect Texture;
    Texture.x = (WIDTH/2 - 25);
    Texture.y = (HEIGHT/12);
    Texture.w = 50;
    Texture.h = 50;

    SDL_RenderCopy(ren,textTexture,NULL,&Texture);
    SDL_RenderPresent(ren);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(scoreFont);

}

// DYNAMIC LIST FUNCTIONS
void addObstaclesToEnd(ObstaclesNode** start, ObstaclesNode** end, Obstacle obstacle) {
    ObstaclesNode* newObstacleNode = (ObstaclesNode*)malloc(sizeof(ObstaclesNode));
    int rngx = rngXPos();
    if(obstacle.rects[1].x > obstacle.rects[0].x)
        obstacle.rects[1].x = rngx+15;
    obstacle.rects[0].x = rngx;
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

void drawObstacleList(SDL_Renderer *ren,ObstaclesNode* start,SDL_Color color){
    ObstaclesNode* currentObstacle = start;
    while (currentObstacle != NULL) {
        drawObstacle(ren, currentObstacle->obstacle,color);      
        currentObstacle = currentObstacle->next;
            }
}

void upObstacleList(ObstaclesNode** start, ObstaclesNode** end, int speed) {
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
        currentObstacle->obstacle.rects[0].y -= (1+speed);
        currentObstacle->obstacle.rects[1].y -= (1+speed); 

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

void addBonusToEnd(BonusNode** start, BonusNode** end, Obstacle bonus) {
    BonusNode* newBonusNode = (BonusNode*)malloc(sizeof(BonusNode));

    if(bonus.rects[0].h != 120){
        int rngx = rngXPos();
        bonus.rects[0].x = rngx;
    }
    newBonusNode->bonus = bonus;
    newBonusNode->next = NULL;

    if (*start == NULL) {
        *start = newBonusNode;
        *end = newBonusNode;
    } else {
        (*end)->next = newBonusNode;
        *end = newBonusNode;
    }
}

void drawBonusList(SDL_Renderer *ren,BonusNode* start,SDL_Color colorApple,SDL_Color colorRope,SDL_Color colorBomb){
    BonusNode* currentBonus = start;
    SDL_Color color;
    while (currentBonus != NULL) {
        if(currentBonus->bonus.rects[0].h == 120){
            color = colorRope;
        }
        else if(currentBonus->bonus.rects[0].h == 15){
            color = colorBomb;
        }
        else if(currentBonus->bonus.rects[0].h == 10){
            color = colorApple;
        }
        drawObstacle(ren, currentBonus->bonus,color);      
        currentBonus = currentBonus->next;
        }
}

void upBonusList(BonusNode** start, BonusNode** end, int speed) {
    BonusNode* currentBonus = *start;
    BonusNode* previousBonus = NULL;
    int bonusH,bonusY;
    
    while (currentBonus != NULL) {
        bonusY = currentBonus->bonus.rects[0].y;
        bonusH = currentBonus->bonus.rects[0].h;

        currentBonus->bonus.rects[0].y -= (1+speed);

        if ( (bonusY + bonusH) == 0) {
            removeBonus(start, end, currentBonus, previousBonus);
            currentBonus = currentBonus->next;
        } 
        else {
            previousBonus = currentBonus;
            currentBonus = currentBonus->next;
        }
    }
}

void removeBonus(BonusNode** start, BonusNode** end, BonusNode* currentBonus, BonusNode* previousBonus){
    if (previousBonus != NULL) {
        previousBonus->next = currentBonus->next;
    }
    else {
            *start = currentBonus->next;
    }

    if (currentBonus == *end) {
        *end = previousBonus;
    }

    free(currentBonus);
}

void removeAllBonus(BonusNode** start, BonusNode** end) {
    BonusNode* currentBonus = *start;
    while (currentBonus != NULL) {
        BonusNode* nextBonus = currentBonus->next;
        free(currentBonus); 
        currentBonus = nextBonus; 
    }
    
    *start = NULL;
    *end = NULL;
}

// TTF FUNCTIONS

TTF_Font *loadFont(const char *path, int size){
    TTF_Font *font = TTF_OpenFont(path,size);
    if (!font) {
        printf("Erreur lors du chargement de la police : %s\n", TTF_GetError());
    }
    return font;
}

SDL_Surface *createTextSurf(TTF_Font* font, const char *text,SDL_Color color){
    return TTF_RenderText_Solid(font, text, color);
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
    k = rand() % (max);
    return k;
}

int rngXPos() {
    int k;
    int xPosTab[23] = {40, 70, 100, 130, 160, 190, 220, 250, 280, 310, 340, 370, 400, 430, 460, 490, 520, 550, 580, 610, 640, 670, 700};
    k = xPosTab[rng(23)];
    return(k);
}

int isPixelColor(SDL_Renderer *ren,int x, int y,SDL_Color drawColor){
    SDL_Color ColorApple = {135,20,142,255}; //purple Apple
    SDL_Color ColorRope = {158,58,14,255}; //Rope
    SDL_Color ColorBomb = {28,15,136,255}; // Bomb
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

    if((r == drawColor.r && g == drawColor.g && b == drawColor.b) == 1 ){
            return(1);
        }
    else if((r == ColorApple.r && g == ColorApple.g && b == ColorApple.b) == 1){
        return(2);
    }
    else if((r == ColorRope.r && g == ColorRope.g && b == ColorRope.b) == 1){
        return(3);
    }
    else if((r == ColorBomb.r && g == ColorBomb.g && b == ColorBomb.b) == 1){
        return(4);
    }
    return(0);   
}
