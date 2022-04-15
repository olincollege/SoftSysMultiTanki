#include "common.h"

extern int getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y);
extern void blit(SDL_Texture *texture, int x, int y, int center);
extern int getReflectedAngle(int angle, int axis);

extern Stage stage;