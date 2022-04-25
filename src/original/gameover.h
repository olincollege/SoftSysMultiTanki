#include "../common.h"

extern void initStage(void);
extern void blitRotated(SDL_Texture *texture, int x, int y, float angle);
extern SDL_Texture *loadTexture(char *filename);

extern void drawText(char *text, int x, int y, int r, int g, int b, int align, int maxWidth);
extern void initTitle(void);
extern void cleanup(void);

extern App app;