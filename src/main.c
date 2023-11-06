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




Obstacle predefinedObstacles[] = {
    {{{40, HEIGHT, 30, 60},{40, HEIGHT, 60, 30}} //L a l'envers
    },
    {{{40, HEIGHT, 60, 30},{55, HEIGHT, 30, 60}} // T
    },
    {{{40, HEIGHT, 60, 30},{55, HEIGHT-45, 30, 60}} // T a l'envers
    },
    {{40, HEIGHT,60,60} // carre 1x1
    },
    {{40,HEIGHT,60,30} // barre horizontale
    },
    {{40,HEIGHT,30,60} // barre Verticale
    }
};

int main(int argc, char** argv)
{
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0 ){
        SDL_ExitWithError("Init SDL");
    }

    if(SDL_CreateWindowAndRenderer(WIDTH,HEIGHT, 0, &win, &ren) != 0){
        SDL_ExitWithError("Window and renderer creation");
    }


/*-------------------------------------------------------------------------*/
            //VARIABLE Gameplay
/*-------------------------------------------------------------------------*/
    int *wy,*wh;
    wy = (int *)malloc(sizeof(int));
    wh = (int *)malloc(sizeof(int));

    if (wy != NULL && wh != NULL){
        *wy = HEIGHT/4;
        *wh = HEIGHT-HEIGHT/4;
    }else {
        SDL_ExitWithError("Malloc Wall");
    }

    int *py, *px;
    py = (int *)malloc(sizeof(int));
    px = (int *)malloc(sizeof(int));
    if (py != NULL && px != NULL) {
        *py = HEIGHT/4 - 62; // Attribuer une valeur de base à py
        *px = 15; // Attribuer une valeur de base à px
    } else {
        SDL_ExitWithError("Malloc Player");
    }

    int *oy;
    oy = (int *)malloc(sizeof(int));
    if (oy != NULL) {
        *oy = 150;
    } else {
        SDL_ExitWithError("Malloc Player");
    }

    SDL_bool running = SDL_TRUE;
    int playing = 0;
    unsigned int frame = 0;
    int i = rng(5);
    int x_base = rngXPos();
/*-------------------------------------------------------------------------*/
            //main loop
/*-------------------------------------------------------------------------*/
    drawPlayer(ren,*px,*py);
    drawWalls(ren,*wy,*wh);
    frame = SDL_GetTicks() + LimitFps;


    while (running)
    {   SDL_Event event;
        limit_FPS(frame);

        if(playing == 1){
            predefinedObstacles[i].rects[0].x = x_base;
            if(i == 1 | i == 2)
                predefinedObstacles[i].rects[1].x = x_base+15;
            else
                predefinedObstacles[i].rects[1].x = x_base;
            drawObstacle(ren, predefinedObstacles[i]);
            eraseGamingField(ren);
            drawPlayer(ren,*px,*py);
            *wy -= 1;
            *wh += 1;
            *oy += 1;
            predefinedObstacles[i].rects[1].y -= 1;
            predefinedObstacles[i].rects[0].y -= 1;
            drawObstacle(ren, predefinedObstacles[i]);
            drawWalls(ren,*wy,*wh);
        }

        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_RETURN:
                            if(playing == 0){
                                playing = 1;
                                StartingGame(ren,px,py);
                                
                                continue;}
                        case SDLK_a:
                            if(playing == 1){
                                moveLeft(ren,px,py);
                                continue;
                            }
                        case SDLK_d:
                            if(playing == 1){
                                moveRight(ren,px,py);
                                continue;
                            }
                        case SDLK_ESCAPE:
                            if(playing == 1){
                                playing = 2;
                            }
                            else if (playing == 2){
                                playing = 1;
                            }
                            continue;

                        default:
                            continue;

                    }
                case SDL_QUIT:
                    running = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }
        
        
        frame = SDL_GetTicks() + LimitFps;
        SDL_RenderPresent(ren);
    }
     

    
/*-------------------------------------------------------------------------*/
            //Close all
/*-------------------------------------------------------------------------*/
    free(py);
    free(px);
    free(wy);
    free(wh);
    free(oy);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}




