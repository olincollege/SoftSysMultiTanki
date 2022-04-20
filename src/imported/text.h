/*
Copyright (C) 2021 Parallel Realities
*/

#include "../common.h"
#include "SDL2/SDL_ttf.h"

#define FONT_SIZE            96
#define FONT_TEXTURE_SIZE    2048
#define MAX_GLYPHS           400
#define MAX_GLYPH_SIZE       8
#define MAX_WORD_LENGTH      32
#define MAX_LINE_LENGTH      1024

extern SDL_Texture *toTexture(SDL_Surface *surface, int destroySurface);

extern App app;
