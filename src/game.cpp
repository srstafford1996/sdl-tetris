#include "game.hpp"

#include <ctime>
#include <cstdio>

#include <SDL3/SDL.h>

#include "player.hpp"
#include "render.hpp"
#include "common.hpp"
#include "board.hpp"

Game::Game()
{
    LoadBlockTextures();
    
    player = new Player();
}

Game::~Game()
{
    DestroyBlockTextures();
    delete player;
}



// So this game needs to draw different types of blocks
void Game::Update()
{
    
    Uint64 tickStart = SDL_GetTicks();
    
    SDL_PumpEvents();
    const bool *keys = SDL_GetKeyboardState(NULL);

    InputState s = {keys[SDL_SCANCODE_LEFT], keys[SDL_SCANCODE_RIGHT], keys[SDL_SCANCODE_UP], keys[SDL_SCANCODE_DOWN], keys[SDL_SCANCODE_SPACE], keys[SDL_SCANCODE_LSHIFT]};
    player->Update(s);

    if (player->overflow)
    {
        SDL_Delay(200);
        SDL_Delay(200);
        player->Reset();
    }

    DrawBoard(&player->board);
    DrawSidebar(player);
    DrawPlayerPiece(player);
}