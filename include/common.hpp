#pragma once
#include <SDL3/SDL.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 600

#define GRID_VIEW_HEIGHT 800
#define GRID_VIEW_WIDTH 400

#define GRID_X_OFFSET 100

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

#define GRID_LINE_THICKNESS 2

#define PIECE_COUNT 7

#define BACKGROUND_COLOR 0x00000000
#define GRID_COLOR 0x00AAAAAA

#define COLOR_BLUE 0x00A0A0FF
#define COLOR_DEEP_BLUE 0x000404FF

#define COLOR_ORANGE 0x00FFA000
#define COLOR_RED 0x00FFA0A0

#define COLOR_GREEN 0x000AFF0A
#define COLOR_YELLOW 0x00FFFF0A
#define COLOR_VIOLET 0x00FF0AFF

enum BlockState : Uint32 {
    BS_EMPTY = 0,
    BS_BLUE = COLOR_BLUE,
    BS_DEEP_BLUE = COLOR_DEEP_BLUE,
    BS_ORANGE = COLOR_ORANGE,
    BS_RED = COLOR_RED,
    BS_GREEN = COLOR_GREEN,
    BS_YELLOW = COLOR_YELLOW,
    BS_VIOLET = COLOR_VIOLET
};

struct TetrisPiece {
    
    Uint32 color;
    int blocks[4][2];
};

struct InputState {
    bool left;
    bool right;
    bool up;
    bool down;
};

static const TetrisPiece PIECES[PIECE_COUNT] = {
    {
        COLOR_BLUE,
        { {0,0}, {0, 1}, {0, 2}, {0, 3} } // Line piece 
    },
    {
        COLOR_DEEP_BLUE,
        { {0,0}, {0, 1}, {0, 2}, {1, 2} } // L piece
    },
    {
        COLOR_ORANGE,
        { {1,0}, {1, 1}, {1, 2}, {0, 2} } // Reverse L piece
    },
    {
        COLOR_GREEN,
        { {0,1}, {1, 1}, {1, 0}, {2, 0} } // Z piece
    },
    {
        COLOR_RED,
        { {0, 0}, {1, 0}, {1, 1}, {2, 1} } // Reverse Z piece
    },
    {
        COLOR_YELLOW,
        { {0, 0}, {0, 1}, {1, 0}, {1, 1} } // Square piece
    },
    {
        COLOR_VIOLET,
        { {0, 1}, {1, 1}, {1, 0}, {2, 1} } // T piece
    }
};