#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef OBSTACLES_H
#define OBSTACLES_H

typedef struct {
    SDL_Rect rects[2]; // Un tableau de 3 SDL_Rect pour chaque obstacle
} Obstacle;


#endif