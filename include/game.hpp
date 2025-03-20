#pragma once
#include <SDL3/SDL.h>

#include "player.hpp"

class Game
{
    Player *player;

public:
    Game();
    ~Game();
    
    void Update();

};