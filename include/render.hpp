#pragma once
#include <SDL3/SDL.h>

#include "common.hpp"
#include "board.hpp"

void RenderInit(SDL_Surface *);

void DrawBlock(int x, int y, BlockState bs);
void DrawBoard(GameBoard &board);
void DrawSidebar(int *nextPieceArr, int nextPieceArrIndex);


void DrawPlayerPiece(int x, int y, TetrisPiece *piece);
void ClearPlayerPiece(int x, int y, TetrisPiece *piece);