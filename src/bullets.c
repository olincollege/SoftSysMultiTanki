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
static SDL_Texture *testBullet;

void initBullets(void)
{
	bulletTexture = loadTexture("gfx/bulletBlue3_outline.png");
	testBullet = loadTexture("gfx/crosshair001.png");
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
	b->health = BULLET_MAX_HEALTH;
	b->angle = playerHead->angle;
	b->etrailTail = &b->etrailHead;
	b->trailDistance = BULLET_TRAIL_DISTANCE * 3;
	b->active = 1;
	
	calcSlope(app.mouse.x, app.mouse.y, b->x, b->y, &b->dx, &b->dy);
	
	b->dx *= BULLET_SPEED;
	b->dy *= BULLET_SPEED;

	playerHead->reload = PLAYER_RELOAD;
	playerHead->ammo -= 1;
	playerHead->bulletShot.health = PLAYER_RELOAD;
}

void doBullets(void)
{
	Bullet *b, *prev;
	
	prev = &stage.bHead;
	
	for (b = stage.bHead.next ; b != NULL ; b = b->next)
	{
		if (b->active == 0)
		{
			if (&b->etrailHead == b->etrailTail)
			{
				if (b == stage.bTail)
				{
					stage.bTail = prev;
				}
				
				prev->next = b->next;
				free(b);
				b = prev;
				//printf("check\n");
				continue;
			}
			else
			{
				continue;
			}
		}

		float startX = b->x;
        float startY = b->y;

		b->x += b->dx;
		b->y += b->dy;

		b->bp[0] = b->x + (BULLET_HEIGHT * 0.5 * sin((PI/180) * b->angle));
		b->bp[1] = b->y - (BULLET_HEIGHT * 0.5 * cos((PI/180) * b->angle));

		float travelDistance = sqrtf(powf(b->x - startX, 2) + powf(b->y - startY, 2));

        b->trailDistance -= travelDistance;
		
		if (b->health <= 0)
		{
			b->active = 0;
			playerHead->ammo += 1;
		}
		
		prev = b;
	}
}

void drawBullets(void)
{
	Bullet *b;
	
	for (b = stage.bHead.next ; b != NULL ; b = b->next)
	{
		if (b->active == 1)
		{
			blitRotated(b->texture, b->x, b->y, b->angle);
		}
		//blitRotated(testBullet, b->bp[0], b->bp[1], b->angle);
		// blitRotated(testBullet, b->x, b->y, b->angle);
	}
}
