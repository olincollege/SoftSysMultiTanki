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

#include "../common.h"

extern void blit(SDL_Texture *texture, int x, int y, int center);
extern void blitRotated(SDL_Texture *texture, int x, int y, float angle);
extern void doBullets(void);
extern void doEntities(void);
extern void doPlayer(void);
extern void drawBullets(void);
extern void drawEntities(void);
extern void initPlayer(void);
extern void drawPlayers(void);
extern void collisionWallsBullets(void);
extern void collisionPlayerWallsWithMove(void);
extern void collisionPlayerBullets(void);
extern void doEffect(void);
extern void drawEffectOver(void);
extern void drawEffectUnder(void);
extern void initGameOver(int playerIndex);
extern void playSound(int id, int channel);
extern void doAI(void);

extern SDL_Texture *loadSprite(int num);
extern SDL_Texture *loadTexture(char *filename);

extern struct MultiplayerInfo doMatchmaking(void);
extern int setupConnection(struct MultiplayerInfo);

extern App app;
extern Stage stage;

extern int gdata[GRID_HEIGHT][GRID_WIDTH];
extern int wdata[GRID_HEIGHT][GRID_WIDTH];
