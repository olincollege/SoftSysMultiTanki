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

#include "main.h"

static void capFrameRate(long *then, float *remainder);

int main(int argc, char *argv[])
{
	long then;
	float remainder;
	
	memset(&app, 0, sizeof(App));
	app.textureTail = &app.textureHead;

	app.maxPlayer = MAX_PLAYER;

	initSDL();
	
	atexit(cleanup);
	
	initGame();

	initTitle();

	//initStage();
	
	then = SDL_GetTicks();
	
	remainder = 0;

	SDL_Event e;
	while (SDL_PollEvent(&e));

	while (1)
	{
		prepareScene();
		
		doInput();		
		
		app.delegate.logic();
		
		app.delegate.draw();
		
		presentScene();

		capFrameRate(&then, &remainder);
	}

	return 0;
}

static void capFrameRate(long *then, float *remainder)
{
	long wait, frameTime;
	
	wait = WAIT + *remainder;
	
	*remainder -= (int)*remainder;
	
	frameTime = SDL_GetTicks() - *then;
	
	wait -= frameTime;
	
	if (wait < 1)
	{
		wait = 1;
	}
		
	SDL_Delay(wait);
	
	*remainder += REMAINDER;
	
	*then = SDL_GetTicks();
}
