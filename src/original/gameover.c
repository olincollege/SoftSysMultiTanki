/*
	Copyright 2022 SeungU Lyu
*/

#include "gameover.h"

static void logic(void);
static void draw(void);
static void reset(void);

static SDL_Texture *logo;
static SDL_Texture *winner;

extern float title_scale;
extern int increase;
extern int gameover;

int win = 0;

/*
 * Function: initGameOver
 * ----------------------------
 *  Initialize game over screen by changing logic/draw delegate and loading necessary textures.
 *  The loaded texture depends on the win condition of the user.
 */
void initGameOver(int playerIndex)
{
    app.delegate.logic = logic;
	app.delegate.draw = draw;

    if (playerIndex == app.playerIndex)
    {
        logo = loadTexture("gfx/victory.png");
        win = 1;
    }
    else
    {
        logo = loadTexture("gfx/defeat.png");
    }

    if (playerIndex == 0)
    {
        winner = loadTexture("gfx/tank_blue.png");
    }
    else if (playerIndex == 1)
    {
        winner = loadTexture("gfx/tank_red.png");
    }
    else if (playerIndex == 2)
    {
        winner = loadTexture("gfx/tank_green.png");
    }
    else if (playerIndex == 3)
    {
        winner = loadTexture("gfx/tank_sand.png");
    }

    title_scale = 0.5f;
    increase = 0;
}

/*
 * Function: logic
 * ----------------------------
 *  Logic for the game over screen.
 *  Detects if user input was SPACE or ESCAPE.
 *  If SPACE, return to the title screen.
 *  If ESC, exit the game.
 */
static void logic(void)
{
    if (app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_SPACE])
	{
        app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_SPACE] = 0;
        reset();
		initTitle();
	}

    if (app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_ESCAPE])
	{
        app.playerInputs[app.playerIndex].keyboard[SDL_SCANCODE_ESCAPE] = 0;
		cleanup();
        exit(0);
	}
}

/*
 * Function: draw
 * ----------------------------
 *  Draw the game over screen.
 *  The game over screen graphics depends on the win condition of the user.
 */
static void draw(void)
{
    blitRotated(logo, SCREEN_WIDTH/2, 200, 0);
    blitRotated(winner, SCREEN_WIDTH/2, 400, 0);
    app.fontScale = 1.0f;
    if (win == 1)
    {
        drawText("WINNER!", SCREEN_WIDTH / 2, 450, 0, 134, 212, TEXT_ALIGN_CENTER, 0);
    }
    else
    {
        drawText("WINNER!", SCREEN_WIDTH / 2, 450, 232, 55, 45, TEXT_ALIGN_CENTER, 0);
    }
    

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
    if (win == 1)
    {
        drawText("Game Over! Press Space To Restart, ESC to Quit", SCREEN_WIDTH / 2, 650, 0, 134, 212, TEXT_ALIGN_CENTER, 0);
    }
    else
    {
        drawText("Game Over! Press Space To Restart, ESC to Quit", SCREEN_WIDTH / 2, 650, 232, 55, 45, TEXT_ALIGN_CENTER, 0);
    }
}

/*
 * Function: reset
 * ----------------------------
 *  Reset the game by freeing allocated datas and resetting variable values.
 */
void reset(void)
{
    //TODO: Implement this function.
}
