#include <iostream>
#include <SDL3/SDL.h>
#include "GardenState.h"
#include "WindowPainter.h"
#include "Models.h"

#define grn_interp(ratio) (float)((0.7*ratio)+0.3)
#define red_interp(ratio) (float)((0.3*ratio)+0.1)
#define blu_interp(ratio) (float)((-0.3*ratio)+0.1)


WindowPainter::WindowPainter(int screenResX, int screenResY, int tileSize)
{
    this->screenResX = screenResX;
    this->screenResY = screenResY;

    //Create window and Renderer
    if(!SDL_CreateWindowAndRenderer("PlantLife", screenResX, screenResY, 0,&this->window,&this->wRenderer))
    {
        SDL_Log("Failure creating Window and Renderer! SDL error: %s\n", SDL_GetError());
    }
    this->tileTexture = SDL_CreateTexture(this->wRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tileSize, tileSize);
    if (this->tileTexture == nullptr)
    {
        SDL_Log("Failed To Create Tile Texture: %s\n", SDL_GetError());
    }
    this->splodge= SDL_CreateTexture(this->wRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->splodgeStartSize, this->splodgeStartSize);
    if (this->splodge == nullptr)
    {
        SDL_Log("Failed To Create Splodge Texture: %s\n", SDL_GetError());
    }

}

WindowPainter::~WindowPainter()
{
    SDL_DestroyTexture(this->splodge);
    SDL_DestroyTexture(this->tileTexture);
    SDL_DestroyRenderer(this->wRenderer);
    SDL_DestroyWindow(this->window);
}

bool WindowPainter::PerformRenderCycle(GardenState* garden)
{
    GetLightingCoefficients(garden);
    if (!RenderTileTexture(garden))
    {
        return false;
    }
    if (!SDL_SetRenderTarget(this->wRenderer, nullptr))
    {
        SDL_Log("Failed to set render target back to window: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(this->wRenderer, 150, 180, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(this->wRenderer);
    //SDL_SetRenderScale(this->wRenderer, 3, 3);
    PaintGrid(garden);
    RenderTestLine();
    //Paint Plants

    if (!SDL_RenderPresent(this->wRenderer))
    {
        SDL_Log("Failure rendering backbuffer to window: %s\n", SDL_GetError());
    }
    return true;
}


//What happens if we fail to render the texture?
bool WindowPainter::RenderTileTexture(GardenState* garden)
{
    int xSize = garden->GetTileSize();
    int ySize = garden->GetTileSize();

    //R&B range from 0.1->0.4
    //G Ranges from  0.3->1

    //Remember to add indices here so we can resue points
    if (!SDL_SetRenderTarget(this->wRenderer, this->tileTexture))
    {
        SDL_Log("Failed to set Render Target to Tile Texture: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(this->wRenderer, 100, 100, 100, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(this->wRenderer);

    SDL_Vertex vertices[4];
    SDL_zeroa(vertices);

    //Top----------------------------------------
    vertices[0].position.x = xSize / 2;
    vertices[0].position.y = ySize / 2;
    vertices[0].color = { red_interp(this->topLightCoef),grn_interp(this->topLightCoef),blu_interp(this->topLightCoef) };
    vertices[0].color.a = 1;
    vertices[1].position.x = xSize / 2;
    vertices[1].position.y = 0;
    vertices[1].color = { red_interp(this->topLightCoef),grn_interp(this->topLightCoef),blu_interp(this->topLightCoef) };
    vertices[1].color.a = 1;
    vertices[2].position.x = 0;
    vertices[2].position.y = ySize / 4;
    vertices[2].color = { red_interp(this->topLightCoef),grn_interp(this->topLightCoef),blu_interp(this->topLightCoef) };
    vertices[2].color.a = 1;
    SDL_RenderGeometry(this->wRenderer, nullptr, vertices, 3, NULL, 0);
    vertices[0].position.x = xSize / 2;
    vertices[0].position.y = ySize / 2;
    vertices[0].color = { red_interp(this->topLightCoef),grn_interp(this->topLightCoef),blu_interp(this->topLightCoef) };
    vertices[0].color.a = 1;
    vertices[1].position.x = xSize / 2;
    vertices[1].position.y = 0;
    vertices[1].color = { red_interp(this->topLightCoef),grn_interp(this->topLightCoef),blu_interp(this->topLightCoef) };
    vertices[1].color.a = 1;
    vertices[2].position.x = xSize - 1;
    vertices[2].position.y = ySize / 4;
    vertices[2].color = { red_interp(this->topLightCoef),grn_interp(this->topLightCoef),blu_interp(this->topLightCoef) };
    vertices[2].color.a = 1;
    SDL_RenderGeometry(this->wRenderer, NULL, vertices, 3, NULL, 0);

    //Right--------------------------------------
    vertices[0].position.x = xSize / 2;
    vertices[0].position.y = ySize / 2;
    vertices[0].color = { red_interp(this->rightLightCoef),grn_interp(this->rightLightCoef),blu_interp(this->rightLightCoef) };
    vertices[0].color.a = 1;
    vertices[1].position.x = xSize - 1;
    vertices[1].position.y = ySize / 4;
    vertices[1].color = { red_interp(this->rightLightCoef),grn_interp(this->rightLightCoef),blu_interp(this->rightLightCoef) };
    vertices[1].color.a = 1;
    vertices[2].position.x = xSize - 1;
    vertices[2].position.y = 3 * (ySize / 4);
    vertices[2].color = { red_interp(this->rightLightCoef),grn_interp(this->rightLightCoef),blu_interp(this->rightLightCoef) };
    vertices[2].color.a = 1;
    SDL_RenderGeometry(this->wRenderer, NULL, vertices, 3, NULL, 0);
    vertices[0].position.x = xSize / 2;
    vertices[0].position.y = ySize / 2;
    vertices[0].color = { red_interp(this->rightLightCoef),grn_interp(this->rightLightCoef),blu_interp(this->rightLightCoef) };
    vertices[0].color.a = 1;
    vertices[1].position.x = xSize / 2;
    vertices[1].position.y = ySize - 1;
    vertices[1].color = { red_interp(this->rightLightCoef),grn_interp(this->rightLightCoef),blu_interp(this->rightLightCoef) };
    vertices[1].color.a = 1;
    vertices[2].position.x = xSize - 1;
    vertices[2].position.y = 3 * (ySize / 4);
    vertices[2].color = { red_interp(this->rightLightCoef),grn_interp(this->rightLightCoef),blu_interp(this->rightLightCoef) };
    vertices[2].color.a = 1;
    SDL_RenderGeometry(this->wRenderer, NULL, vertices, 3, NULL, 0);

    //Left---------------------------------------
    vertices[0].position.x = xSize / 2;
    vertices[0].position.y = ySize / 2;
    vertices[0].color = { red_interp(this->leftLightCoef),grn_interp(this->leftLightCoef),blu_interp(this->leftLightCoef) };
    vertices[0].color.a = 1;
    vertices[1].position.x = 0;
    vertices[1].position.y = ySize / 4;
    vertices[1].color = { red_interp(this->leftLightCoef),grn_interp(this->leftLightCoef),blu_interp(this->leftLightCoef) };
    vertices[1].color.a = 1;
    vertices[2].position.x = 0;
    vertices[2].position.y = 3 * (ySize / 4);
    vertices[2].color = { red_interp(this->leftLightCoef),grn_interp(this->leftLightCoef),blu_interp(this->leftLightCoef) };
    vertices[2].color.a = 1;
    SDL_RenderGeometry(this->wRenderer, NULL, vertices, 3, NULL, 0);
    vertices[0].position.x = xSize / 2;
    vertices[0].position.y = ySize / 2;
    vertices[0].color = { red_interp(this->leftLightCoef),grn_interp(this->leftLightCoef),blu_interp(this->leftLightCoef) };
    vertices[0].color.a = 1;
    vertices[1].position.x = xSize / 2;
    vertices[1].position.y = ySize - 1;
    vertices[1].color = { red_interp(this->leftLightCoef),grn_interp(this->leftLightCoef),blu_interp(this->leftLightCoef) };
    vertices[1].color.a = 1;
    vertices[2].position.x = 0;
    vertices[2].position.y = 3 * (ySize / 4);
    vertices[2].color = { red_interp(this->leftLightCoef),grn_interp(this->leftLightCoef),blu_interp(this->leftLightCoef) };
    vertices[2].color.a = 1;
    SDL_RenderGeometry(this->wRenderer, NULL, vertices, 3, NULL, 0);

    SDL_SetRenderDrawColor(this->wRenderer, 110, 110, 0, SDL_ALPHA_OPAQUE);
    SDL_FPoint linePoints[7];

    //This will be super useful for doing the bezier curves
    linePoints[0] = { (float)(xSize / 2),0 };
    linePoints[1] = { (float)xSize - 1,(float)(ySize / 4) };
    linePoints[2] = { (float)xSize - 1,(float)(3 * (ySize / 4)) - 1 };
    linePoints[3] = { (float)(xSize / 2),float(ySize - 1) };
    linePoints[4] = { 0,(float)(3 * (ySize / 4)) - 1 };
    linePoints[5] = { 0,(float)(ySize / 4) };
    linePoints[6] = { (float)(xSize / 2),0 };
    SDL_RenderLines(this->wRenderer, linePoints, 7);

    linePoints[0] = { (float)(xSize - 2),(float)(ySize / 4) };
    linePoints[1] = { (float)(xSize - 2),(float)(3 * (ySize / 4)) - 1 };
    SDL_RenderLines(this->wRenderer, linePoints, 2);

    linePoints[0] = { 1,(float)(ySize / 4) };
    linePoints[1] = { 1,(float)(3 * (ySize / 4)) - 1 };
    SDL_RenderLines(this->wRenderer, linePoints, 2);

    return true;
}



//WindowPainter is responsible for drawing things where it wants them. Garden will create everything 0 based, so window pianter has to position it in the middle of the screen
//Means Garden is agnostic of screen size
bool WindowPainter::PaintGrid(GardenState* garden)
{
    SDL_FRect rect{ 0,0,garden->GetTileSize(),garden->GetTileSize() };
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            GridSquare crSquare = garden->GetGridSquare(x, y);
            rect.x = crSquare.screenCoord.x +( screenResX/2)-32;
            rect.y = crSquare.screenCoord.y+(screenResX/6);
            if (!SDL_RenderTexture(wRenderer, this->tileTexture, nullptr, &rect))
            {
                SDL_Log("Failed to paint grid to renderer: %s\n", SDL_GetError());
                return false;
            }
            //SDL_SetRenderDrawColor(this->wRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            //Renders centre - should have a grid translation method
            //SDL_FRect spldge = { crSquare.centerPos.x + (screenResX / 2) - 32 ,crSquare.centerPos.y + (screenResX / 6) ,6,6 };
            //SDL_RenderFillRect(this->wRenderer, &spldge);
        }
    }
    return true;
}


bool WindowPainter::RenderSplodge( )
{
    
    if (!SDL_SetRenderTarget(this->wRenderer, this->splodge))
    {
        SDL_Log("Failed to set Render Target to Splodge: %s\n", SDL_GetError());
        return false;
    }
    //SDL_SetRenderDrawColor(this->wRenderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(this->wRenderer);
    
    for (int x = 0; x < this->splodgeStartSize;x++)
    {
        for (int y = 0;y< this->splodgeStartSize;y++)
        {
            if (this->circleRef16[x][y] == 2)
            {
                SDL_SetRenderDrawColor(this->wRenderer, 150, 150, 70, SDL_ALPHA_OPAQUE);
                SDL_RenderPoint(this->wRenderer, x, y);
            }
            else if (this->circleRef16[x][y] == 0)
            {
                SDL_SetRenderDrawColor(this->wRenderer, 150, 150, 70, SDL_ALPHA_TRANSPARENT);
                SDL_RenderPoint(this->wRenderer, x, y);
            }
        }
    }
    
    if (!SDL_SetRenderTarget(this->wRenderer, nullptr))
    {
        SDL_Log("Failed to reset render target to window from splodge texture: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void WindowPainter::RenderBezier(SDL_Texture* lineTex, BezierSeg* seg, int numSegs, float strtRatio, float endRatio )
{
    SDL_FRect toDraw = { 0,0,this->splodgeStartSize,this->splodgeStartSize };
    float distStartToEnd= sqrt(pow((seg[numSegs-1].pEnd.y - seg[0].pStart.y), 2) + pow((seg[numSegs-1].pEnd.x - seg[0].pStart.x), 2));
    //Draw each seg
    for (int i = 0; i < numSegs; i++)
    {
        //pointSep used to work out how many splodges to draw
        int pointSep = sqrt(pow((seg[i].pEnd.y - seg[i].pStart.y),2) + pow((seg[i].pEnd.x - seg[i].pStart.x),2));
        for (int p = 0; p < pointSep; p++)
        {
            float t = (float)p / (float)pointSep;
            //Gets point on bezier curve
            toDraw.x = ((1 - t) * (1 - t) * seg[i].pStart.x) + (2 * t * (1 - t) * seg[i].pCtrl.x) + ((t * t) * seg[i].pEnd.x);
            toDraw.y = ((1 - t) * (1 - t) * seg[i].pStart.y) + (2 * t * (1 - t) * seg[i].pCtrl.y) + ((t * t) * seg[i].pEnd.y);

            //This section is concerned with thickness - there is a problem here for very deformed curves - think there will be anomolies
            //Possible I cold set up a function that approximates this to start, rather than do it at every call...
            float distFromBase= sqrt(pow((toDraw.x - seg[0].pStart.x), 2) + pow((toDraw.y - seg[0].pStart.y), 2));
            float ratio = 1-(distFromBase / distStartToEnd);
            ratio = ((1 - ratio) * endRatio) + (ratio * strtRatio);
            toDraw.w = ratio * this->splodgeStartSize;
            toDraw.h = ratio * this->splodgeStartSize;
            SDL_RenderTexture(this->wRenderer, this->splodge, nullptr, &toDraw);
        }
    }
}

//Have my original bezier structure, then adjust a copy based on draw width to get the line bezier for either side
//For shading, there was an interesting effect caused by having part of the texture transparent, and rendering over itself
//We need the RenderBezier function to be fully re-usable - I should be able to pass in a start and end ratio
void WindowPainter::RenderTestLine()
{
    SDL_Texture* lineDot = SDL_CreateTexture(this->wRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 3, 3);

    RenderSplodge();
    BezierSeg* segs = new BezierSeg[2];
    segs[0] = { {100,400},{120,350},{100,300} };
    segs[1] = { {100,300},{80,250},{100,200} };

    BezierSeg* segB = new BezierSeg;
    *segB = { {100,300},{75,300},{40,250} };


    RenderBezier(this->splodge, segs, 2,1,0.2);
    RenderBezier(this->splodge, segB, 1, 0.3, 0.1);

    delete[] segs;

    SDL_DestroyTexture(lineDot);
}



void WindowPainter::GetLightingCoefficients(GardenState* garden)
{
    float pi = 3.14159;
   
    //Ratio from 0->1 of the sun positon
    float lrRatio = (garden->sun.screenPos.x / screenResX);
    //Flip so it goes 1->0 across screen
    lrRatio = 1 - lrRatio;
    //Equation preforms non-linear interpolation to the x axis of cos function
    lrRatio = acos((2 * lrRatio) - 1);
    //interpolate to 0->1
    lrRatio = lrRatio / pi;

    float udRatio = garden->sun.screenPos.y / screenResY;
    udRatio = acos((2 * udRatio) - 1);
    udRatio = udRatio / pi;

    this->topLightCoef = udRatio;
    this->leftLightCoef = (1 - lrRatio) * (1 - this->topLightCoef);
    this->rightLightCoef = lrRatio * (1 - this->topLightCoef);
}

//Base colour set - then modify
//Not sure where to put this, but it must use the rendere so that has to be passed in by the WindowPainter
//The mouse thing is getting removed eventually

//Looks good at extreme angles - less so in the middle, because the brightnes gets too low - looks weird
//-Need some non linear brightness scaling to fix
//Use non linear position interpolation
//Think the side surfaces being lit at extreme angles is ok as long as the effect is that its facing directly at that and not  "under" the structure