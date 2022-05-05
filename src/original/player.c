/*
	Copyright 2022 SeungU Lyu
*/

#include "player.h"

static SDL_Texture *playerExplosion[5];

/*
 * Function: initPlayer
 * ----------------------------
 *  Initialize players by loading necessary textures, allocating memory, and assigning default values.
 */
void initPlayer(void)
{
	playerExplosion[0] = loadTexture("gfx/explosion.png");
	playerExplosion[1] = loadTexture("gfx/explosion2.png");
	playerExplosion[2] = loadTexture("gfx/explosion3.png");
	playerExplosion[3] = loadTexture("gfx/explosion4.png");
	playerExplosion[4] = loadTexture("gfx/explosion5.png");
	
	for (int i = 0; i < app.maxPlayer; i++)
	{
		Player *playerHead;
		Player *playerBody;
		playerHead = malloc(sizeof(Player));
		memset(playerHead, 0, sizeof(Player));

		playerBody = malloc(sizeof(Player));
		memset(playerBody, 0, sizeof(Player));

		app.playerInputs[i].mouse.x = SCREEN_WIDTH/2;
        app.playerInputs[i].mouse.y = SCREEN_HEIGHT/2;

		stage.pTail->next = playerBody;
		stage.pTail = playerBody;

		stage.pTail->next = playerHead;
		stage.pTail = playerHead;

		playerHead->playerIndex = i;
		playerBody->playerIndex = i;

		playerHead->bTail = &playerHead->bHead;

		switch(i)
		{
			case 0:
				playerBody->texture = loadTexture("gfx/tankBody_blue_outline.png");
				playerHead->texture = loadTexture("gfx/tankBlue_barrel2_outline.png");
				playerBody->x = 4 * GRID_SIZE;
				playerBody->y = 4 * GRID_SIZE;
				break;
			case 1:
				playerBody->texture = loadTexture("gfx/tankBody_red_outline.png");
				playerHead->texture = loadTexture("gfx/tankRed_barrel2_outline.png");
				playerBody->x = 17 * GRID_SIZE;
				playerBody->y = 4 * GRID_SIZE;
				break;
			case 2:
				playerBody->texture = loadTexture("gfx/tankBody_green_outline.png");
				playerHead->texture = loadTexture("gfx/tankGreen_barrel2_outline.png");
				playerBody->x = 4 * GRID_SIZE;
				playerBody->y = 11 * GRID_SIZE;
				break;
			case 3:
				playerBody->texture = loadTexture("gfx/tankBody_sand_outline.png");
				playerHead->texture = loadTexture("gfx/tankSand_barrel2_outline.png");
				playerBody->x = 17 * GRID_SIZE;
				playerBody->y = 11 * GRID_SIZE;
				break;
			default:
				playerBody->texture = loadTexture("gfx/tankBody_sand_outline.png");
				playerHead->texture = loadTexture("gfx/tankSand_barrel2_outline.png");
				playerBody->x = SCREEN_WIDTH / 2;
				playerBody->y = SCREEN_HEIGHT / 2;
				break;
		}
		
		playerBody->isBody = 1;
		playerBody->angle = 0;
		playerBody->trailDistance = PLAYER_TRAIL_DISTANCE;
		playerBody->etrailTail = &playerBody->etrailHead;
		playerBody->health = PLAYER_MAX_HEALTH;
		
		playerHead->x = playerBody->x;
		playerHead->y = playerBody->y;
		playerHead->ammo = PLAYER_MAX_BULLET;
		playerHead->bulletShot.texture = loadTexture("gfx/shotLarge.png");

		SDL_QueryTexture(playerBody->texture, NULL, NULL, &playerBody->w, &playerBody->h);
	}
}

/*
 * Function: doPlayer
 * ----------------------------
 *  Calculates player movement from user input and checks if bullet is firing.
 *  Also plays player related sounds.
 */
void doPlayer(void)
{
	Player *playerHead;
	Player *playerBody;
	int currentAngle;

	Player *p;
	
	for (p = stage.pHead.next ; p != NULL ; p = p->next)
	{
		if (p->isBody == 1)
		{
			playerBody = p;
			playerHead = p->next;
		}
		else
		{
			continue;
		}
		
		if (p->isDead == PLAYER_EXPLOSION_TIME)
		{
			if (playerBody->playerIndex == 0)
			{
				playSound(SND_TANK_EXPLOSION, CH_PLAYER1_TANK);
			}
			else if (playerBody->playerIndex == 1)
			{
				playSound(SND_TANK_EXPLOSION, CH_PLAYER2_TANK);
			}
			else if (playerBody->playerIndex == 2)
			{
				playSound(SND_TANK_EXPLOSION, CH_PLAYER3_TANK);
			}
			else if (playerBody->playerIndex == 3)
			{
				playSound(SND_TANK_EXPLOSION, CH_PLAYER4_TANK);
			}
		}

		if (p->isDead == PLAYER_EXPLOSION_TIME * 0.65 && p->health == 0)
		{
			continue;
		}
		else if (p->isDead == PLAYER_EXPLOSION_TIME * 0.65)
		{
			int x = 0;
			int y = 0;

			if (p->health == 1)
			{
				playerBody->health -= 1;
				continue;
			}
			else 
			{
				getEmptyPosition(&x, &y);
				playerBody->isDead -= 1;
				playerBody->x = x * GRID_SIZE;
				playerBody->y = y * GRID_SIZE;
				playerBody->angle = 0;
				playerHead->x = playerBody->x;
				playerHead->y = playerBody->y;
				playerBody->health -= 1;
			}
			
			continue;
		}
		else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.6)
		{
			playerBody->dx = 0;
			playerBody->dy = 0;
			playerBody->isDead -= 1;
			continue;
		}
		else if (p->isDead > 0)
		{
			playerBody->isDead -= 1;
		}
		
		currentAngle = playerBody -> angle;

		playerBody->dx *= 0.85;
		playerBody->dy *= 0.85;

		if (app.playerInputs[playerBody->playerIndex].keyboard[SDL_SCANCODE_W])
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
		
		if (app.playerInputs[playerBody->playerIndex].keyboard[SDL_SCANCODE_S])
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
		
		if (app.playerInputs[playerBody->playerIndex].keyboard[SDL_SCANCODE_A])
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
		
		if (app.playerInputs[playerBody->playerIndex].keyboard[SDL_SCANCODE_D])
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

		playerHead->angle = getAngle(playerHead->x, playerHead->y, app.playerInputs[playerBody->playerIndex].mouse.x, app.playerInputs[playerBody->playerIndex].mouse.y);
		playerHead->x = playerBody->x;
		playerHead->y = playerBody->y;
		
		if (playerHead->reload == 0 && playerHead->ammo > 0 && app.playerInputs[playerBody->playerIndex].mouse.button[SDL_BUTTON_LEFT] && playerBody->isDead <= 0)
		{
			if (playerHead->playerIndex == 0)
			{
				playSound(SND_BULLET_FIRE, CH_PLAYER1_TANK);
			}
			else if (playerHead->playerIndex == 1)
			{
				playSound(SND_BULLET_FIRE, CH_PLAYER2_TANK);
			}
			else if (playerHead->playerIndex == 2)
			{
				playSound(SND_BULLET_FIRE, CH_PLAYER3_TANK);
			}
			else if (playerHead->playerIndex == 3)
			{
				playSound(SND_BULLET_FIRE, CH_PLAYER4_TANK);
			}
			fireBullet(playerHead);
		}

		playerHead->reload = MAX(playerHead->reload - 1, 0);
	}
}

/*
 * Function: drawPlayers
 * ----------------------------
 *  Draws graphics related to the players. 
 *  Tank texure will be displayed unless the Player is dead.
 *  If player is dead, shows explosion textures over time.
 */
void drawPlayers(void)
{
	Player *p;
	
	for (p = stage.pHead.next ; p != NULL ; p = p->next)
	{
		if (p->isBody == 1)
		{
			if (p->isDead > PLAYER_EXPLOSION_TIME * 0.97)
			{
				blitRotated(p->texture, p->x, p->y, p->angle);
				blitRotated(p->next->texture, p->next->x, p->next->y, p->next->angle);
				blitRotated(playerExplosion[0], p->x, p->y, p->angle);
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.94)
			{
				blitRotated(p->texture, p->x, p->y, p->angle);
				blitRotated(p->next->texture, p->next->x, p->next->y, p->next->angle);
				blitRotated(playerExplosion[1], p->x, p->y, p->angle);
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.91)
			{
				blitRotated(p->texture, p->x, p->y, p->angle);
				blitRotated(p->next->texture, p->next->x, p->next->y, p->next->angle);
				blitRotated(playerExplosion[2], p->x, p->y, p->angle);
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.88)
			{
				blitRotated(playerExplosion[3], p->x, p->y, p->angle);
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.85)
			{
				blitRotated(playerExplosion[4], p->x, p->y, p->angle);
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.6)
			{
				
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.55)
			{
				blitRotated(p->texture, p->x, p->y, p->angle);
				blitRotated(p->next->texture, p->next->x, p->next->y, p->next->angle);
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.5)
			{
				
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.45)
			{
				blitRotated(p->texture, p->x, p->y, p->angle);
				blitRotated(p->next->texture, p->next->x, p->next->y, p->next->angle);
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.4)
			{
				
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.35)
			{
				blitRotated(p->texture, p->x, p->y, p->angle);
				blitRotated(p->next->texture, p->next->x, p->next->y, p->next->angle);
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.30)
			{
				
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.25)
			{
				blitRotated(p->texture, p->x, p->y, p->angle);
				blitRotated(p->next->texture, p->next->x, p->next->y, p->next->angle);
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.2)
			{
				
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.15)
			{
				blitRotated(p->texture, p->x, p->y, p->angle);
				blitRotated(p->next->texture, p->next->x, p->next->y, p->next->angle);
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.1)
			{
				
			}
			else if (p->isDead > PLAYER_EXPLOSION_TIME * 0.05)
			{
				blitRotated(p->texture, p->x, p->y, p->angle);
				blitRotated(p->next->texture, p->next->x, p->next->y, p->next->angle);
			}
			else if (p->isDead > 0)
			{
				
			}
			else
			{
				blitRotated(p->texture, p->x, p->y, p->angle);
				blitRotated(p->next->texture, p->next->x, p->next->y, p->next->angle);
			}
		}
	}
}
