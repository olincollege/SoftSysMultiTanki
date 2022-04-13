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

static SDL_Texture *targetterTexture;

void initStage(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	
	stage.pTail = &stage.pHead;
	stage.bTail = &stage.bHead;
	stage.oTail = &stage.oHead;

	targetterTexture = loadTexture("gfx/crosshair010.png");

	for (int y = 0 ; y < GRID_HEIGHT ; y += 1)
	{
		for (int x = 0 ; x < GRID_WIDTH ; x += 1)
		{
			printf("(%i, %i)\n", x, y);
			MapTile* g = malloc(sizeof(MapTile));
			memset(g, 0, sizeof(MapTile));

			g->x = x * GRID_SIZE;
			g->y = y * GRID_SIZE;

			g->texture = loadSprite(gdata[y][x]);
			stage.ground[y][x] = g;

			MapTile* w = malloc(sizeof(MapTile));
			memset(w, 0, sizeof(MapTile));

			w->texture = loadSprite(wdata[y][x]);
			stage.walls[y][x] = w;
		}
	}
	
	initPlayer();
}

static void logic(void)
{
	doPlayer();
	
	doBullets();
}

static void draw(void)
{
	drawGrid();

	drawBullets();
	
	drawPlayers();
	
	blit(targetterTexture, app.mouse.x, app.mouse.y, 1);
}

static void drawGrid(void)
{
	int x, y;
	
	for (y = 0 ; y < GRID_HEIGHT ; y += 1)
	{
		for (x = 0 ; x < GRID_WIDTH ; x += 1)
		{
			//printf("(%i, %i)\n", x, y);
			MapTile* m = stage.ground[y][x];
			blit(m->texture, m->x, m->y, 0);
		}
	}

	MapTile* m = stage.ground[0][18];
	blit(m->texture, 1200, 120, 0);
}
