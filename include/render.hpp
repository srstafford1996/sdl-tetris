#pragma once
#include <SDL3/SDL.h>

#include "common.hpp"

class GameBoard;
class Player;


void RenderInit(SDL_Renderer *);

void LoadBlockTextures();
void DestroyBlockTextures();

void DrawBlock(int x, int y, BlockState bs, SDL_Texture *blockTexture);
void DrawBoard(GameBoard *board);

void DrawSwapPiece(int pieceIndex);
void DrawSidebar(Player *player);

void DrawPlayerPiece(Player *player);
void ClearPlayerPiece(Player *player);

