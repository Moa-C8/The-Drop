#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "obstacles.h"
#include "func.h"

#define WIDTH  800
#define HEIGHT 900
#define PlayerWidth 30
#define PlayerHeight 45
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
    SDL_Color drawColor;
    SDL_Color obsColor0 = {236,226,105,255};
    SDL_Color obsColor1 = {234,214,12,255};
    SDL_Color obsColor2 = {239,152,18,255};
    SDL_Color obsColor3 = {224,39,39,255};
    SDL_Color obsColor4 = {255,0,0,255};
    SDL_Color bonusColor0 = {135,20,142,255};
    SDL_Color White = {255,255,255,255};
    SDL_Color Black = {0,0,0,255};
    SDL_Color Green = {0,255,0,255};
    SDL_Color Red = {255,0,0,255};
    SDL_Color Blue = {0,0,255,255};

    int baseSpeed = 1;
    int scoreSpeed = 1;
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
                    drawColor = obsColor0;
                    fpsLimit = 16;
                    scoreSpeed = 1;
                }
                if (score >= 50 && score< 150){
                    drawColor = obsColor1;
                    scoreSpeed = 4;
                }
                if(score >= 150 && score < 350){
                    drawColor = obsColor2;
                    scoreSpeed = 7;
                }
                if(score >= 350 && score < 500){
                    drawColor = obsColor3;
                    scoreSpeed = 9;
                }
                if(score >= 500){
                    drawColor = obsColor4;
                    scoreSpeed = 12;
                }
                lastScoreTime = currentTime;
                if(baseSpeed > 1){
                    score = score + baseSpeed;
                }
                else{
                    score++;
                }
                
            }
            // condition pour creer des obstacles avec toujours la meme distance hauteur
            if (obstaclesListEnd->obstacle.rects[0].y < HEIGHT-180) {
                int k = rng(10);
                while( k > 0){
                    addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[rng(5)]);
                    k--;
                }
                }

            drawObstacleList(ren,obstaclesListStart,drawColor);
            eraseGamingField(ren);
            wallY -= 1 + baseSpeed;
            wallH += 1 + baseSpeed;
            drawWalls(ren,ptrWallY,ptrWallH);
            upObstacleList(&obstaclesListStart,&obstaclesListEnd,(baseSpeed+scoreSpeed));
            drawObstacleList(ren,obstaclesListStart,drawColor);
            //condition eliminatoire du joueur
            if(checkColorCollision(ren, *ptrPlayerX, *ptrPlayerY,drawColor)) {
                eraseGamingField(ren);  
                removeAllObstacles(&obstaclesListStart,&obstaclesListEnd);
                addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[rng(5)]);

                char actScore[20];
                sprintf(actScore, "%d", score);               
                char lastScore[20];
                FILE *filScore = fopen("src/assets/file/score.txt","r+");
                if (filScore == NULL) {
                    fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
                    exit(EXIT_FAILURE);
                }

                fgets(lastScore, 255, filScore);
                int lastScoreInt = atoi(lastScore);
                int actScoreInt = atoi(actScore);
                fseek(filScore, 0, SEEK_SET);
                if (lastScoreInt < actScoreInt) {
                    fputs(actScore, filScore);
                    writeScores(ren,Green,actScore,lastScore);
                }
                else{
                    writeScores(ren,Red,actScore,lastScore);
                }
                fclose(filScore);           

                
                
                StartingGame(ren,ptrPlayerX,ptrPlayerY,ptrWallY,ptrWallH);
                playing = 0;
                score = 0;
                lastScoreTime = 0;
                baseSpeed = 1;
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
                        case SDLK_a :
                            if(playing == 1){
                                moveLeft(ren,ptrPlayerX,ptrPlayerY);
                                continue;
                            }
                        case SDLK_LEFT:
                            if(playing == 1){
                                moveLeft(ren,ptrPlayerX,ptrPlayerY);
                                continue;
                            }
                        case SDLK_d :
                            if(playing == 1){
                                moveRight(ren,ptrPlayerX,ptrPlayerY);
                                continue;
                            }
                        case SDLK_RIGHT:
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

                        case SDLK_s:
                            if(playing == 1){
                                if(baseSpeed < 8){
                                    baseSpeed *= 2;}
                                else{
                                    continue;
                                }}

                        default:
                            continue;
                    }

                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_s:
                            if(playing == 1){
                                if(baseSpeed > 1){
                                    baseSpeed = 1;}
                            }

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




