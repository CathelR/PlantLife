#include <SDL3/SDL.h>
#include "GardenStructs.h"

#pragma once


class GardenState
{

    //We will translate the whole plant by the base - But we only need to do that once - when we generate the plant, as it is rooted.
    //It may make sense to house the converted grid here too since it is "static"

public:
    
    Sun sun;
    const int gridSize = 12;

    GardenState(int);
    ~GardenState();

    void GenerateGrid();

    GridSquare GetGridSquare(int, int);
    int GetTileSize();
    SDL_FPoint ScreenSpaceConvert(SDL_FPoint);

    //bool GeneratePlants();

private:
    //Wind wind;
    //Rain rain;
    //Sun sun;
    //const float gravity{ 9.81 };
    GridSquare grid[12][12];
    int tileSize;
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


class Plant
{

};
*/



