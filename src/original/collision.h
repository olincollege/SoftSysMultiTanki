#include "../common.h"

extern int getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y);
extern void blit(SDL_Texture *texture, int x, int y, int center);
extern int getReflectedAngle(int angle, int axis);
extern int getRectOverlap(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
extern int getPointInsideRect(int x1, int y1, int x2, int y2, int x, int y);
extern void playSound(int id, int channel);

extern Stage stage;