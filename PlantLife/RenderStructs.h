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
	SDL_FPoint pStart{ 0,0 };
	SDL_FPoint pCtrl{ 0,0 };
	SDL_FPoint pEnd{ 0,0 };
} BezierSeg;



typedef struct SplodgeRefs
{
	const int lineSplodgeRef[16][16] =
	{

		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0 },
		{ 0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0 },
		{ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
		{ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
		{ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
		{ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
		{ 0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0 },
		{ 0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
	};

	const int fillSplodgeRef[16][16] =
	{

		{ 0,0,0,2,2,2,0,0,0 },
		{ 0,2,2,2,2,2,2,2,0 },
		{ 0,2,2,2,2,2,2,2,0 },
		{ 1,1,2,2,2,2,2,1,1 },
		{ 1,1,2,2,2,2,2,1,1 },
		{ 1,1,2,2,2,2,2,1,1 },
		{ 0,1,2,2,2,2,2,1,0 },
		{ 0,2,2,3,3,3,2,2,0 },
		{ 0,0,0,3,3,3,0,0,0 }

	};
}SplodgeRefs;


//Using parent and startT, it will be possible tog et the actual starting coord of a branch, without it having to be on a knuckle
//if parent is null - its the root and doesnt move
typedef struct Branch
{
	Branch* parent;
	Branch* children;
	float startT;
	BezierSeg* bSegs;
	int numSegs;
}Branch;

//So a plant will be a collection of branches - need tree structure for engine - parents must be adjusted first
//We would then have a second structure called build order, this would be a flat array pointing to different branches in diff orders
//The build order only matter when two branches are connected
//Tail recursion to update using engine
typedef struct Plant
{
	SDL_FPoint rootPos;
	Branch* rootBranches;
	int numRoots;
	//Used for rendering, we interleave branch render order to get branches in front and behind
	Branch* branchOrder;
	int totalNumBranches;
	SDL_Color branchCol;
	SDL_Color leafCol;
}Plant;

/*
So a plant has a build order for its components
->crucial: we know that if its rendered behdind the thing its attatched to, it has the opposite side darkness property than if it was rendered in front
->so we have a way of saying whether it should be light or dark when lit from the side


*/