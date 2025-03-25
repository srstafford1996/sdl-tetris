#include "common.hpp"
#include "render.hpp"
#include "game.hpp"

#include <SDL3/SDL.h>
#include <ctime>

#define FRAME_DELAY 33 // MS for FPS cap

int main()
{
    Uint64 frameStart = 0;
    Uint64 lastFrame = 0;

    SDL_Window *window = SDL_CreateWindow("SK Tetris", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    SDL_Event e;
    int isRunning = 1;

    RenderInit(renderer);
    
    SDL_srand(time(NULL));

    Game game;

    while (isRunning)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            isRunning = 0;
        }

        frameStart = SDL_GetTicks();
        if (frameStart - lastFrame < FRAME_DELAY)
            continue;
        
        SDL_RenderClear(renderer);
        game.Update();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}