#pragma once
#include <SDL3/SDL.h>

#include "common.h"

void RenderInit(SDL_Surface *);

void DrawBlock(int x, int y, BlockState bs);
void DrawBoard();

void DrawPlayerPiece(int x, int y, TetrisPiece *piece);
void ClearPlayerPiece(int x, int y, TetrisPiece *piece);