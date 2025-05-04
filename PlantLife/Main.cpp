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
* Perlin noise for terrain generation
* IMprove lighting interpolation so it doesnt go dark in the middle - total birghtness should remain even regardless of where light is?
* Get lining working for bezier curves, using either rotating texture method, or by calculating edge bezier curves. rotating texture may be easier?
* Get shading working for plants. Drawing a transparent texture over itself provides a potential way to do this.
*/


//L Systems - Lindenmayer systems


/* Constants */
//Screen dimension constants
constexpr int kScreenWidth{ 1600 };
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
        
        WorldManager world(kScreenWidth,kScreenHeight, 64);

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

                //Tracking mouse
                if (e.type == SDL_EVENT_MOUSE_MOTION || e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    //Get mouse position
                    SDL_GetMouseState(&mouseX, &mouseY);
                    world.garden->sun.screenPos = { mouseX, mouseY };
                }
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


