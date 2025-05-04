#include <SDL3/SDL.h>
#pragma once

typedef struct GridSquare
{
    SDL_FPoint baseCoord;
    SDL_FPoint screenCoord;
    SDL_FPoint centerPos;
    float height;
    //Plant* plant;
} GridSquare;

typedef struct Sun
{
    SDL_FPoint screenPos;
    float brightness;
} Sun;

typedef struct BezierSeg
{
    SDL_Point pStart{ 0,0 };
    SDL_Point pCtrl{ 0,0 };
    SDL_Point pEnd{ 0,0 };
} BezierSeg;
