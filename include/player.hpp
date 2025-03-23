#pragma once
#include "common.hpp"
#include "board.hpp"

#define FALL_DELAY 500
#define MOVE_DELAY 100
#define DROP_DELAY 100
#define ROTATE_DELAY 150

class Player
{
    bool pieceShouldPlace();
    void movePieceDown();

    bool checkCollision(int x, int y);
    void spawnNewPiece();
    
    void moveDown();
    void moveLateral(int direction);
    void rotate();  

public:
    int x, y;
    int nextPieceIndex;
    int nextPieces[FUTURE_PIECES_COUNT];

    bool overflow;
    GameBoard board;
    TetrisPiece currentPiece;

    Uint32 lastRotateTime;
    Uint32 lastMoveTime;
    Uint32 lastFallTime;
    Uint32 lastMoveDownTime;

    void Update(InputState is);
    void Reset();

    Player();
};
