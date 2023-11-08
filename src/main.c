#include <SDL.h>
#include <SDL_ttf.h>
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




Obstacle predefinedObstacles[] = {
    {{{40, HEIGHT, 30, 60},{40, HEIGHT, 60, 30}} //L a l'envers
    },
    {{{100, HEIGHT, 60, 30},{115, HEIGHT, 30, 60}} // T
    },
    {{{160, HEIGHT, 60, 30},{175, HEIGHT-45, 30, 60}} // T a l'envers
    },
    {{220, HEIGHT,60,60} // carre 1x1
    },
    {{280,HEIGHT,60,30} // barre horizontale
    },
    {{340,HEIGHT,30,60} // barre Verticale
    }
};

int main(int argc, char** argv)
{
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    SDL_PixelFormat *pixelFormat = NULL;
    
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0 ){
        SDL_ExitWithError("Init SDL");
    }

    if(SDL_CreateWindowAndRenderer(WIDTH,HEIGHT, 0, &win, &ren) != 0){
        SDL_ExitWithError("Window and renderer creation");
    }

    if (TTF_Init() == -1){
        SDL_ExitWithError("TTF");
    }


    
/*-------------------------------------------------------------------------*/
            //VARIABLE Gameplay
/*-------------------------------------------------------------------------*/
    int wallY,wallH,*ptrWallY,*ptrWallH;
    wallY = 0;
    wallH = 0;
    ptrWallY = &wallY;
    ptrWallH = &wallH;

    int playerY,playerX, *ptrPlayerX,*ptrPlayerY;
    playerY = 0;
    playerX = 0;
    ptrPlayerX = &playerX;
    ptrPlayerY = &playerY;

    SDL_bool running = SDL_TRUE;
    int playing = 0;
    float frame = 0;
    unsigned int tickMs = 0;
    unsigned int lastScoreTime = 0;
    unsigned int score = 0;

    float fpsLimit = LimitFps;
    ObstaclesNode* obstaclesListStart = NULL;
    ObstaclesNode* obstaclesListEnd = NULL;
    pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    int drawColor[3] = {239,152,18};

/*-------------------------------------------------------------------------*/
            //main loop
/*-------------------------------------------------------------------------*/
    StartingGame(ren,ptrPlayerX,ptrPlayerY,ptrWallY,ptrWallH);
    frame = SDL_GetTicks() + fpsLimit;
    addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[rng(5)]);

    while (running)
    {   SDL_Event event;
        limit_FPS(frame);

        if(playing == 1){
            unsigned int currentTime = SDL_GetTicks();
            if (currentTime - lastScoreTime >= 1000) {
                if(score == 0){
                    changeColorSDL(drawColor,234,214,12);
                    fpsLimit = 16;
                }
                if (score >= 50 && score< 150){
                    changeColorSDL(drawColor,239,152,18);
                    fpsLimit -= fpsLimit/4;
                }
                if(score >= 150 && score < 350){
                    changeColorSDL(drawColor,224,39,39);
                    fpsLimit -= fpsLimit/3;
                }
                if(score >= 350){
                    changeColorSDL(drawColor,239,12,219);
                    fpsLimit -= fpsLimit/2;
                }
                lastScoreTime = currentTime;
                score++;
            }
            if (obstaclesListEnd->obstacle.rects[0].y < HEIGHT-180) {
                int k = rng(10);
                while( k > 0){
                    addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[rng(5)]);
                    k--;
                }
                }

            drawObstacleList(ren,obstaclesListStart,drawColor);
            eraseGamingField(ren);
            wallY -= 1;
            wallH += 1;
            drawWalls(ren,ptrWallY,ptrWallH);
            upObstacleList(&obstaclesListStart,&obstaclesListEnd);
            drawObstacleList(ren,obstaclesListStart,drawColor);
            if(checkCollisionObs(ren, *ptrPlayerX, *ptrPlayerY)) {

                char Txtscore[20];
                sprintf(Txtscore, "%d", score);
                eraseGamingField(ren);  
                removeAllObstacles(&obstaclesListStart,&obstaclesListEnd);
                addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[rng(5)]);

                TTF_Font *scoreFont = loadFont("src/assets/fonts/Roboto-Black.ttf",200);
                SDL_Color writingColor = {255,255,255,255};
                SDL_Surface* txtSurf = createTextSurf(scoreFont,Txtscore,writingColor);
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, txtSurf);
                SDL_FreeSurface(txtSurf);
                SDL_Rect Texture;
                Texture.x = (WIDTH/2 - 75);
                Texture.y = (HEIGHT/8);
                Texture.w = 150;
                Texture.h = 100;

                SDL_RenderCopy(ren,textTexture,NULL,&Texture);
                SDL_RenderPresent(ren);
                SDL_Delay(1000);
                SDL_DestroyTexture(textTexture);
                TTF_CloseFont(scoreFont);
                StartingGame(ren,ptrPlayerX,ptrPlayerY,ptrWallY,ptrWallH);
                playing = 0;
                score = 0;
                lastScoreTime = 0;

                
            }
            drawPlayer(ren,*ptrPlayerX,*ptrPlayerY);
        }

        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_RETURN:
                            if(playing == 0){
                                playing = startPlaying(ren,ptrPlayerX,ptrPlayerY);
                                
                                continue;}
                        case SDLK_a:
                            if(playing == 1){
                                moveLeft(ren,ptrPlayerX,ptrPlayerY);
                                continue;
                            }
                        case SDLK_d:
                            if(playing == 1){
                                moveRight(ren,ptrPlayerX,ptrPlayerY);
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
        frame = SDL_GetTicks() + fpsLimit;
        SDL_RenderPresent(ren);
    }
     

    
/*-------------------------------------------------------------------------*/
            //Close all
/*-------------------------------------------------------------------------*/
    
    SDL_FreeFormat(pixelFormat);
    removeAllObstacles(&obstaclesListStart,&obstaclesListEnd);    
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}




