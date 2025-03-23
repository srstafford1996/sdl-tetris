#include <SDL3/SDL.h>

#include <cstdio>

#include "board.hpp"
#include "player.hpp"
#include "render.hpp"



Player::Player() : lastRotateTime(0), lastMoveTime(0), lastFallTime(0), lastMoveDownTime(0), x(4), y(GRID_HEIGHT - 1), nextPieceIndex(0), overflow(0)
{
    currentPiece = PIECES[SDL_rand(PIECE_COUNT)];

    for (int i = 0; i < FUTURE_PIECES_COUNT; i++)
        nextPieces[i] = SDL_rand(PIECE_COUNT);
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

        if (topRow >= GRID_HEIGHT)
        {
            overflow = true;
            return;
        }
        spawnNewPiece();
        DrawBoard(board);
    }
    else
    {
        y--;
    }
}

bool Player::checkCollision(int x, int y)
{
    for (int i = 0; i < 4; i++)
    {
        int blockY = y - currentPiece.blocks[i][1];
        int blockX = x + currentPiece.blocks[i][0];

        if (blockX < 0 || blockX == GRID_WIDTH || board[blockY][blockX] != BS_EMPTY)
            return true;
    } 
    
    return false;
}

void Player::spawnNewPiece()
{
    currentPiece = PIECES[ nextPieces[nextPieceIndex] ];
    nextPieces[nextPieceIndex] = SDL_rand(PIECE_COUNT);
    
    if (nextPieceIndex == FUTURE_PIECES_COUNT - 1)
        nextPieceIndex = 0;
    else
        nextPieceIndex++;

    x = 4;
    y = GRID_HEIGHT - 1;

    while (checkCollision(x, y)) {
        y++;
    }

    DrawSidebar(nextPieces, nextPieceIndex);
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
    int nextBlocks[4][2];
    int minX = 500;
    int minY = 500;

    for (int i = 0; i < 4; i++)
    {
        nextBlocks[i][1] = currentPiece.blocks[i][0];
        nextBlocks[i][0] = (-currentPiece.blocks[i][1]);

        if (nextBlocks[i][0] < minX) minX = nextBlocks[i][0];
        if (nextBlocks[i][1] < minY) minY = nextBlocks[i][1];
    }

    for (int i = 0; i < 4; i++)
    {
        if (minX < 0) nextBlocks[i][0] -= minX;
        if (minY < 0) nextBlocks[i][1] -= minY;

        int nextX = x + nextBlocks[i][0];
        int nextY = y - nextBlocks[i][1];

        if (nextY < 0 || nextX < 0 || nextX >= GRID_WIDTH || board[nextY][nextX] != BS_EMPTY)
            return;
    }

    for (int i = 0; i < 4; i++)
    {
        currentPiece.blocks[i][0] = nextBlocks[i][0];
        currentPiece.blocks[i][1] = nextBlocks[i][1];
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

void Player::Reset()
{
    lastRotateTime = 0;
    lastMoveTime = 0;
    lastMoveDownTime = 0;    
    lastFallTime = 0;
    overflow = false;

    
    x = 4;
    y = GRID_HEIGHT - 1;

    currentPiece = PIECES[SDL_rand(PIECE_COUNT)];
    nextPieceIndex = 0;
    for (int i = 0; i < FUTURE_PIECES_COUNT; i++)
        nextPieces[i] = SDL_rand(PIECE_COUNT);
    
    board.ClearBoard();
    DrawBoard(board);
}