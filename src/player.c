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

#include "player.h"

void initPlayer(void)
{
	playerHead = malloc(sizeof(Player));
	memset(playerHead, 0, sizeof(Player));

	playerBody = malloc(sizeof(Player));
	memset(playerBody, 0, sizeof(Player));

	stage.pTail->next = playerBody;
	stage.pTail = playerBody;

	stage.pTail->next = playerHead;
	stage.pTail = playerHead;

	playerBody->texture = loadTexture("gfx/tankBody_blue_outline.png");
	playerBody->isBody = 1;
	playerBody->x = SCREEN_WIDTH / 2;
	playerBody->y = SCREEN_HEIGHT / 2;
	playerBody->angle = 70;
	playerBody->trailDistance = PLAYER_TRAIL_DISTANCE;
	playerBody->etrailTail = &playerBody->etrailHead;
	
	playerHead->texture = loadTexture("gfx/tankBlue_barrel2_outline.png");
	playerHead->x = playerBody->x;
	playerHead->y = playerBody->y;
	playerHead->ammo = PLAYER_MAX_BULLET;
	playerHead->bulletShot.texture = loadTexture("gfx/shotLarge.png");

	SDL_QueryTexture(playerBody->texture, NULL, NULL, &playerBody->w, &playerBody->h);
}

void doPlayer(void)
{
	int currentAngle; 
	
	currentAngle = playerBody -> angle;

	playerBody->dx *= 0.85;
	playerBody->dy *= 0.85;

	if (app.keyboard[SDL_SCANCODE_W])
	{
		if (currentAngle > 90 && currentAngle < 270)
		{
			if (currentAngle == 180)
			{

			}
			else if (currentAngle < 180)
			{
				currentAngle += PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle <= 180 ? currentAngle : 180;
			}
			else
			{
				currentAngle -= PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle >= 180 ? currentAngle : 180;
			}

			playerBody->dx = -(PLAYER_SPEED * sin((PI/180) * currentAngle));
			playerBody->dy = (PLAYER_SPEED * cos((PI/180) * currentAngle));
		}
		else
		{
			if (currentAngle == 0)
			{
				playerBody->dy = -PLAYER_SPEED;
				playerBody->dx = 0;
			}
			else if (currentAngle >= 270)
			{
				currentAngle += PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle < 360 ? currentAngle : 0;
			}
			else
			{
				currentAngle -= PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle >= 0 ? currentAngle : 0;
			}
			playerBody->dx = (PLAYER_SPEED * sin((PI/180) * currentAngle));
			playerBody->dy = -(PLAYER_SPEED * cos((PI/180) * currentAngle));
		}
	}
	
	if (app.keyboard[SDL_SCANCODE_S])
	{
		if (currentAngle >= 90 && currentAngle <= 270)
		{
			if (currentAngle == 180)
			{

			}
			else if (currentAngle < 180)
			{
				currentAngle += PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle <= 180 ? currentAngle : 180;
			}
			else
			{
				currentAngle -= PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle >= 180 ? currentAngle : 180;
			}
			playerBody->dx = (PLAYER_SPEED * sin((PI/180) * currentAngle));
			playerBody->dy = -(PLAYER_SPEED * cos((PI/180) * currentAngle));
		}
		else
		{
			if (currentAngle == 0)
			{

			}
			else if (currentAngle > 270)
			{
				currentAngle += PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle < 360 ? currentAngle : 0;
			}
			else
			{
				currentAngle -= PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle >= 0 ? currentAngle : 0;
			}
			playerBody->dx = -(PLAYER_SPEED * sin((PI/180) * currentAngle));
			playerBody->dy = (PLAYER_SPEED * cos((PI/180) * currentAngle));
		}
	}
	
	if (app.keyboard[SDL_SCANCODE_A])
	{
		if (currentAngle > 0 && currentAngle < 180)
		{
			if (currentAngle == 90)
			{

			}
			else if (currentAngle < 90)
			{
				currentAngle += PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle <= 90 ? currentAngle : 90;
			}
			else
			{
				currentAngle -= PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle >= 90 ? currentAngle : 90;
			}

			playerBody->dx = -(PLAYER_SPEED * sin((PI/180) * currentAngle));
			playerBody->dy = (PLAYER_SPEED * cos((PI/180) * currentAngle));
		}
		else
		{
			if (currentAngle == 0)
			{
				currentAngle = 360;
			}

			if (currentAngle == 270)
			{

			}
			else if (currentAngle > 270)
			{
				currentAngle -= PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle >= 270 ? currentAngle : 270;
			}
			else
			{
				currentAngle += PLAYER_ROTATION_SPEED;
				currentAngle = currentAngle <= 270 ? currentAngle : 270;
			}

			playerBody->dx = (PLAYER_SPEED * sin((PI/180) * currentAngle));
			playerBody->dy = -(PLAYER_SPEED * cos((PI/180) * currentAngle));
		}
	}
	
	if (app.keyboard[SDL_SCANCODE_D])
	{
		if (currentAngle >= 0 && currentAngle <= 180)
		{
			if (currentAngle == 90)
			{

			}
			else if (currentAngle < 90)
			{
				currentAngle += PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle <= 90 ? currentAngle : 90;
			}
			else
			{
				currentAngle -= PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle >= 90 ? currentAngle : 90;
			}

			playerBody->dx = (PLAYER_SPEED * sin((PI/180) * currentAngle));
			playerBody->dy = -(PLAYER_SPEED * cos((PI/180) * currentAngle));
		}
		else
		{
			if (currentAngle == 270)
			{

			}
			else if (currentAngle > 270)
			{
				currentAngle -= PLAYER_ROTATION_SPEED;
				currentAngle =  currentAngle >= 270 ? currentAngle : 270;
			}
			else
			{
				currentAngle += PLAYER_ROTATION_SPEED;
				currentAngle = currentAngle <= 270 ? currentAngle : 270;
			}

			playerBody->dx = -(PLAYER_SPEED * sin((PI/180) * currentAngle));
			playerBody->dy = (PLAYER_SPEED * cos((PI/180) * currentAngle));
		}
	}
	
	playerBody->angle = currentAngle;

	//printf("%d\n", playerBody->angle);
	
	playerHead->angle = getAngle(playerHead->x, playerHead->y, app.mouse.x, app.mouse.y);
	playerHead->x = playerBody->x;
	playerHead->y = playerBody->y;
	
	if (playerHead->reload == 0 && playerHead->ammo > 0 && app.mouse.button[SDL_BUTTON_LEFT])
	{
		fireBullet();
	}

	playerHead->reload = MAX(playerHead->reload - 1, 0);
	//printf("(%f, %f)\n", playerBody->x, playerBody->y);
}

void drawPlayers(void)
{
	Player *e;
	
	for (e = stage.pHead.next ; e != NULL ; e = e->next)
	{
		blitRotated(e->texture, e->x, e->y, e->angle);
	}
}
