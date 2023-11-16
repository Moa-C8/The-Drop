#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "obstacles.h"
#ifndef FUNC_H
#define FUNC_H

// GAMEPLAY FUNCTIONS
void StartingGame(SDL_Renderer *ren,int *playerX, int *playerY,int *wallY, int *wallH,SDL_Texture *playerTexture);
int startPlaying(SDL_Renderer *ren, int *playerX, int *playerY,SDL_Texture *playerTexture);
void limit_FPS(unsigned int limit);
void eraseGamingField(SDL_Renderer *ren);
void drawWalls(SDL_Renderer *ren, int *wallY, int *wallH);
void drawPlayer(SDL_Renderer *ren, int playerX, int playerY,SDL_Texture *playerTexture);
void moveRight(SDL_Renderer *ren,int *playerX, int *playerY,SDL_Texture *playerTexture);
void moveLeft(SDL_Renderer *ren,int *playerX, int *playerY,SDL_Texture *playerTexture);
void drawObstacle(SDL_Renderer* ren, Obstacle obstacle,SDL_Color color);
int checkColorCollision(SDL_Renderer* ren,int playerX, int playerY,SDL_Color drawColor);
void writeScores(SDL_Renderer* ren,SDL_Color color, char* actScore, char* lastScore);
void writeLastScore(SDL_Renderer* ren, char* score);



// DYNAMIC LIST FUNCTIONS
void addObstaclesToEnd(ObstaclesNode** start, ObstaclesNode** end, Obstacle obstacle);
void drawObstacleList(SDL_Renderer *ren,ObstaclesNode* start,SDL_Color color);
void upObstacleList(ObstaclesNode** start, ObstaclesNode** end, int vitesse);
void removeAllObstacles(ObstaclesNode** start, ObstaclesNode** end);

void addBonusToEnd(BonusNode** start, BonusNode** end, Obstacle bonus);
void drawBonusList(SDL_Renderer *ren,BonusNode* start,SDL_Color colorApple,SDL_Color colorRope,SDL_Color colorBomb);
void upBonusList(BonusNode** start, BonusNode** end, int speed);
void removeBonus(BonusNode** start, BonusNode** end, BonusNode* currentBonus, BonusNode* previousBonus);
void removeAllBonus(BonusNode** start, BonusNode** end);

// TTF FUNCTIONS

TTF_Font *loadFont(const char *path, int size);
SDL_Surface *createTextSurf(TTF_Font* font, const char *text,SDL_Color color);

// OTHERS FUNCTIONS
void SDL_ExitWithError(const char *errorMsg);
void SDL_DestroyAndExit(SDL_Window *win,SDL_Renderer *ren,const char *errorMsg);
int rng(int max);
int rngXPos();
int isPixelColor(SDL_Renderer *ren,int x, int y,SDL_Color drawColor);
SDL_Texture *loadTexture(SDL_Renderer *ren,const char *filePath);

#endif