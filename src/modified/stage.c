/*
Copyright (C) 2015-2018 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "stage.h"

static void logic(void);
static void draw(void);
static void drawGrid(void);
static void drawUI(void);
static void drawCountDown(void);
static void checkGameOver(void);

static SDL_Texture *targetterTexture;
static SDL_Texture *tankTextures[4];
static SDL_Texture *countdownTextures[5];

int countdown;
int gameover;
int lastLiveIndex;

/*
 * Function: initStage
 * ----------------------------
 *  Initialized the game stage the changing logic/draw delegate and loading necessary textures.
 *	MapTile structs are created based on map data stored inside map.c.
 */
void initStage(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	countdown = FPS * 4;
	gameover = 0;
	lastLiveIndex = 0;
	
	stage.pTail = &stage.pHead;
	stage.oTail = &stage.oHead;

	targetterTexture = loadTexture("gfx/crosshair010.png");
	tankTextures[0] = loadTexture("gfx/tank_blue.png");
	tankTextures[1] = loadTexture("gfx/tank_red.png");
	countdownTextures[0] = loadTexture("gfx/1.png");
	countdownTextures[1] = loadTexture("gfx/2.png");
	countdownTextures[2] = loadTexture("gfx/3.png");
	countdownTextures[3] = loadTexture("gfx/start.png");
	countdownTextures[4] = loadTexture("gfx/gameover.png");

	for (int y = 0 ; y < GRID_HEIGHT ; y += 1)
	{
		for (int x = 0 ; x < GRID_WIDTH ; x += 1)
		{
			MapTile* g = malloc(sizeof(MapTile));
			memset(g, 0, sizeof(MapTile));

			g->x = x * GRID_SIZE;
			g->y = y * GRID_SIZE;

			g->texture = loadSprite(gdata[y][x]);
			stage.ground[y][x] = g;

			if (wdata[y][x] != 0) 
			{
				MapTile* w = malloc(sizeof(MapTile));
				memset(w, 0, sizeof(MapTile));

				w->x = x * GRID_SIZE;
				w->y = y * GRID_SIZE;

				w->texture = loadSprite(wdata[y][x]);

				stage.oTail->next = w;
				stage.oTail = w;
			}
		}
	}
	
	initPlayer();
}

/*
 * Function: logic
 * ----------------------------
 *  Execute functions related to the game logic one by one. 
 *	Player, Bullet, Effect calculation, collision detection, and check game over all happens in this function.
 */
static void logic(void)
{
	if (countdown == 0 && gameover == 1)
	{
		initGameOver(lastLiveIndex);
	}
	else if (countdown > 0)
	{
		return;
	}

	doPlayer();
	
	doBullets();

	doEffect();

	collisionWallsBullets();

	collisionPlayerWallsWithMove();

	collisionPlayerBullets();

	checkGameOver();
}

/*
 * Function: draw
 * ----------------------------
 *  Draw all necessary graphics for the game one by one.
 */
static void draw(void)
{
	drawGrid();

	drawEffectUnder();
	
	drawPlayers();

	drawBullets();

	drawEffectOver();

	drawUI();
	
	blit(targetterTexture, app.playerInputs[app.playerIndex].mouse.x, app.playerInputs[app.playerIndex].mouse.y, 1);

	if (countdown > 0 && gameover == 0)
	{
		drawCountDown();
		countdown -= 1;
	} 
	else if (countdown > 0 && gameover == 1)
	{
		blitRotated(countdownTextures[4], SCREEN_WIDTH / 2, SCREEN_HEIGHT/2, 0);
		countdown -= 1;
	}
}
/*
 * Function: checkGameOver
 * ----------------------------
 *  Check the game over state of the game by going through all the players and checking their health.
 *	If only one player is alive, toggle the gameover flag and get ready to end the game.
 */
static void checkGameOver(void)
{
	Player *p;

	int livePlayersNum;
	
	for (p = stage.pHead.next ; p != NULL ; p = p->next)
	{
		if (p->isBody == 1)
		{
			if (p->health > 0)
			{
				livePlayersNum += 1;
				lastLiveIndex = p->playerIndex;
			}
		}
		else
		{
			continue;
		}
	}

	if (livePlayersNum == 1 && gameover == 0)
	{
		gameover = 1;
		countdown = FPS * 2;
		playSound(SND_GAME_START, CH_GAME);
	}
}

/*
 * Function: drawGrid
 * ----------------------------
 *  Draws the game map based on MapTile data created in the initStage function. 
 */
static void drawGrid(void)
{
	int x, y;
	
	for (y = 0 ; y < GRID_HEIGHT ; y += 1)
	{
		for (x = 0 ; x < GRID_WIDTH ; x += 1)
		{
			MapTile* m = stage.ground[y][x];
			blit(m->texture, m->x, m->y, 0);
		}
	}

	MapTile *m;
	
	for (m = stage.oHead.next ; m != NULL ; m = m->next)
	{
		blit(m->texture, m->x, m->y, 0);
	}
}

/*
 * Function: drawUI
 * ----------------------------
 *  Draws the game UI based on current health of each player.
 */
static void drawUI(void)
{
	Player *p;
	
	for (p = stage.pHead.next ; p != NULL ; p = p->next)
	{
		if (p->isBody == 1 && p->playerIndex == 0)
		{
			int x = GRID_SIZE * 0.5;
			int y = GRID_SIZE * 0.5;

			for (int i = 0; i < p->health; i++)
			{
				blitRotated(tankTextures[0], x, y, 0);
				x += GRID_SIZE;
			}
		}

		if (p->isBody == 1 && p->playerIndex == 1)
		{
			int x = SCREEN_WIDTH - GRID_SIZE * 0.5;
			int y = GRID_SIZE * 0.5;

			for (int i = 0; i < p->health; i++)
			{
				blitRotated(tankTextures[1], x, y, 0);
				x -= GRID_SIZE;
			}
		}
	}
}

/*
 * Function: drawCountDown
 * ----------------------------
 *  Draws the game start countdown based on the countdown variable.
 *	Also plays sound but it is not the best behavior, will need to create another function just for that.
 */
static void drawCountDown(void)
{
	if (countdown > FPS * 3)
	{
		blitRotated(countdownTextures[2], SCREEN_WIDTH / 2, SCREEN_HEIGHT/2, 0);
	}
	else if (countdown > FPS * 2)
	{
		blitRotated(countdownTextures[1], SCREEN_WIDTH / 2, SCREEN_HEIGHT/2, 0);
	} 
	else if (countdown > FPS * 1)
	{
		blitRotated(countdownTextures[0], SCREEN_WIDTH / 2, SCREEN_HEIGHT/2, 0);
	}
	else if (countdown > 0)
	{
		blitRotated(countdownTextures[3], SCREEN_WIDTH / 2, SCREEN_HEIGHT/2, 0);
	}

	if ((countdown == FPS * 4 || countdown == FPS * 3 || countdown == FPS * 2) && gameover == 0)
	{
		playSound(SND_COUNTDOWN, CH_GAME);
	} 
	else if (countdown == FPS * 1 && gameover == 0)
	{
		playSound(SND_GAME_START, CH_GAME);
	}
}
