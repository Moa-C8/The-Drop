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

Obstacle bonus[] = {
    {{40,HEIGHT,20,120}}, // corde0
    {{WIDTH-60,HEIGHT,20,120}}, // corde1
    {{100,HEIGHT,10,10}}, // apple
    {{100,HEIGHT,15,15}} //Bomb
};

int main(int argc, char** argv)
{
    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    SDL_PixelFormat *pixelFormat = NULL;
    pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
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

    SDL_Surface *icon = SDL_LoadBMP("src/assets/img/Logo.bmp");
    if(icon == NULL){
        SDL_FreeSurface(icon);
        SDL_ExitWithError("Icon image");        
    }

    SDL_SetWindowIcon(win,icon);
    SDL_FreeSurface(icon);
    SDL_SetWindowTitle(win,"Drop");

    SDL_Texture *playerTexture=loadTexture(ren,"src/assets/img/player.bmp");
    SDL_Texture *startPlayerText = loadTexture(ren,"src/assets/img/player1.bmp");
    SDL_Texture *startBg = loadTexture(ren,"src/assets/img/StartBackground.bmp");
    SDL_Texture *bg0 = loadTexture(ren,"src/assets/img/GameBackGround0.bmp");
    SDL_Texture *bg1 = loadTexture(ren,"src/assets/img/GameBackGround1.bmp");

    int bg = 0;

/*-------------------------------------------------------------------------*/
            //VARIABLE Gameplay
/*-------------------------------------------------------------------------*/
    //Walls variable
    int wallY,wallY1,*ptrWallY,*ptrWallY1;
    wallY = 0;
    wallY1 = 899;
    ptrWallY = &wallY;
    ptrWallY1 = &wallY1;
    //Player variable
    int playerY,playerX, *ptrPlayerX,*ptrPlayerY;
    playerY = 0;
    playerX = 0;
    ptrPlayerX = &playerX;
    ptrPlayerY = &playerY;
    //gameplay variable
    SDL_bool running = SDL_TRUE;
    int playing = 0;
    float frame = 0;
    unsigned int tickMs = 0;
    unsigned int lastScoreTime = 0;
    unsigned int score = 0;
    int baseSpeed = 1;
    int scoreSpeed = 1;
    int bonusSpeed = 0;
    int speed;
    int spawn = 0;
    //Dynamic list
    ObstaclesNode* obstaclesListStart = NULL;
    ObstaclesNode* obstaclesListEnd = NULL;
    BonusNode* BonusListStart = NULL;
    BonusNode* BonusListEnd = NULL;
    // All colors used
    SDL_Color obsColor0 = {236,226,105,255};
    SDL_Color obsColor1 = {234,214,12,255};
    SDL_Color obsColor2 = {239,152,18,255};
    SDL_Color obsColor3 = {224,39,39,255};
    SDL_Color obsColor4 = {255,0,0,255};
    SDL_Color ColorApple = {135,20,142,255}; //purple Apple
    SDL_Color ColorRope = {158,58,14,255}; //Rope
    SDL_Color ColorBomb = {28,15,136,255}; // Bomb
    SDL_Color White = {255,255,255,255};
    SDL_Color Black = {0,0,0,255};
    SDL_Color Green = {0,255,0,255};
    SDL_Color Red = {255,0,0,255};
    SDL_Color Blue = {0,0,255,255};
    SDL_Color drawColor = obsColor0;
/*-------------------------------------------------------------------------*/
            //main loop
/*-------------------------------------------------------------------------*/
    StartingGame(ren,ptrPlayerX,ptrPlayerY,ptrWallY,startPlayerText,startBg);
    SDL_DestroyTexture(startPlayerText);
    frame = SDL_GetTicks() + LimitFps;
    addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[rng(6)]);

    while (running)
    {   SDL_Event event;
        limit_FPS(frame);

        if(playing == 1){
            
            unsigned int currentTime = SDL_GetTicks();
            // score condition every secondes
            if (currentTime - lastScoreTime >= 1000) {
                if(score == 0){
                    // recreate dynamic list and set variable as default if restart
                    ObstaclesNode* obstaclesListStart = NULL;
                    ObstaclesNode* obstaclesListEnd = NULL;
                    BonusNode* BonusListStart = NULL;
                    BonusNode* BonusListEnd = NULL;
                    drawColor = obsColor0;
                    scoreSpeed = 1;
                    spawn = 0;
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
                spawn += 1;
                
            }
            // condition to create Obstacle after increasing the last cycle by 180 y
            if (obstaclesListEnd->obstacle.rects[0].y < HEIGHT-180) {
                int k = rng(11);
                while( k > 0){
                    addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[rng(6)]);
                    k--;
                }
                }
            //condition spawning Bonus
            if (spawn == 10){
                int rngScore = rng(50);
                if(rngScore >= 41){
                    addBonusToEnd(&BonusListStart, &BonusListEnd,bonus[3]);
                }
                else if(rngScore >= 0 && rngScore <= 15){
                    addBonusToEnd(&BonusListStart, &BonusListEnd,bonus[rng(1)]);
                }
                else if(rngScore >= 16 && rngScore <= 40){
                    addBonusToEnd(&BonusListStart, &BonusListEnd,bonus[2]);
                }
                spawn = 0;
            }
            speed = baseSpeed + scoreSpeed + bonusSpeed;
            SDL_RenderClear(ren);



            wallY -= speed;
            wallY1 -= speed;

            if(wallY <= -895 && bg == 0){
                bg = 1;
            }
            if(wallY <= -895){
                wallY = wallY1+900;
            }
            if(wallY1 <= -895){
                wallY1 = wallY+900;
            }
            

            upObstacleList(&obstaclesListStart,&obstaclesListEnd,speed);
            upBonusList(&BonusListStart,&BonusListEnd,speed);
            if(bg == 0)
                drawWalls(ren,ptrWallY,startBg);
            else if(bg == 1){
                drawWalls(ren,ptrWallY,bg1);
            }
                
            drawWalls(ren,ptrWallY1,bg0);
            drawBonusList(ren,BonusListStart,ColorApple,ColorRope,ColorBomb);
            drawObstacleList(ren,obstaclesListStart,drawColor);
            //1 obstacle 2 apple(+10 score) 3 rope (decrease speed) 4 bomb(destroy obstacle)
            //elimination or bonuses
            int collision = checkColorCollision(ren, *ptrPlayerX, *ptrPlayerY,drawColor);
            if( collision == 1) {
                eraseGamingField(ren);  
                removeAllObstacles(&obstaclesListStart,&obstaclesListEnd);
                removeAllBonus(&BonusListStart,&BonusListEnd);
                addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[rng(6)]);

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
                          

                wallY = 0;
                wallY1 = 899;
                SDL_Texture *startPlayerText = loadTexture(ren,"src/assets/img/player1.bmp");
                SDL_Texture *startBg = loadTexture(ren,"src/assets/img/StartBackground.bmp");
                StartingGame(ren,ptrPlayerX,ptrPlayerY,ptrWallY,startPlayerText,startBg);
                SDL_DestroyTexture(startPlayerText);
                if (lastScoreInt < actScoreInt) {
                    fputs(actScore, filScore);
                    writeScores(ren,Green,actScore,lastScore);
                }
                else{
                    writeScores(ren,Red,actScore,lastScore);
                }
                fclose(filScore); 
                playing = 0;
                score = 0;
                lastScoreTime = 0;
                baseSpeed = 1;
                bonusSpeed = 0;
                spawn = 0;
                bg = 0;
            }
            else if (collision == 2) {
                score += 10;
                removeAllBonus(&BonusListStart,&BonusListEnd);
                BonusNode* BonusListStart = NULL;
                BonusNode* BonusListEnd = NULL;
            }
            else if (collision == 3) {
                bonusSpeed = -1;
                removeAllBonus(&BonusListStart,&BonusListEnd);
                BonusNode* BonusListStart = NULL;
                BonusNode* BonusListEnd = NULL;
            }
            else if (collision == 4) {
                removeAllObstacles(&obstaclesListStart,&obstaclesListEnd);
                removeAllBonus(&BonusListStart,&BonusListEnd);
                BonusNode* BonusListStart = NULL;
                BonusNode* BonusListEnd = NULL;
                addObstaclesToEnd(&obstaclesListStart, &obstaclesListEnd,predefinedObstacles[rng(6)]);
            }
            if (playing != 0){          
                drawPlayer(ren,*ptrPlayerX,*ptrPlayerY,playerTexture);}
        }

        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_RETURN:
                            if(playing == 0){
                                playing = startPlaying(ren,ptrPlayerX,ptrPlayerY,playerTexture);
                                
                                continue;}
                        case SDLK_a :
                            if(playing == 1){
                                moveLeft(ren,ptrPlayerX,ptrPlayerY,playerTexture);
                                continue;
                            }
                        case SDLK_LEFT:
                            if(playing == 1){
                                moveLeft(ren,ptrPlayerX,ptrPlayerY,playerTexture);
                                continue;
                            }
                        case SDLK_d :
                            if(playing == 1){
                                moveRight(ren,ptrPlayerX,ptrPlayerY,playerTexture);
                                continue;
                            }
                        case SDLK_RIGHT:
                            if(playing == 1){
                                moveRight(ren,ptrPlayerX,ptrPlayerY,playerTexture);
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
        frame = SDL_GetTicks() + LimitFps;
        SDL_RenderPresent(ren);
    }
     

    
/*-------------------------------------------------------------------------*/
            //Close all
/*-------------------------------------------------------------------------*/
    
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(startBg);
    SDL_DestroyTexture(bg0);
    SDL_DestroyTexture(bg1);
    SDL_DestroyTexture(startPlayerText);
    SDL_FreeFormat(pixelFormat);
    removeAllBonus(&BonusListStart,&BonusListEnd);
    removeAllObstacles(&obstaclesListStart,&obstaclesListEnd);  
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}




