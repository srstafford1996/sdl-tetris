#include "render.hpp"

#include "common.hpp"
#include "board.hpp"
#include "player.hpp"

#include <SDL3/SDL.h>
#include <cstdio>


SDL_Renderer *renderer;

const int BLOCK_WIDTH = GRID_VIEW_WIDTH / GRID_WIDTH;
const int BLOCK_HEIGHT = GRID_VIEW_HEIGHT / GRID_HEIGHT;

const char* textureFiles[7] = {
    "textures/l-blue.bmp",
    "textures/d-blue.bmp",
    "textures/orange.bmp",
    "textures/red.bmp",
    "textures/green.bmp",
    "textures/yellow.bmp",
    "textures/purple.bmp"
};

SDL_Texture *blockTextures[7];
SDL_Texture *shadowTextures[7];

void RenderInit(SDL_Renderer *r)
{
    renderer = r;
}

void LoadBlockTextures()
{
    for (int i = 0; i < 7; i++)
    {
        SDL_Surface *s = SDL_LoadBMP(textureFiles[i]);

        if (s == NULL)
        {
            printf("Error loading texture(%s): %s\n", textureFiles[i], SDL_GetError());
        }

        blockTextures[i] = SDL_CreateTextureFromSurface(renderer, s);
        if (blockTextures[i] == NULL)
        {
            printf("Error loading texture(%s): %s\n", textureFiles[i], SDL_GetError());
        }
        
        shadowTextures[i] = SDL_CreateTextureFromSurface(renderer, s);
        SDL_SetTextureBlendMode(shadowTextures[i], SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(shadowTextures[i], 127);


        SDL_DestroySurface(s);
    }
}

void DestroyBlockTextures()
{
    for (int i = 0; i < 7; i++)
    {
        SDL_DestroyTexture(blockTextures[i]);
    }
} 


// Draw a block to the grid
void DrawBlock(int x, int y, BlockState bs)
{
    
    SDL_FRect fullSquare = {
        x * BLOCK_WIDTH + GRID_X_OFFSET, 
        GRID_VIEW_HEIGHT - ((y + 1) * BLOCK_HEIGHT), 
        BLOCK_WIDTH,
        BLOCK_HEIGHT
    };

    if (bs == BS_EMPTY)
    {

        SDL_FRect innerSquare = {
            fullSquare.x,
            fullSquare.y,
            BLOCK_WIDTH - GRID_LINE_THICKNESS,
            BLOCK_HEIGHT - GRID_LINE_THICKNESS
        };
        
        if (x == 0) {
            innerSquare.x += GRID_LINE_THICKNESS;
            innerSquare.w -= GRID_LINE_THICKNESS;
        }

        SDL_SetRenderDrawColor(renderer, GRID_COLOR_R, GRID_COLOR_G, GRID_COLOR_B, 0);
        SDL_RenderFillRect(renderer, &fullSquare);

        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, 0);
        SDL_RenderFillRect(renderer, &innerSquare); 

        return;
    }

    SDL_RenderTexture(renderer, blockTextures[ bs - 1 ], NULL, &fullSquare);
}

void DrawShadowBlock(int x, int y, BlockState bs)
{
    SDL_FRect fullSquare = {
        x * BLOCK_WIDTH + GRID_X_OFFSET, 
        GRID_VIEW_HEIGHT - ((y + 1) * BLOCK_HEIGHT), 
        BLOCK_WIDTH,
        BLOCK_HEIGHT
    };

    SDL_RenderTexture(renderer, shadowTextures[ bs - 1 ], NULL, &fullSquare);

}

void DrawBoard(GameBoard *board)
{
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            DrawBlock(x, y, board->GetRow(y)[x]);
        }
    }
}

void DrawSwapPiece(int pieceIndex)
{
    if (pieceIndex == -1) return;

    float pieceX = SIDEBAR_OFFSET;
    float pieceY = NEXT_PIECE_PADDING;

    SDL_FRect fullSquare = {
        pieceX,
        pieceY, 
        SIDEBAR_BLOCK_WIDTH,
        SIDEBAR_BLOCK_HEIGHT
    };

    for (int k = 0; k < 4; k++)
    {
        fullSquare.x = pieceX + PIECES[pieceIndex].blocks[k][0] * SIDEBAR_BLOCK_WIDTH;
        fullSquare.y = pieceY + PIECES[pieceIndex].blocks[k][1] * SIDEBAR_BLOCK_HEIGHT;

        SDL_RenderTexture(renderer, blockTextures[PIECES[pieceIndex].color - 1], NULL, &fullSquare);
    }
}

void DrawSidebar(Player *player)
{

    DrawSwapPiece(player->swapPiece);

    float pieceX = GRID_VIEW_WIDTH + GRID_X_OFFSET + SIDEBAR_OFFSET;
    float pieceY = NEXT_PIECE_PADDING;

    SDL_FRect fullSquare = {
        pieceX,
        pieceY, 
        SIDEBAR_BLOCK_WIDTH,
        SIDEBAR_BLOCK_HEIGHT
    };

    int count = 0;
    int i = player->nextPieceIndex;
    while (count < FUTURE_PIECES_COUNT)
    {
        for (int k = 0; k < 4; k++)
        {
            fullSquare.x = pieceX + PIECES[ player->nextPieces[i] ].blocks[k][0] * SIDEBAR_BLOCK_WIDTH;
            fullSquare.y = pieceY + PIECES[ player->nextPieces[i] ].blocks[k][1] * SIDEBAR_BLOCK_HEIGHT;
            
            SDL_RenderTexture(renderer, blockTextures[ PIECES[ player->nextPieces[i] ].color - 1 ], NULL, &fullSquare);
        }

        pieceY += NEXT_PIECE_PADDING + SIDEBAR_BLOCK_HEIGHT * PIECES[ player->nextPieces[i] ].height;
        count++;
        
        if (i < FUTURE_PIECES_COUNT - 1)
            i++;
        else
            i = 0;
    }
}

void DrawPlayerPiece(Player *player)
{
    // X and Y are origin point of the "rectangle" of the piece
    int y = player->y;
    for (int i = 0; i < 4; i++)
    {
        DrawBlock(player->x + player->currentPiece.blocks[i][0], y - player->currentPiece.blocks[i][1], player->currentPiece.color);
    }

    while (!pieceShouldPlace(player->x, y, &player->currentPiece, &player->board)) 
    {
        y--;
    }

    for (int i = 0; i < 4; i++)
    {
        DrawShadowBlock(player->x + player->currentPiece.blocks[i][0], y - player->currentPiece.blocks[i][1], player->currentPiece.color);
    }



}

void ClearPlayerPiece(Player *player)
{
    for (int i = 0; i < 4; i++)
    {
        DrawBlock(player->x + player->currentPiece.blocks[i][0], player->y - player->currentPiece.blocks[i][1], BS_EMPTY);
    }

}