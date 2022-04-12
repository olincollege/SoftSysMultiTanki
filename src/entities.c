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

#include "entities.h"

void doEntities(void)
{
	Entity *e;
	
	for (e = stage.entityHead.next ; e != NULL ; e = e->next)
	{
		e->x += e->dx;
		e->y += e->dy;
		
		if (e == player)
		{
			e->x = MIN(MAX(e->x, e->w / 2), SCREEN_WIDTH - e->w / 2);
			e->y = MIN(MAX(e->y, e->h / 2), SCREEN_HEIGHT - e->h / 2);
		}
	}
}

void drawEntities(void)
{
	Entity *e;
	
	for (e = stage.entityHead.next ; e != NULL ; e = e->next)
	{
		blitRotated(e->texture, e->x, e->y, e->angle);
	}
}
