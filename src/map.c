#include "map.h"

int gdata[GRID_HEIGHT][GRID_WIDTH] = 
{
    { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  0,  0,  0,  0,  0,  0, 22,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  0,  0,  0,  0,  0,  0, 22,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  0,  0,  0,  0,  0,  0, 22,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  0,  0,  0,  0,  0,  0, 22,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  0,  0, 29, 26, 26, 26, 24,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  0,  0, 22,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  0,  0, 22,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1, 23, 25, 25, 27, 26, 26, 24,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1, 21,  1,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1, 21,  1,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1, 23, 25, 25, 25, 28,  1,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1, 21,  1,  1,  1,  1,  1,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1, 21,  1,  1,  1,  1,  1,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1, 21,  1,  1,  1,  1,  1,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1, 21,  1,  1,  1,  1,  1,  1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
};

int wdata[GRID_HEIGHT][GRID_WIDTH] = 
{
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  3,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  4,  0},
    { 0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0},
    { 0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0},
    { 0,  7,  0,  0,  0,  0,  0,  0,  0, 13, 19, 14,  0,  0,  0,  0,  0,  0,  0,  7,  0},
    { 0,  7,  0,  0,  0,  0,  0,  0,  0, 15, 20, 16,  0,  0,  0,  0,  0,  0,  0,  7,  0},
    { 0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0},
    { 0,  9,  8,  8,  8, 11,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12,  8,  8,  8, 10,  0},
    { 0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0},
    { 0,  7,  0,  0,  0,  0,  0,  0,  0, 13, 19, 14,  0,  0,  0,  0,  0,  0,  0,  7,  0},
    { 0,  7,  0,  0,  0,  0,  0,  0,  0, 15, 20, 16,  0,  0,  0,  0,  0,  0,  0,  7,  0},
    { 0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0},
    { 0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0},
    { 0,  5,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  6,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
};

SDL_Texture* loadSprite(int num)
{
    SDL_Texture *t;
    switch(num)
    {
        case 0:
            t = loadTexture("gfx/tile/tileSand1.png");
            break;
        case 1:
            t = loadTexture("gfx/tile/tileGrass1.png");
            break;
        case 2:
            t = loadTexture("gfx/tile/tileGrass_transitionE.png");
            break;
        case 3:
            t = loadTexture("gfx/tile/tile_118.png");
            break;
        case 4:
            t = loadTexture("gfx/tile/tile_119.png");
            break;
        case 5:
            t = loadTexture("gfx/tile/tile_145.png");
            break;
        case 6:
            t = loadTexture("gfx/tile/tile_146.png");
            break;
        case 7:
            t = loadTexture("gfx/tile/tile_147.png");
            break;
        case 8:
            t = loadTexture("gfx/tile/tile_120.png");
            break;
        case 9:
            t = loadTexture("gfx/tile/tile_148.png");
            break;
        case 10:
            t = loadTexture("gfx/tile/tile_149.png");
            break;
        case 11:
            t = loadTexture("gfx/tile/tile_123.png");
            break;
        case 12:
            t = loadTexture("gfx/tile/tile_151.png");
            break;
        case 13:
            t = loadTexture("gfx/tile/tile_125.png");
            break;
        case 14:
            t = loadTexture("gfx/tile/tile_126.png");
            break;
        case 15:
            t = loadTexture("gfx/tile/tile_152.png");
            break;
        case 16:
            t = loadTexture("gfx/tile/tile_153.png");
            break;
        case 17:
            t = loadTexture("gfx/tile/tile_176.png");
            break;
        case 18:
            t = loadTexture("gfx/tile/tile_177.png");
            break;
        case 19:
            t = loadTexture("gfx/tile/tile_203.png");
            break;
        case 20:
            t = loadTexture("gfx/tile/tile_204.png");
            break;
        case 21:
            t = loadTexture("gfx/tile/tileGrass_roadNorth.png");
            break;
        case 22:
            t = loadTexture("gfx/tile/tileSand_roadNorth.png");
            break;
        case 23:
            t = loadTexture("gfx/tile/tileGrass_roadCornerLR.png");
            break;
        case 24:
            t = loadTexture("gfx/tile/tileSand_roadCornerUL.png");
            break;
        case 25:
            t = loadTexture("gfx/tile/tileGrass_roadEast.png");
            break;
        case 26:
            t = loadTexture("gfx/tile/tileSand_roadEast.png");
            break;
        case 27:
            t = loadTexture("gfx/tile/tileGrass_roadTransitionE_dirt.png");
            break;
        case 28:
            t = loadTexture("gfx/tile/tileGrass_roadCornerUL.png");
            break;
        case 29:
            t = loadTexture("gfx/tile/tileSand_roadCornerLR.png");
            break;
    }
    return t;
}