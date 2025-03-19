#include <SDL3/SDL.h>

#include "common.h"
#include "game.h"

int main()
{

    SDL_Window *window = SDL_CreateWindow("SK Tetris", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    SDL_Event e;
    int isRunning = 1;

    InitGame(surface);
    while (isRunning)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                isRunning = 0;
        }

        UpdateGame();
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    return 0;
}