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

#include "input.h"

void doKeyUp(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		app.playerInputs[app.playerIndex].keyboard[event->keysym.scancode] = 0;
	}
}

void doKeyDown(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		app.playerInputs[app.playerIndex].keyboard[event->keysym.scancode] = 1;
	}
}

void doMouseButtonUp(SDL_MouseButtonEvent *event)
{
	app.playerInputs[app.playerIndex].mouse.button[event->button] = 0;
}

void doMouseButtonDown(SDL_MouseButtonEvent *event)
{
	app.playerInputs[app.playerIndex].mouse.button[event->button] = 1;
}

void doInput(void)
{
	SDL_Event event;
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;
				
			case SDL_KEYDOWN:
				doKeyDown(&event.key);
				break;
				
			case SDL_KEYUP:
				doKeyUp(&event.key);
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				doMouseButtonDown(&event.button);
				break;
				
			case SDL_MOUSEBUTTONUP:
				doMouseButtonUp(&event.button);
				break;
				
			case SDL_MOUSEWHEEL:
				app.playerInputs[app.playerIndex].mouse.wheel = event.wheel.y;
				break;

			default:
				break;
		}
	}
	
	SDL_GetMouseState(&app.playerInputs[app.playerIndex].mouse.x, &app.playerInputs[app.playerIndex].mouse.y);
}
