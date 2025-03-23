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
    SDL_srand(time(NULL));

    player = new Player();

    DrawBoard(player->board);
    DrawSidebar(player->nextPieces, player->nextPieceIndex);
    DrawPlayerPiece(player->x, player->y, &player->currentPiece);

}

Game::~Game()
{
    delete player;
}



// So this game needs to draw different types of blocks
void Game::Update()
{
    ClearPlayerPiece(player->x, player->y, &player->currentPiece); 
    
    Uint64 tickStart = SDL_GetTicks();
    
    SDL_PumpEvents();
    const bool *keys = SDL_GetKeyboardState(NULL);

    InputState s = {keys[SDL_SCANCODE_LEFT], keys[SDL_SCANCODE_RIGHT], keys[SDL_SCANCODE_UP], keys[SDL_SCANCODE_DOWN]};
    player->Update(s);

    if (player->overflow)
    {
        printf("Resetting");
        SDL_Delay(200);
        SDL_Delay(200);
        player->Reset();
    }

    DrawPlayerPiece(player->x, player->y, &player->currentPiece);
}