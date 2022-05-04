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

struct MultiplayerInfo peer_info;

void *send_to_serv(void *threadid){
	long tid;
	tid = (long)threadid;
	printf("thread #%ld!\n", tid);
	doSendToServer(peer_info);
	pthread_exit(NULL);
}
void *recv_from_serv(void *threadid){
	long tid;
	tid = (long)threadid;
	printf("thread #%ld!\n", tid);
	doReceiveFromServer(peer_info);
	pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
	long then;
	float remainder;

	pthread_t threads[2];
	int rc;
	long t;
	
	memset(&app, 0, sizeof(App));
	app.textureTail = &app.textureHead;

	app.maxPlayer = MAX_PLAYER;

	// connect to matchmaking server
	// get peer ip address and current tank number
	peer_info = doMatchmaking();
	// create connection between peers
	peer_info = setupConnection(peer_info);

	app.playerIndex = peer_info.tank_no;
	
	initSDL();
	
	atexit(cleanup);
	
	initGame();

	initTitle();

	//initStage();
	
	then = SDL_GetTicks();
	
	remainder = 0;

	SDL_Event e;
	while (SDL_PollEvent(&e));

	t=0;
	printf("In main: creating thread %ld\n", t);
	rc = pthread_create(&threads[1], NULL, send_to_serv, (void *)t);
	if (rc){
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}
	t=1;
	printf("In main: creating thread %ld\n", t);
	rc = pthread_create(&threads[2], NULL, recv_from_serv, (void *)t);
	if (rc){
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}

	while (1)
	{
		prepareScene();
		
		doInput();		
		
		app.delegate.logic();
		
		app.delegate.draw();
		
		presentScene();
		
		/* utilize pthread
		if (app.playerIndex == 0) {
			doReceiveFromServer(peer_info);
			doSendToServer(peer_info);
		}
		if (app.playerIndex == 1) {
			doSendToServer(peer_info);
			doReceiveFromServer(peer_info);
		}*/

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
