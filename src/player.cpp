#include <SDL3/SDL.h>

#include <cstdio>

#include "board.hpp"
#include "player.hpp"
#include "render.hpp"



Player::Player() : lastRotateTime(0), lastMoveTime(0), lastFallTime(0), lastMoveDownTime(0), x(4), y(GRID_HEIGHT - 1)
{
    currentPiece = PIECES[SDL_rand(PIECE_COUNT)];
}

bool Player::pieceShouldPlace()
{
    for (int i = 0; i < 4; i++)
    {
        int nextY = y - currentPiece.blocks[i][1] - 1;
        int nextX = x + currentPiece.blocks[i][0];

        if (nextY < 0 || board[nextY][nextX] != BS_EMPTY)
            return true;
    }

    return false;
}


void Player::moveDown()
{
    if (pieceShouldPlace())
    {
        // Place piece and check rows
        int bottomRow = -1;
        int topRow = -1;
        for (int i = 0; i < 4; i++)
        {
            int blockY = y - currentPiece.blocks[i][1];
            if (bottomRow == -1)
            {
                bottomRow = blockY;
                topRow = blockY;
            }

            if (blockY < bottomRow)
                bottomRow = blockY;
            if (y > topRow)
                topRow = blockY;

            board[blockY][x + currentPiece.blocks[i][0]] = (BlockState) currentPiece.color;
        }

        for (int i = bottomRow; i <= topRow; i++)
        {
            if (board.RowIsFull(i))
            {
                board.DeleteRow(i);
                topRow--;
                i--;
            }
        }

        currentPiece = PIECES[SDL_rand(PIECE_COUNT)];
        
        x = 4;
        y = GRID_HEIGHT - 1;
        DrawBoard(board);
    }
    else
    {
        y--;
    }
}


void Player::moveLateral(int direction)
{
    for (int i = 0; i < 4; i++)
    {
        int nextY = y - currentPiece.blocks[i][1];
        int nextX = x + currentPiece.blocks[i][0] + direction;
        
        if (nextX < 0 || nextX == GRID_WIDTH || board[nextY][nextX] != BS_EMPTY)
            return;
    } 
    
    x += direction;
}

void Player::rotate()
{
    // Rotation method:
    // y = -prevX
    // x = prevY

    // Check for piece collisions
    for (int i = 0; i < 4; i++)
    {
        int nextY = y - currentPiece.blocks[i][0];
        int nextX = x + (-currentPiece.blocks[i][1]);

        if (nextY < 0 || nextX < 0 || nextY >= GRID_HEIGHT || nextX >= GRID_WIDTH || board[nextY][nextX] != BS_EMPTY)
            return;
    }

    for (int i = 0; i < 4; i++)
    {
        int temp = currentPiece.blocks[i][0];
        currentPiece.blocks[i][0] = -currentPiece.blocks[i][1];
        currentPiece.blocks[i][1] = temp;
    }

}

void Player::Update(InputState is)
{
    Uint64 tickStart = SDL_GetTicks();

    bool canMove = tickStart - lastMoveTime >= MOVE_DELAY;
    bool canRotate = tickStart - lastRotateTime >= MOVE_DELAY;

   if (canMove && is.down)
    {
        moveDown();
        lastMoveTime = tickStart;
    }

    if (canRotate && is.up)
    {
        rotate();
        lastRotateTime = tickStart;
    }

    if (canMove && is.right)
    {
        moveLateral(1);
        lastMoveTime = tickStart;
    }
    else if (canMove && is.left)
    {
        moveLateral(-1);
        lastMoveTime = tickStart;
    }
    
    if (tickStart - lastFallTime >= FALL_DELAY)
    {
        moveDown();
        lastFallTime = tickStart;
    }
}