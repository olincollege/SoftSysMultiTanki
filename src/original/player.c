#include "player.h"

void initPlayer(void)
{
	for (int i = 0; i < app.maxPlayer; i++)
	{
		Player *playerHead;
		Player *playerBody;
		playerHead = malloc(sizeof(Player));
		memset(playerHead, 0, sizeof(Player));

		playerBody = malloc(sizeof(Player));
		memset(playerBody, 0, sizeof(Player));

		stage.pTail->next = playerBody;
		stage.pTail = playerBody;

		stage.pTail->next = playerHead;
		stage.pTail = playerHead;

		playerHead->playerIndex = i;
		playerBody->playerIndex = i;

		switch(i)
		{
			case 0:
				playerBody->texture = loadTexture("gfx/tankBody_blue_outline.png");
				playerHead->texture = loadTexture("gfx/tankBlue_barrel2_outline.png");
				playerBody->x = 5 * GRID_SIZE;
				playerBody->y = 5 * GRID_SIZE;
				break;
			case 1:
				playerBody->texture = loadTexture("gfx/tankBody_red_outline.png");
				playerHead->texture = loadTexture("gfx/tankRed_barrel2_outline.png");
				playerBody->x = 15 * GRID_SIZE;
				playerBody->y = 5 * GRID_SIZE;
				break;
			default:
				playerBody->texture = loadTexture("gfx/tankBody_blue_outline.png");
				playerHead->texture = loadTexture("gfx/tankBlue_barrel2_outline.png");
				playerBody->x = SCREEN_WIDTH / 2;
				playerBody->y = SCREEN_HEIGHT / 2;
				break;
		}
		
		playerBody->isBody = 1;
		playerBody->angle = 0;
		playerBody->trailDistance = PLAYER_TRAIL_DISTANCE;
		playerBody->etrailTail = &playerBody->etrailHead;
		
		playerHead->x = playerBody->x;
		playerHead->y = playerBody->y;
		playerHead->ammo = PLAYER_MAX_BULLET;
		playerHead->bulletShot.texture = loadTexture("gfx/shotLarge.png");

		SDL_QueryTexture(playerBody->texture, NULL, NULL, &playerBody->w, &playerBody->h);
	}
	
}

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

		//printf("%d\n", playerBody->angle);
		
		playerHead->angle = getAngle(playerHead->x, playerHead->y, app.playerInputs[playerBody->playerIndex].mouse.x, app.playerInputs[playerBody->playerIndex].mouse.y);
		playerHead->x = playerBody->x;
		playerHead->y = playerBody->y;
		
		if (playerHead->reload == 0 && playerHead->ammo > 0 && app.playerInputs[playerBody->playerIndex].mouse.button[SDL_BUTTON_LEFT])
		{
			fireBullet(playerHead);
		}

		playerHead->reload = MAX(playerHead->reload - 1, 0);
		//printf("(%f, %f)\n", playerBody->x, playerBody->y);
	}
}

void drawPlayers(void)
{
	Player *e;
	
	for (e = stage.pHead.next ; e != NULL ; e = e->next)
	{
		blitRotated(e->texture, e->x, e->y, e->angle);
	}
}
