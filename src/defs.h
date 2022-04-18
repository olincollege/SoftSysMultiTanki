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

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'
#define PI                   3.14159265358979323846

#define FPS                 	60
#define WAIT                    1000/FPS                  
#define REMAINDER               (1000/(float)FPS) - WAIT

#define GRID_SIZE           	64
#define GRID_WIDTH				21
#define GRID_HEIGHT				15

#define SCREEN_WIDTH        	GRID_WIDTH * GRID_SIZE
#define SCREEN_HEIGHT       	GRID_HEIGHT * GRID_SIZE

#define MAX_KEYBOARD_KEYS   	350
#define MAX_MOUSE_BUTTONS   	6

#define MAX_NAME_LENGTH			32

#define MAX_SND_CHANNELS    	16

#define PLAYER_MAX_BULLET		5
#define PLAYER_RELOAD			10 * (FPS/(float)60)

#define PLAYER_SPEED        	2 * ((float)60/FPS)
#define PLAYER_ROTATION_SPEED 	3 * ((float)60/FPS)
#define PLAYER_TRAIL_DISTANCE	12 * (FPS/(float)60)
#define PLAYER_BARREL_HEIGHT    38

#define BULLET_SPEED			3 * ((float)60/FPS)
#define BULLET_HEIGHT			15
#define BULLET_MAX_HEALTH		3
#define BULLET_TRAIL_DISTANCE	18 * (FPS/(float)60)
