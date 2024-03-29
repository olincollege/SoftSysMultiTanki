#include "../common.h"

extern void fireBullet(Player* playerHead);
extern float getAngle(int x1, int y1, int x2, int y2);
extern SDL_Texture *loadTexture(char *filename);
extern void blitRotated(SDL_Texture *texture, int x, int y, float angle);
extern void getEmptyPosition(int *x, int *y);
extern void playSound(int id, int channel);

extern App app;
extern Stage stage;
