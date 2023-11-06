#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "obstacles.h"
#ifndef FUNC_H
#define FUNC_H

void SDL_ExitWithError(const char *errorMsg);
void SDL_DestroyAndExit(SDL_Window *win,SDL_Renderer *ren,const char *errorMsg);
int rng(int max);
int rngXPos();

void StartingGame(SDL_Renderer *ren, int *playerX, int *playerY);
void limit_FPS(unsigned int limit);
void eraseGamingField(SDL_Renderer *ren);

void drawPlayer(SDL_Renderer *ren, int playerX, int playerY);
void moveRight(SDL_Renderer *ren,int *playerX, int *playerY);
void moveLeft(SDL_Renderer *ren,int *playerX, int *playerY);

void drawWalls(SDL_Renderer *ren, int wallY, int wallH);
void scrollWalls(SDL_Renderer *ren, int *wallY, int *wallH);

void drawObstacle(SDL_Renderer* ren, Obstacle obstacle);


#endif