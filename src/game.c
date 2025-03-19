#include <SDL3/SDL.h>
#include <time.h>
#include <stdlib.h>

#include "game.h"
#include "render.h"
#include "common.h"
#include "board.h"

int playerX;
int playerY;

Uint64 fall_delay = 500;
Uint64 last_fall_time;

Uint64 move_delay = 100;
Uint64 last_move_time;

Uint64 rotate_delay = 150;
Uint64 last_rotate_time;

TetrisPiece current_piece;

void InitGame(SDL_Surface *s)
{
    RenderInit(s);
    InitBoard();
    // Initialize rand
    srand(time(NULL));

    playerX = 4;
    playerY = GRID_HEIGHT - 1;

    current_piece = PIECES[rand() % PIECE_COUNT];

    DrawPlayerPiece(playerX, playerY, &current_piece);
    DrawBoard();

    last_fall_time = SDL_GetTicks();
    last_move_time = SDL_GetTicks();
    last_rotate_time = SDL_GetTicks();
}

bool _pieceShouldPlace()
{
    for (int i = 0; i < 4; i++)
    {
        int nextY = playerY - current_piece.blocks[i][1] - 1;
        int nextX = playerX + current_piece.blocks[i][0];

        if (nextY < 0 || board_GetRowAt(nextY)[nextX] != BS_EMPTY)
            return true;
    }

    return false;
}

void _movePieceDown()
{
    if (_pieceShouldPlace())
    {
        // Place piece and check rows
        int bottomRow = -1;
        int topRow = -1;
        for (int i = 0; i < 4; i++)
        {
            int y = playerY - current_piece.blocks[i][1];
            if (bottomRow == -1)
            {
                bottomRow = y;
                topRow = y;
            }

            if (y < bottomRow)
                bottomRow = y;
            if (y > topRow)
                topRow = y;

            board_GetRowAt(y)[playerX + current_piece.blocks[i][0]] = current_piece.color;
        }

        for (int i = bottomRow; i <= topRow; i++)
        {
            if (board_RowIsFull(i))
            {
                board_DeleteRow(i);
                topRow--;
                i--;
            }
        }

        current_piece = PIECES[rand() % PIECE_COUNT];
        
        playerX = 4;
        playerY = GRID_HEIGHT - 1;
        DrawBoard();
    }
    else playerY--;
}

void _movePieceLateral(int direction)
{
    for (int i = 0; i < 4; i++)
    {
        int nextY = playerY - current_piece.blocks[i][1];
        int nextX = playerX + current_piece.blocks[i][0] + direction;
        
        if (nextX < 0 || nextX == GRID_WIDTH || board_GetRowAt(nextY)[nextX] != BS_EMPTY)
            return;
    } 

    playerX += direction;
}

void _rotatePiece()
{
    // Rotation method:
    // y = -prevX
    // x = prevY

    // Check for piece collisions
    for (int i = 0; i < 4; i++)
    {
        int nextY = playerY - (-current_piece.blocks[i][0]);
        int nextX = playerX + current_piece.blocks[i][1];

        if (nextY < 0 || nextX < 0 || nextY >= GRID_HEIGHT || nextX >= GRID_WIDTH || board_GetRowAt(nextY)[nextX] != BS_EMPTY)
            return;
    }

    for (int i = 0; i < 4; i++)
    {
        int temp = current_piece.blocks[i][0];
        current_piece.blocks[i][0] = current_piece.blocks[i][1];
        current_piece.blocks[i][1] = -temp;
    }
}

// So this game needs to draw different types of blocks
void UpdateGame()
{
    ClearPlayerPiece(playerX, playerY, &current_piece); 
    
    Uint64 tickStart = SDL_GetTicks();
    
    SDL_PumpEvents();
    const bool *keys = SDL_GetKeyboardState(NULL);

    bool canMove = tickStart - last_move_time >= move_delay;
    bool canRotate = tickStart - last_rotate_time >= rotate_delay;
    
    if (canMove && keys[SDL_SCANCODE_DOWN])
    {
        _movePieceDown();
        last_move_time = tickStart;
    }

    if (canRotate && keys[SDL_SCANCODE_UP])
    {
        _rotatePiece();
        last_rotate_time = tickStart;
    }

    if (canMove && keys[SDL_SCANCODE_RIGHT])
    {
        _movePieceLateral(1);
        last_move_time = tickStart;
    }
    else if (canMove && keys[SDL_SCANCODE_LEFT])
    {
        _movePieceLateral(-1);
        last_move_time = tickStart;
    }


    if (tickStart - last_fall_time >= fall_delay)
    {
        _movePieceDown();
        last_fall_time = tickStart;
    }

    DrawPlayerPiece(playerX, playerY, &current_piece);
}