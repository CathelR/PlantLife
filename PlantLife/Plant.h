#include <SDL3/SDL.h>
#pragma once


typedef struct BezierSeg
{
	SDL_FPoint pStart{ 0,0 };
	SDL_FPoint pCtrl{ 0,0 };
	SDL_FPoint pEnd{ 0,0 };
} BezierSeg;


//Using parent and startT, it will be possible tog et the actual starting coord of a branch, without it having to be on a knuckle
//if parent is null - its the root and doesnt move
typedef struct Branch
{
	Branch* parent;
	Branch* children;
	float startT;
	int startSegIndx;
	BezierSeg* bSegs;
	int numSegs;
	float stiffness;
	float defaultPos;
}Branch;




/*
So a plant has a build order for its components
->crucial: we know that if its rendered behdind the thing its attatched to, it has the opposite side darkness property than if it was rendered in front
->so we have a way of saying whether it should be light or dark when lit from the side
*/



class Plant
{
public:
	SDL_FPoint rootPos;
	Branch* main;
	int numRoots;
	//A flat array whic orders the individual branches - we don't draw them in tree order
	Branch* branchOrder;
	int totalNumBranches;
	SDL_Color branchCol;
	SDL_Color leafCol;

	Plant();
	~Plant();
};
