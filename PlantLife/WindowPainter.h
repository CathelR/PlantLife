#include <iostream>
#include <SDL3/SDL.h>
#include "GardenState.h"
#include "GardenStructs.h"
#include "RenderStructs.h"
#pragma once

class WindowPainter
{
public:
	WindowPainter(int,int, int);
	~WindowPainter();

	SDL_Window* window;
	SDL_Renderer* wRenderer;
	SDL_Texture* tileTexture;
	SDL_Texture* fillSplodge; //A splodge is like a splodge of point, used to draw plants
	SDL_Texture* lineSplodge; //A splodge is like a splodge of point, used to draw plants

	
	
	bool PerformRenderCycle(GardenState*);

private:
	int screenResX;
	int screenResY;
	float leftLightCoef=0;
	float rightLightCoef=0;
	float topLightCoef = 0;
	int lineSplodgeStartSize = 16;
	int fillSplodgeStartSize = 9;
	SplodgeRefs splodgeRefs;

	
	bool PaintGrid(GardenState*);
	bool RenderTileTexture(GardenState*);
	bool RenderLineSplodge();
	bool RenderFillSplodge();
	void GetLightingCoefficients(GardenState*);
	//SDL_Color GetLitColours(SDL_Color, float, float, float);
	void PaintPlants(GardenState*);
	void RenderBezierOutline(SDL_Texture*, BezierSeg*,int, float, float);
	void RenderBezierFill(SDL_Texture*, BezierSeg*, int, float, float);

};


/*


*/