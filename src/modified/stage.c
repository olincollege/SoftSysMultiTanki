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

static SDL_Texture *targetterTexture;
static SDL_Texture *tankTextures[4];

void initStage(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	
	stage.pTail = &stage.pHead;
	stage.oTail = &stage.oHead;

	targetterTexture = loadTexture("gfx/crosshair010.png");
	tankTextures[0] = loadTexture("gfx/tank_blue.png");
	tankTextures[1] = loadTexture("gfx/tank_red.png");

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

static void logic(void)
{
	doPlayer();
	
	doBullets();

	doEffect();

	collisionWallsBullets();

	collisionPlayerWallsWithMove();

	collisionPlayerBullets();
}

static void draw(void)
{
	drawGrid();

	drawEffectUnder();
	
	drawPlayers();

	drawBullets();

	drawEffectOver();

	drawUI();
	
	blit(targetterTexture, app.playerInputs[app.playerIndex].mouse.x, app.playerInputs[app.playerIndex].mouse.y, 1);
}

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
