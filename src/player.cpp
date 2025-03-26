#include <SDL3/SDL.h>

#include <cstdio>
#include <cmath>

#include "board.hpp"
#include "player.hpp"
#include "render.hpp"


bool pieceShouldPlace(int x, int y, TetrisPiece *currentPiece, GameBoard *board)
{
    for (int i = 0; i < 4; i++)
    {
        int nextY = y - currentPiece->blocks[i][1] - 1;
        int nextX = x + currentPiece->blocks[i][0];

        if (nextY < 0 || board->GetRow(nextY)[nextX] != BS_EMPTY)
            return true;
    }

    return false;
}

Player::Player() : lastRotateTime(0), lastMoveTime(0), lastFallTime(0), lastMoveDownTime(0), x(4),
    y(GRID_HEIGHT - 1), nextPieceIndex(0), overflow(0), swapUsed(0),
    swapPiece(-1), lastInputState{0}, lastInputPressTime{0}, paused(0), score(0), linesCleared(0)
{
    currentPieceIndex = SDL_rand(PIECE_COUNT);
    currentPiece = PIECES[currentPieceIndex];

    for (int i = 0; i < FUTURE_PIECES_COUNT; i++)
        nextPieces[i] = SDL_rand(PIECE_COUNT);
}


void Player::parseInputState(Uint64 tickStart, InputState *is)
{
    InputState state = *is;

    is->up = parseInput(lastInputState.up, is->up, tickStart, 0);
    is->down = parseInput(lastInputState.down, is->down, tickStart, 1);
    is->left = parseInput(lastInputState.left, is->left, tickStart, 2);
    is->right = parseInput(lastInputState.right, is->right, tickStart, 3);
    is->swap = parseInput(lastInputState.swap, is->swap, tickStart, 4);
    is->fastfall = parseInput(lastInputState.fastfall, is->fastfall, tickStart, 5);
    is->pause = parseInput(lastInputState.pause, is->pause, tickStart, 6);

    lastInputState = state;
}

bool Player::parseInput(bool oldState, bool currState, Uint64 &tickStart, const Uint8 index)
{
    if (currState)
    {
        if (!oldState || (tickStart - lastInputPressTime[index] >= BUTTON_REPEAT_DELAY))
        {
            lastInputPressTime[index] = SDL_GetTicks();
            return true;
        }
    }

    return false;
}

void Player::moveDown()
{
    if (pieceShouldPlace(x, y, &currentPiece, &board))
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
        
        int lines = 0;
        for (int i = bottomRow; i <= topRow; i++)
        {

            if (board.RowIsFull(i))
            {
                lines++;
                board.DeleteRow(i);
                topRow--;
                i--;
            }
        }

        if(lines > 0)
        {
            linesCleared += lines;
            score += std::pow(5, lines);
        }

        if (topRow >= GRID_HEIGHT)
        {
            overflow = true;
            return;
        }

        swapUsed = false;
        spawnNewPiece();
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

    currentPieceIndex = nextPieces[nextPieceIndex];
    currentPiece = PIECES[ currentPieceIndex ];
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

    int edgeDiff = 0;
    for (int i = 0; i < 4; i++)
    {
        if (minX < 0) nextBlocks[i][0] -= minX;
        if (minY < 0) nextBlocks[i][1] -= minY;

        int nextX = x + nextBlocks[i][0];
        int nextY = y - nextBlocks[i][1];

        if (nextY < 0 || nextX < 0 || board[nextY][nextX] != BS_EMPTY)
            return;

        // Trick for moving if we're blocked by side
        if ( nextX >= GRID_WIDTH && (nextX - (GRID_WIDTH - 1)) > edgeDiff)
        {
            edgeDiff = nextX - (GRID_WIDTH - 1);
        }
    }

    if (edgeDiff != 0)
    {
        x -= edgeDiff;
        for (int i = 0; i < 4; i++)
        {
            int nextX = x + nextBlocks[i][0];
            int nextY = y - nextBlocks[i][1];

            if (nextY < 0 || nextX < 0 || board[nextY][nextX] != BS_EMPTY)
            {
                x += edgeDiff;
                return;
            }

        }
    }

    for (int i = 0; i < 4; i++)
    {
        currentPiece.blocks[i][0] = nextBlocks[i][0];
        currentPiece.blocks[i][1] = nextBlocks[i][1];
    }

}

void Player::fastfall()
{
    while (!pieceShouldPlace(x, y, &currentPiece, &board)) moveDown();

    moveDown();
}
void Player::Update(InputState is)
{
    Uint64 tickStart = SDL_GetTicks();

    parseInputState(tickStart, &is);

    if (is.pause)
        paused = !paused;

    if (paused)
    {
        return;
    }

    bool canMove = tickStart - lastMoveTime >= MOVE_DELAY;
    bool canDrop = tickStart - lastDropTime >= DROP_DELAY;
    bool canRotate = tickStart - lastRotateTime >= MOVE_DELAY;

    if (canDrop && is.fastfall)
    {
        fastfall();
        lastDropTime = tickStart;
        return;
    }

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

    if (is.swap && !swapUsed)
    {
        swapUsed = true;
        if (swapPiece != -1)
        {
            currentPiece = PIECES[ swapPiece ];

            int temp = currentPieceIndex;
            currentPieceIndex = swapPiece;
            swapPiece = temp;

            x = 4;
            y = GRID_HEIGHT - 1;

            while (checkCollision(x, y))
            {
                y++;
            }
        }
        else
        {
            swapPiece = currentPieceIndex;
            spawnNewPiece();
        }
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

    currentPieceIndex = SDL_rand(PIECE_COUNT);
    currentPiece = PIECES[currentPieceIndex];
    nextPieceIndex = 0;
    for (int i = 0; i < FUTURE_PIECES_COUNT; i++)
        nextPieces[i] = SDL_rand(PIECE_COUNT);
    
    board.ClearBoard();
}