/*
	Copyright 2022 SeungU Lyu
*/

#include "title.h"

static void logic(void);
static void draw(void);

static SDL_Texture *titleBackground;
static SDL_Texture *logo;

float title_scale;;
int increase;
int current_choice;

/*
 * Function: initTitle
 * ----------------------------
 *  Initialize title screen by changing logic/draw delegate and loading necessary textures.
 */
void initTitle(void)
{
    app.delegate.logic = logic;
	app.delegate.draw = draw;

    titleBackground = loadTexture("gfx/title.png");
    logo = loadTexture("gfx/logo4.png");

    title_scale = 0.5f;
    increase = 0;
    current_choice = 0;
}

/*
 * Function: logic
 * ----------------------------
 *  Logic for the title screen.
 *  Detects if user input was SPACE or W/S
 *  Select menu on SPACE press
 */
static void logic(void)
{
    if (app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_SPACE])
	{
        if (current_choice == 0)
        {
            app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_SPACE] = 0;
            app.playerIndex = 0;
            initStage();
        }
        else if (current_choice == 1)
        {
            app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_Q] = 0;
            app.isMulti = 1;
            app.maxPlayer = 2;
            initStage();
        }
        else if (current_choice == 2)
        {
            app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_ESCAPE] = 0;
            cleanup();
            exit(0);
        }
	}
    else if (app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_W])
    {
        app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_W] = 0;
        current_choice = current_choice - 1 >= 0 ? current_choice - 1 : 2;
    }
    else if (app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_S])
    {
        app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_S] = 0;
        current_choice = current_choice + 1 <= 2 ? current_choice + 1 : 0;
    }
}

/*
 * Function: draw
 * ----------------------------
 *  Draw the title screen.
 */
static void draw(void)
{
    blitRotated(titleBackground, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0);
    blitRotated(logo, SCREEN_WIDTH/2, 200, 0);

    if (increase)
    {
        title_scale += 0.007f;
    }
    else 
    {
        title_scale -= 0.007f;
    }

    if (title_scale >= 0.6)
    {
        increase = 0;
    }
    else if (title_scale <= 0.5)
    {
        increase = 1;
    }

    app.fontScale = title_scale;

    app.fontScale = 0.5;
    if (current_choice == 0)
    {
        app.fontScale = title_scale;
    }
    drawText("Single Player", SCREEN_WIDTH / 2, 550, 0, 134, 212, TEXT_ALIGN_CENTER, 0);

    app.fontScale = 0.5;
    if (current_choice == 1)
    {
        app.fontScale = title_scale;
    }
    drawText("Multi Player", SCREEN_WIDTH / 2, 600, 0, 134, 212, TEXT_ALIGN_CENTER, 0);

    app.fontScale = 0.5;
    if (current_choice == 2)
    {
        app.fontScale = title_scale;
    }
    drawText("Quit", SCREEN_WIDTH / 2, 650, 0, 134, 212, TEXT_ALIGN_CENTER, 0);
}
