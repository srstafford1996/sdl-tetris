#pragma once
#include "common.h"

void InitBoard();
void board_DeleteRow(int rowIndex);
BlockState* board_GetRowAt(int rowIndex);
bool board_RowIsFull(int rowIndex);