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

#include "bullets.h"

static SDL_Texture *bulletTexture;

void initBullets(void)
{
	bulletTexture = loadTexture("gfx/bulletBlue3_outline.png");
}

void fireBullet(void)
{
	Bullet *b;
	
	b = malloc(sizeof(Bullet));
	memset(b, 0, sizeof(Bullet));
	stage.bTail->next = b;
	stage.bTail = b;
	
	b->x = playerHead->x;
	b->y = playerHead->y;
	b->texture = bulletTexture;
	b->health = FPS * 2;
	b->angle = playerHead->angle;
	
	calcSlope(app.mouse.x, app.mouse.y, b->x, b->y, &b->dx, &b->dy);
	
	b->dx *= 8;
	b->dy *= 8;

	playerHead->reload = PLAYER_RELOAD;
	playerHead->ammo -= 1;
}

void doBullets(void)
{
	Bullet *b, *prev;
	
	prev = &stage.bHead;
	
	for (b = stage.bHead.next ; b != NULL ; b = b->next)
	{
		b->x += b->dx;
		b->y += b->dy;
		
		if (--b->health <= 0)
		{
			if (b == stage.bTail)
			{
				stage.bTail = prev;
			}
			
			playerHead->ammo += 1;
			prev->next = b->next;
			free(b);
			b = prev;
		}
		
		prev = b;
	}
}

void drawBullets(void)
{
	Bullet *b;
	
	for (b = stage.bHead.next ; b != NULL ; b = b->next)
	{
		blitRotated(b->texture, b->x, b->y, b->angle);
	}
}
