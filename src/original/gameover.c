#include "gameover.h"

static void logic(void);
static void draw(void);

//static SDL_Texture *titleBackground;
static SDL_Texture *logo;

void initGameOver(void)
{
    app.delegate.logic = logic;
	app.delegate.draw = draw;

    //titleBackground = loadTexture("gfx/title.png");
    logo = loadTexture("gfx/logo.png");
}

static void logic(void)
{
    if (app.playerInputs[0].keyboard[SDL_SCANCODE_Q])
	{
		initTitle();
	}
}

static void draw(void)
{
    //blitRotated(titleBackground, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0);
    //blitRotated(logo, SCREEN_WIDTH/2, 150, 0);

    app.fontScale = 0.5f;
    drawText("Temp Game Over", SCREEN_WIDTH / 2, 650, 128, 128, 128, TEXT_ALIGN_CENTER, 0);
}
