/*
	Copyright 2022 SeungU Lyu
*/

#include "bullets.h"

static SDL_Texture *bulletTexture[4];
static SDL_Texture *testBullet;
static SDL_Texture *bulletExplosion[5];

/*
 * Function: initBullets
 * ----------------------------
 *  Load all the graphic resources for bullet.
 */
void initBullets(void)
{
	bulletTexture[0] = loadTexture("gfx/bulletBlue3_outline.png");
	bulletTexture[1] = loadTexture("gfx/bulletRed3_outline.png");
	bulletTexture[2] = loadTexture("gfx/bulletGreen3_outline.png");
	bulletTexture[3] = loadTexture("gfx/bulletSand3_outline.png");

	testBullet = loadTexture("gfx/crosshair001.png");

	bulletExplosion[0] = loadTexture("gfx/explosionSmoke1.png");
	bulletExplosion[1] = loadTexture("gfx/explosionSmoke2.png");
	bulletExplosion[2] = loadTexture("gfx/explosionSmoke3.png");
	bulletExplosion[3] = loadTexture("gfx/explosionSmoke4.png");
	bulletExplosion[4] = loadTexture("gfx/explosionSmoke5.png");
}

/*
 * Function: fireBullet
 * ----------------------------
 *  Fires a bullet from a player. 
 *  New bullet will replace the bTail of the player.
 *
 *  playerHead: pointer to the Player who is shooting the bullet
 */
void fireBullet(Player* playerHead)
{
	Bullet *b;
	
	b = malloc(sizeof(Bullet));
	memset(b, 0, sizeof(Bullet));

	playerHead->bTail->next = b;
	playerHead->bTail = b;
	
	b->x = playerHead->x + (PLAYER_BARREL_HEIGHT * 0.8 * sin((PI/180) * playerHead->angle));
	b->y = playerHead->y - (PLAYER_BARREL_HEIGHT * 0.8 * cos((PI/180) * playerHead->angle));
	switch(playerHead->playerIndex)
	{
		case 0:
			b->texture = bulletTexture[0];
			break;
		case 1:
			b->texture = bulletTexture[1];
			break;
		case 2:
			b->texture = bulletTexture[2];
			break;
		case 3:
			b->texture = bulletTexture[3];
			break;
		default:
			b->texture = bulletTexture[0];
			break;

	}

	b->health = BULLET_MAX_HEALTH;
	b->angle = playerHead->angle;
	b->etrailTail = &b->etrailHead;
	b->trailDistance = BULLET_TRAIL_DISTANCE * 3;
	b->active = 1;
	b->playerIndex = playerHead->playerIndex;

	calcSlope(app.playerInputs[playerHead->playerIndex].mouse.x, app.playerInputs[playerHead->playerIndex].mouse.y, b->x, b->y, &b->dx, &b->dy);
	
	b->dx *= BULLET_SPEED;
	b->dy *= BULLET_SPEED;

	playerHead->reload = PLAYER_RELOAD;
	playerHead->ammo -= 1;
	playerHead->bulletShot.health = PLAYER_RELOAD;
}

/*
 * Function: doBullets
 * ----------------------------
 *  Calculates bullets next position in the frame.
 *  Checks if the bullet has 0 health, and if it does make it inactive.
 *  If inactive bullet has no trail left, destroy the bullet and free the memory.
 */
void doBullets(void)
{
	Player *p;
	
	for (p = stage.pHead.next ; p != NULL ; p = p->next)
	{	
		if (p->isBody == 1)
		{
			continue;
		}

		Bullet *b, *prev;
		
		prev = &p->bHead;
		
		for (b = p->bHead.next ; b != NULL ; b = b->next)
		{
			if (b->active == 0)
			{
				if (&b->etrailHead == b->etrailTail && b->isDead <= 0)
				{
					if (b == p->bTail)
					{
						p->bTail = prev;
					}
					
					prev->next = b->next;
					free(b);
					b = prev;
					//printf("check\n");
					continue;
				}
				else
				{
					b->isDead -= 1;
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
				b->isDead = BULLET_EXPLOSION_TIME;
				p->ammo += 1;
			}
			
			prev = b;
		}
	}
}

/*
 * Function: drawBullets
 * ----------------------------
 *  Draw all the bullets available in the game.
 *  Draw the bullet texture when it is active, else draw the explosion texture.
 */
void drawBullets(void)
{
	Player *p;
	
	for (p = stage.pHead.next ; p != NULL ; p = p->next)
	{	
		if (p->isBody == 1)
		{
			continue;
		}
		Bullet *b;
		
		for (b = p->bHead.next ; b != NULL ; b = b->next)
		{
			if (b->active == 1)
			{
				blitRotated(b->texture, b->x, b->y, b->angle);
			}
			else if (b->isDead > BULLET_EXPLOSION_TIME * 0.8)
			{
				blitRotated(bulletExplosion[0], b->x, b->y, b->angle);
			}
			else if (b->isDead > BULLET_EXPLOSION_TIME * 0.6)
			{
				blitRotated(bulletExplosion[1], b->x, b->y, b->angle);
			}
			else if (b->isDead > BULLET_EXPLOSION_TIME * 0.4)
			{
				blitRotated(bulletExplosion[2], b->x, b->y, b->angle);
			}
			else if (b->isDead > BULLET_EXPLOSION_TIME * 0.2)
			{
				blitRotated(bulletExplosion[3], b->x, b->y, b->angle);
			}
			else if (b->isDead > 0)
			{
				blitRotated(bulletExplosion[4], b->x, b->y, b->angle);
			}
			//blitRotated(testBullet, b->bp[0], b->bp[1], b->angle);
			// blitRotated(testBullet, b->x, b->y, b->angle);
		}
	}
}
