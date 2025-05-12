#include <SDL3/SDL.h>
#include "GardenStructs.h"

#pragma once


class GardenState
{

    //We will translate the whole plant by the base - But we only need to do that once - when we generate the plant, as it is rooted.
    //It may make sense to house the converted grid here too since it is "static"

public:
    
    Sun sun;
    const int gridSize = 10;
    Plant* plants;
    

    GardenState(int,int,int);
    ~GardenState();
    GridSquare* GetGridSquare(int, int);
    int GetTileSize();
    SDL_FPoint ScreenSpaceConvert(SDL_FPoint);
    void GenerateGrid();
    bool GeneratePlants();
    

    

private:
    //Wind wind;
    //Rain rain;
    //Sun sun;
    //const float gravity{ 9.81 };
    GridSquare grid[10][10];
    int tileSize;
    int screenSizeX;
    int screenSizeY;
    //Array of plants, which will

};

/*
class Rain
{
    float quantity;
};
class Sun
{
    float strength;
    SDL_FPoint position;
    float angle;
    //May include the angle calculation in here - since its the only place that will use it??
    //Then going forward you can just refer to the suns angle -
};
class Wind
{
    float direction;
    float strength;
};


*/



