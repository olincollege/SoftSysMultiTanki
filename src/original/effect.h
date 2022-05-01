#include "../common.h"

extern void blitRotated(SDL_Texture *texture, int x, int y, float angle);
extern SDL_Texture *loadTexture(char *filename);
extern void playSound(int id, int channel);

extern Stage stage;
extern App app;