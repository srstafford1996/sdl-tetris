#pragma once
#include "common.hpp"
#include "board.hpp"

#define FALL_DELAY 500
#define MOVE_DELAY 100
#define DROP_DELAY 300
#define ROTATE_DELAY 200

class GameBoard;

bool pieceShouldPlace(int x, int y, TetrisPiece *piece, GameBoard *board);

class Player
{
    void movePieceDown();

    bool checkCollision(int x, int y);
    void spawnNewPiece();
    
    void fastfall();
    void moveDown();
    void moveLateral(int direction);
    void rotate();  

public:
    int x, y;
    int swapPiece, nextPieceIndex;
    int nextPieces[FUTURE_PIECES_COUNT];

    bool overflow;
    bool swapUsed;

    GameBoard board;
    int currentPieceIndex;
    TetrisPiece currentPiece;

    Uint64 lastRotateTime;
    Uint64 lastMoveTime;
    Uint64 lastFallTime;
    Uint64 lastMoveDownTime;
    Uint64 lastDropTime;

    void Update(InputState is);
    void Reset();

    Player();
};
