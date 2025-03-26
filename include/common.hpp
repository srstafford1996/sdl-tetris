#pragma once
#include <SDL3/SDL.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1000

#define GRID_VIEW_HEIGHT 800
#define GRID_VIEW_WIDTH 400

#define GRID_X_OFFSET 300

#define SIDEBAR_WIDTH 200
#define SIDEBAR_OFFSET 100
#define SIDEBAR_BLOCK_HEIGHT 30
#define SIDEBAR_BLOCK_WIDTH 30

#define NEXT_PIECE_PADDING 40

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

#define GRID_LINE_THICKNESS 2

#define PIECE_COUNT 7
#define FUTURE_PIECES_COUNT 5

#define BACKGROUND_COLOR_R 0x00
#define BACKGROUND_COLOR_G 0x00
#define BACKGROUND_COLOR_B 0x00

#define GRID_COLOR_R 0xAA
#define GRID_COLOR_G 0xAA
#define GRID_COLOR_B 0xAA

#define COLOR_BLUE 0x00A0A0FF
#define COLOR_DEEP_BLUE 0x000404FF

#define COLOR_ORANGE 0x00FFA000
#define COLOR_RED 0x00FFA0A0

#define COLOR_GREEN 0x000AFF0A
#define COLOR_YELLOW 0x00FFFF0A
#define COLOR_VIOLET 0x00FF0AFF

enum BlockState {
    BS_EMPTY = 0,
    BS_BLUE,
    BS_DEEP_BLUE,
    BS_ORANGE,
    BS_RED,
    BS_GREEN,
    BS_YELLOW,
    BS_VIOLET 
};

struct TetrisPiece {
    
    BlockState color;
    int height;
    int blocks[4][2];
};

struct InputState {
    bool left;
    bool right;
    bool up;
    bool down;
    bool fastfall;
    bool swap;
    bool pause;
};

static const TetrisPiece PIECES[PIECE_COUNT] = {
    {
        BS_BLUE,
        1,
        { {0,0}, {1, 0}, {2, 0}, {3, 0} } // Line piece 
    },
    {
        BS_DEEP_BLUE,
        2,
        { {0,1}, {1, 1}, {2, 1}, {2, 0} } // L piece
    },
    {
        BS_ORANGE,
        2,
        { {0,1}, {1, 1}, {2, 1}, {0, 0} } // Reverse L piece
    },
    {
        BS_GREEN,
        2,
        { {0,1}, {1, 1}, {1, 0}, {2, 0} } // Z piece
    },
    {
        BS_RED,
        2,
        { {0, 0}, {1, 0}, {1, 1}, {2, 1} } // Reverse Z piece
    },
    {
        BS_YELLOW,
        2,
        { {0, 0}, {0, 1}, {1, 0}, {1, 1} } // Square piece
    },
    {
        BS_VIOLET,
        2,
        { {0, 1}, {1, 1}, {1, 0}, {2, 1} } // T piece
    }
};