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
static SDL_Texture *gridTexture;

void initStage(void)
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;
	
	stage.entityTail = &stage.entityHead;
	stage.bulletTail = &stage.bulletHead;
	
	targetterTexture = loadTexture("gfx/crosshair010.png");
	gridTexture = loadTexture("gfx/tileGrass1.png");
	
	initPlayer();
}

static void logic(void)
{
	doPlayer();
	
	doEntities();
	
	doBullets();
}

static void draw(void)
{
	drawGrid();
	
	drawEntities();
	
	drawBullets();
	
	blit(targetterTexture, app.mouse.x, app.mouse.y, 1);
}

static void drawGrid(void)
{
	int x, y;
	
	for (y = 0 ; y < SCREEN_HEIGHT ; y += GRID_SIZE)
	{
		for (x = 0 ; x < SCREEN_WIDTH ; x += GRID_SIZE)
		{
			blit(gridTexture, x, y, 0);
		}
	}
}
