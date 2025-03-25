#pragma once
#include <SDL3/SDL.h>

#include "player.hpp"

class Game
{
public:
    Player *player;

    Game();
    ~Game();
    
    void Update();

};