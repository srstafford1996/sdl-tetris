#include "common.hpp"
#include "board.hpp"

void _swap(short *a, short *b)
{
    short temp = *a;
    *a = *b;
    *b = temp;
}


GameBoard::GameBoard()
{
    for(int i = 0; i < GRID_HEIGHT; i++)
    {
        row_indexes[i] = i;
        for(int j = 0; j < GRID_WIDTH; j++)
            blocks[i][j] = BS_EMPTY;
    }
}

BlockState* GameBoard::GetRow(int rowIndex)
{
    return blocks[row_indexes[rowIndex]];
}

BlockState* GameBoard::operator[](int rowIndex)
{
    return blocks[row_indexes[rowIndex]];
}

void GameBoard::DeleteRow(int rowIndex)
{
    // Clear line and put it at the top
    for (int i = 0; i < GRID_WIDTH; i++)
        blocks[ row_indexes[rowIndex] ][i] = BS_EMPTY;
    
    _swap(&row_indexes[rowIndex], &row_indexes[GRID_HEIGHT - 1]);

    for (int i = rowIndex; i < GRID_HEIGHT - 2; i++)
    {
        _swap(&row_indexes[i], &row_indexes[i + 1]);
    }
}

bool GameBoard::RowIsFull(int rowIndex)
{
    for (int i = 0; i < GRID_WIDTH; i++)
        if (blocks[ row_indexes[rowIndex] ][i] == BS_EMPTY) return false;
    
    return true;
}