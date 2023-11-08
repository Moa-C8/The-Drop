#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef OBSTACLES_H
#define OBSTACLES_H

typedef struct {
    SDL_Rect rects[2];
} Obstacle;

typedef struct ObstaclesNode {
    Obstacle obstacle;
    struct ObstaclesNode* next;
}ObstaclesNode;

#endif