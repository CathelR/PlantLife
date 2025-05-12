#include <iostream>
#include <SDL3/SDL.h>
#include "GardenState.h"
#include "WindowPainter.h"
#include "RenderStructs.h"

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
    this->lineSplodge = SDL_CreateTexture(this->wRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->lineSplodgeStartSize, this->lineSplodgeStartSize);
    if (this->lineSplodge == nullptr)
    {
        SDL_Log("Failed To Create Splodge Texture: %s\n", SDL_GetError());
    }
    this->fillSplodge = SDL_CreateTexture(this->wRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->fillSplodgeStartSize, this->fillSplodgeStartSize);
    if (this->fillSplodge == nullptr)
    {
        SDL_Log("Failed To Create Splodge Texture: %s\n", SDL_GetError());
    }
}

WindowPainter::~WindowPainter()
{
    SDL_DestroyTexture(this->lineSplodge);
    SDL_DestroyTexture(this->fillSplodge);
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
    PaintPlants(garden);
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
    for (int y = 0; y < garden->gridSize; y++)
    {
        for (int x = 0; x < garden->gridSize; x++)
        {
            GridSquare* crSquare = garden->GetGridSquare(x, y);
            rect.x = crSquare->screenCoord.x;
            rect.y = crSquare->screenCoord.y;
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

void WindowPainter::PaintPlants(GardenState* garden)
{
    Plant* plant = garden->plants;
    RenderLineSplodge();
    RenderFillSplodge();
    //If plants is not null
    RenderBezierFill(this->fillSplodge, plant->main->bSegs, 2, 1.1, 0.2);
    RenderBezierOutline(this->lineSplodge, plant->main->bSegs, 2, 0.5, 0.1);
}

bool WindowPainter::RenderLineSplodge()
{
    if (!SDL_SetRenderTarget(this->wRenderer, this->lineSplodge))
    {
        SDL_Log("Failed to set Render Target to LineSplodge: %s\n", SDL_GetError());
        return false;
    }
    SDL_RenderClear(this->wRenderer);
    for (int y = 0; y < this->lineSplodgeStartSize; y++)
    {
        for (int x = 0; x < this->lineSplodgeStartSize; x++)
        {
            if (this->splodgeRefs.lineSplodgeRef[x][y] == 1)
            {
                SDL_SetRenderDrawColor(this->wRenderer, 30, 30, 60, SDL_ALPHA_OPAQUE);
                SDL_RenderPoint(this->wRenderer, x, y);
            }
            else if (this->splodgeRefs.lineSplodgeRef[x][y] == 0)
            {
                SDL_SetRenderDrawColor(this->wRenderer, 140, 100, 100, SDL_ALPHA_TRANSPARENT);
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




bool WindowPainter::RenderFillSplodge(/*Passin coloura*/)
{
    if (!SDL_SetRenderTarget(this->wRenderer, this->fillSplodge))
    {
        SDL_Log("Failed to set Render Target to FillSplodge: %s\n", SDL_GetError());
        return false;
    }
    SDL_RenderClear(this->wRenderer);
    for (int y = 0; y < this->fillSplodgeStartSize; y++)
    {
        for (int x = 0; x < this->fillSplodgeStartSize; x++)
        {
            
            if (this->splodgeRefs.fillSplodgeRef[y][x] == 1)
            {
                SDL_SetRenderDrawColor(this->wRenderer, 90, 85, 70, SDL_ALPHA_OPAQUE);
                SDL_RenderPoint(this->wRenderer, x, y);
            }
            else if (this->splodgeRefs.fillSplodgeRef[y][x] == 2)
            {
                SDL_SetRenderDrawColor(this->wRenderer, 140, 110, 100, SDL_ALPHA_OPAQUE);
                SDL_RenderPoint(this->wRenderer, x, y);
            }
            else if (this->splodgeRefs.fillSplodgeRef[y][x] == 3)
            {
                SDL_SetRenderDrawColor(this->wRenderer, 180, 150, 95, SDL_ALPHA_OPAQUE);
                SDL_RenderPoint(this->wRenderer, x, y);
            }
            else if (this->splodgeRefs.fillSplodgeRef[y][x] == 0)
            {
                SDL_SetRenderDrawColor(this->wRenderer, 140, 110, 100, SDL_ALPHA_TRANSPARENT);
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

//Going forward this should utilittse a strategy pattern - this is just fo tesitng
//There is a relationship between the angle of the bezier and the exact rotation of the splodge that makes it look as if its lit from top or botton - use..
void WindowPainter::RenderBezierFill(SDL_Texture* splodge, BezierSeg* seg, int numSegs, float strtRatio, float endRatio )
{
    int splodgeBaseSize = splodge->w;
    SDL_FRect toDraw = { 0,0,splodgeBaseSize,splodgeBaseSize };
    float distStartToEnd= sqrt(pow((seg[numSegs-1].pEnd.y - seg[0].pStart.y), 2) + pow((seg[numSegs-1].pEnd.x - seg[0].pStart.x), 2));
    //Quick way to get the gradient
    SDL_FPoint prevPoint = seg[0].pStart;
    //Draw each seg
    for (int i = 0; i < numSegs; i++)
    {
        //pointSep used to work out how many splodges to draw - needs to be optimised - easier calc, and more reliable 
        //Could be made more accurate by going strt->control->end, if I had a rapid calc
        int pointSep = sqrt(pow((seg[i].pEnd.y - seg[i].pStart.y),2) + pow((seg[i].pEnd.x - seg[i].pStart.x),2));
        for (int p = 0; p < pointSep; p++)
        {
            float t = (float)p / (float)pointSep;
            //Gets point on bezier curve
            toDraw.x = ((1 - t) * (1 - t) * seg[i].pStart.x) + (2 * t * (1 - t) * seg[i].pCtrl.x) + ((t * t) * seg[i].pEnd.x);
            toDraw.y = ((1 - t) * (1 - t) * seg[i].pStart.y) + (2 * t * (1 - t) * seg[i].pCtrl.y) + ((t * t) * seg[i].pEnd.y);

            //Gives approximate gradient of line at this point with minimal calculation
            //About as efficient as im going to get it
            float gradient = (float)(toDraw.y - (float)prevPoint.y) / (float)(toDraw.x - (float)prevPoint.x);
            double angle = (180/3.141)*atan(gradient);
            prevPoint.x = toDraw.x;
            prevPoint.y = toDraw.y;

            //This section is concerned with thickness - there is a problem here for very deformed curves - think there will be anomolies
            //Possible I cold set up a function that approximates this to start, rather than do it at every call...
            float distFromBase= sqrt(pow((toDraw.x - seg[0].pStart.x), 2) + pow((toDraw.y - seg[0].pStart.y), 2));
            float ratio = 1-(distFromBase / distStartToEnd);
            ratio = ((1 - ratio) * endRatio) + (ratio * strtRatio);
            toDraw.w = ratio * splodgeBaseSize;
            toDraw.h = ratio * splodgeBaseSize;
            
            //SDL_RenderTextureRotated(this->wRenderer, splodge, nullptr, &toDraw, angle, nullptr,SDL_FLIP_NONE);
            //SDL_RenderTexture(this->wRenderer, splodge, nullptr, &toDraw);
            if (numSegs == 1)
            {
                SDL_RenderTextureRotated(this->wRenderer, splodge, nullptr, &toDraw,105 , nullptr, SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderTextureRotated(this->wRenderer, splodge, nullptr, &toDraw, 5, nullptr, SDL_FLIP_NONE);
            }
            
        }
    }
}


void WindowPainter::RenderBezierOutline(SDL_Texture* splodge, BezierSeg* seg, int numSegs, float strtRatio, float endRatio)
{
    int splodgeBaseSize = splodge->w;
    SDL_FRect toDraw = { 0,0,splodgeBaseSize,splodgeBaseSize };
    float distStartToEnd = sqrt(pow((seg[numSegs - 1].pEnd.y - seg[0].pStart.y), 2) + pow((seg[numSegs - 1].pEnd.x - seg[0].pStart.x), 2));
    //Quick way to get the gradient
    SDL_FPoint prevPoint = seg[0].pStart;
    //Draw each seg
    for (int i = 0; i < numSegs; i++)
    {
        //pointSep used to work out how many splodges to draw
        int pointSep = sqrt(pow((seg[i].pEnd.y - seg[i].pStart.y), 2) + pow((seg[i].pEnd.x - seg[i].pStart.x), 2));
        for (int p = 0; p < pointSep; p++)
        {
            float t = (float)p / (float)pointSep;
            //Gets point on bezier curve
            toDraw.x = ((1 - t) * (1 - t) * seg[i].pStart.x) + (2 * t * (1 - t) * seg[i].pCtrl.x) + ((t * t) * seg[i].pEnd.x);
            toDraw.y = ((1 - t) * (1 - t) * seg[i].pStart.y) + (2 * t * (1 - t) * seg[i].pCtrl.y) + ((t * t) * seg[i].pEnd.y);

            //Gives approximate gradient of line at this point with minimal calculation
            float gradient = (float)(toDraw.y - (float)prevPoint.y) / (float)(toDraw.x - (float)prevPoint.x);
            double angle = (180 / 3.141) * atan(gradient);
            prevPoint.x = toDraw.x;
            prevPoint.y = toDraw.y;

            //This section is concerned with thickness - there is a problem here for very deformed curves - think there will be anomolies
            //Possible I cold set up a function that approximates this to start, rather than do it at every call...
            float distFromBase = sqrt(pow((toDraw.x - seg[0].pStart.x), 2) + pow((toDraw.y - seg[0].pStart.y), 2));
            float ratio = 1 - (distFromBase / distStartToEnd);
            ratio = ((1 - ratio) * endRatio) + (ratio * strtRatio);
            toDraw.w = ratio * splodgeBaseSize;
            toDraw.h = ratio * splodgeBaseSize;

            SDL_RenderTextureRotated(this->wRenderer, splodge, nullptr, &toDraw, angle, nullptr,SDL_FLIP_NONE);
           
        }
    }
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