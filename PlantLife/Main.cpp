#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_init.h>
#include "WorldManager.h"
#include "WindowPainter.h"
#include "GardenState.h"

//TO DO:
/*
* Go through and trace error handling

* 
* Game Plan:

* ->Finalise PLant structure and build example/test plant
* ->Use this to finalise shading/drawing method for braches
* ->Try to simplify calculations wherever possible - remember - every little helps
* ->physics I guess
* ->Lystems
* * ->Fix lighting on terrain so its constant - will this look wierd with how plants are done? If anythjing will look less weird I hop..
*/


//L Systems - Lindenmayer systems


/* Constants */
//Screen dimension constants
constexpr int kScreenWidth{ 1800 };
constexpr int kScreenHeight{ 900 };


//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();



int main(int argc, char* argv[]) {
    //Final exit code
    int exitCode{ 0 };
    
    //Initialize
    if (!init())
    {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
    }
    else
    {
        //The quit flag
        bool quit{ false };

        //The event data
        SDL_Event e;
        SDL_zero(e);
        
        WorldManager world(kScreenWidth,kScreenHeight, 112);

        float mouseX = -1.f, mouseY = -1.f;

        //The main loop
        while (quit == false)
        {
            //Get event data
            while (SDL_PollEvent(&e))
            {
                //If event is quit type
                if (e.type == SDL_EVENT_QUIT)
                {
                    //End the main loop
                    quit = true;
                }

                /*
                //Tracking mouse
                if (e.type == SDL_EVENT_MOUSE_MOTION || e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    //Get mouse position
                    SDL_GetMouseState(&mouseX, &mouseY);
                    world.garden->sun.screenPos = { mouseX, mouseY };
                }
                */
            }
            if (!world.RenderScene())
            {
                quit = true;
            }
        }
        
    }

    //Clean up
    close();

    return exitCode;
}

/* Function Implementations */
bool init()
{
    //Initialization flag
    bool success{ true };

    //Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        success = false;
    }

    return success;
}



void close()
{

    //Quit SDL subsystems
    SDL_Quit();
}


