#include "../common.h"

extern void blitRotated(SDL_Texture *texture, int x, int y, float angle);
extern void blit(SDL_Texture *texture, int x, int y, int center);
extern void calcSlope(int x1, int y1, int x2, int y2, float *dx, float *dy);
extern void blitPoint(float x, float y);
extern SDL_Texture *loadTexture(char *filename);

extern App app;
extern Stage stage;
