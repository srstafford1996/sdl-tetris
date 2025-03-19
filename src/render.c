#include <SDL3/SDL.h>

#include "common.h"
#include "render.h"
#include "board.h"

SDL_Surface *surface;

const int BLOCK_WIDTH = GRID_VIEW_WIDTH / GRID_WIDTH;
const int BLOCK_HEIGHT = GRID_VIEW_HEIGHT / GRID_HEIGHT;

void RenderInit(SDL_Surface *s)
{
    surface = s;
}

// Draw a block to the grid
void DrawBlock(int x, int y, BlockState bs)
{
    
    SDL_Rect fullSquare = {
        x * BLOCK_WIDTH + GRID_X_OFFSET, 
        GRID_VIEW_HEIGHT - ((y + 1) * BLOCK_HEIGHT), 
        BLOCK_WIDTH,
        BLOCK_HEIGHT
    };

    if (bs == BS_EMPTY)
    {
        SDL_Rect innerSquare = {
            fullSquare.x,
            fullSquare.y,
            BLOCK_WIDTH - GRID_LINE_THICKNESS,
            BLOCK_HEIGHT - GRID_LINE_THICKNESS
        };
        
        if (x == 0) {
            innerSquare.x += GRID_LINE_THICKNESS;
            innerSquare.w -= GRID_LINE_THICKNESS;
        }

        SDL_FillSurfaceRect(surface, &fullSquare, GRID_COLOR);
        SDL_FillSurfaceRect(surface, &innerSquare, BACKGROUND_COLOR);

        return;
    }

    SDL_FillSurfaceRect(surface, &fullSquare, bs);
}

void DrawBoard()
{
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            DrawBlock(x, y, board_GetRowAt(y)[x]);
        }
    }
}

void DrawPlayerPiece(int x, int y, TetrisPiece *piece)
{
    // X and Y are origin point of the "rectangle" of the piece
    for (int i = 0; i < 4; i++)
    {
        DrawBlock(x + piece->blocks[i][0], y - piece->blocks[i][1], piece->color);
    }
}

void ClearPlayerPiece(int x, int y, TetrisPiece *piece)
{
    for (int i = 0; i < 4; i++)
    {
        DrawBlock(x + piece->blocks[i][0], y - piece->blocks[i][1], BS_EMPTY);
    }

}