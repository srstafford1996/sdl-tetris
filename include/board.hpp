#pragma once
#include "common.hpp"

class GameBoard
{
    BlockState blocks[GRID_HEIGHT][GRID_WIDTH];
    short row_indexes[GRID_HEIGHT];

public:
    GameBoard();

    BlockState* GetRow(int rowIndex);
    void DeleteRow(int rowIndex);
    bool RowIsFull(int rowIndex);

    BlockState* operator [](int rowIndex);
};