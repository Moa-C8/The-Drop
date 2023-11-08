#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "obstacles.h"
#ifndef FUNC_H
#define FUNC_H

// GAMEPLAY FUNCTIONS
void StartingGame(SDL_Renderer *ren,int *playerX, int *playerY,int *wallY, int *wallH);
int startPlaying(SDL_Renderer *ren, int *playerX, int *playerY);
void limit_FPS(unsigned int limit);
void eraseGamingField(SDL_Renderer *ren);
void drawWalls(SDL_Renderer *ren, int *wallY, int *wallH);
void drawPlayer(SDL_Renderer *ren, int playerX, int playerY);
void moveRight(SDL_Renderer *ren,int *playerX, int *playerY);
void moveLeft(SDL_Renderer *ren,int *playerX, int *playerY);
void drawObstacle(SDL_Renderer* ren, Obstacle obstacle,int color[]);
int checkCollisionObs(SDL_Renderer* ren,int playerX, int playerY);

// DYNAMIC LIST FUNCTIONS
void addObstaclesToEnd(ObstaclesNode** start, ObstaclesNode** end, Obstacle obstacle);
void drawObstacleList(SDL_Renderer *ren,ObstaclesNode* start,int color[]);
void upObstacleList(ObstaclesNode** start, ObstaclesNode** end);
void removeAllObstacles(ObstaclesNode** start, ObstaclesNode** end);

TTF_Font *loadFont(const char *path, int size);
SDL_Surface *createTextSurf(TTF_Font* font, const char *text,SDL_Color color);


// OTHERS FUNCTIONS
void SDL_ExitWithError(const char *errorMsg);
void SDL_DestroyAndExit(SDL_Window *win,SDL_Renderer *ren,const char *errorMsg);
int rng(int max);
int rngXPos();
void changeColorSDL(int color[],int R, int G, int B);
int isPixelColor(SDL_Renderer *ren,int x, int y,int R,int G, int B);


#endif