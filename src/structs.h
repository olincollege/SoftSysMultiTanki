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

typedef struct Texture Texture;
typedef struct Player Player;
typedef struct Bullet Bullet;
typedef struct MapTile MapTile;
typedef struct Effect Effect;

typedef struct {
	void (*logic)(void);
	void (*draw)(void);
} Delegate;

struct Texture {
	char name[MAX_NAME_LENGTH];
	SDL_Texture *texture;
	Texture *next;
};

typedef struct {
	int x;
	int y;
	int button[MAX_MOUSE_BUTTONS];
	int wheel;
} Mouse;

struct Effect {
	float x;
	float y;
	int active;
	int maxHealth;
	int health;
	int angle;
	SDL_Texture *texture;
	Effect *next;
};

struct Player {
	int playerIndex;
	int isBody;
	float x;
	float y;
	int w;
	int h;
	int health;
	int reload;
	int ammo;
	float dx;
	float dy;
	int angle;
	Effect etrailHead, *etrailTail;
	Effect bulletShot;
	float trailDistance;
	SDL_Texture *texture;
	Player *next;
};

struct Bullet {
	int playerIndex;
	int active;
	float x;
	float y;
	int w;
	int h;
	int health;
	float dx;
	float dy;
	int angle;
	float bp[2];
	Effect etrailHead, *etrailTail;
	float trailDistance;
	SDL_Texture *texture;
	Bullet *next;
};

struct MapTile {
	float x;
	float y;
	SDL_Texture *texture;
	MapTile *next;
};

typedef struct {
	int keyboard[MAX_KEYBOARD_KEYS];
	Mouse mouse;
	int testInt;
} PlayerInput;

typedef struct {
	SDL_Renderer *renderer;
	SDL_Window *window;
	Delegate delegate;
	int maxPlayer;
	int playerIndex;
	PlayerInput playerInputs[MAX_PLAYER];
	Texture textureHead, *textureTail;
} App;

typedef struct {
	Player pHead, *pTail;
	Bullet bHead, *bTail;
	MapTile oHead, *oTail;
	MapTile* ground[GRID_HEIGHT][GRID_WIDTH];
} Stage;

