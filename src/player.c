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
	player = malloc(sizeof(Entity));
	memset(player, 0, sizeof(Entity));
	stage.entityTail->next = player;
	stage.entityTail = player;
	
	player->texture = loadTexture("gfx/donk.png");
	player->health = 5;
	player->x = SCREEN_WIDTH / 2;
	player->y = SCREEN_HEIGHT / 2;
	
	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

void doPlayer(void)
{
	player->dx *= 0.85;
	player->dy *= 0.85;
	
	if (app.keyboard[SDL_SCANCODE_W])
	{
		player->dy = -PLAYER_SPEED;
	}
	
	if (app.keyboard[SDL_SCANCODE_S])
	{
		player->dy = PLAYER_SPEED;
	}
	
	if (app.keyboard[SDL_SCANCODE_A])
	{
		player->dx = -PLAYER_SPEED;
	}
	
	if (app.keyboard[SDL_SCANCODE_D])
	{
		player->dx = PLAYER_SPEED;
	}
	
	player->angle = getAngle(player->x, player->y, app.mouse.x, app.mouse.y);
	
	if (player->reload == 0 && app.mouse.button[SDL_BUTTON_LEFT])
	{
		fireBullet();
	}

	player->reload = MAX(player->reload - 1, 0);
}
