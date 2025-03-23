#pragma once
#include "common.hpp"

class GameBoard
{
    BlockState blocks[GRID_HEIGHT + 4][GRID_WIDTH];
    short row_indexes[GRID_HEIGHT + 4];

public:
    GameBoard();

    BlockState* GetRow(int rowIndex);
    void DeleteRow(int rowIndex);
    bool RowIsFull(int rowIndex);
    void ClearBoard();

    BlockState* operator [](int rowIndex);
};