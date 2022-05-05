/*
	Copyright 2022 SeungU Lyu
*/

#include "effect.h"

static SDL_Texture *tankTrailTexture;
static SDL_Texture *bulletTrailTexture1;
static SDL_Texture *bulletTrailTexture2;
static SDL_Texture *bulletTrailTexture3;
static SDL_Texture *bulletTrailTexture4;

/*
 * Function: initBullets
 * ----------------------------
 *  Load all the graphic resources for effects.
 */
void initEffect(void)
{
    tankTrailTexture = loadTexture("gfx/tracksSmall_light.png");
    bulletTrailTexture1 = loadTexture("gfx/smoke_01_resized.png");
    bulletTrailTexture2 = loadTexture("gfx/smoke_02_resized.png");
    bulletTrailTexture3 = loadTexture("gfx/smoke_05_resized.png");
    bulletTrailTexture4 = loadTexture("gfx/smoke_06_resized.png");
}

/*
 * Function: leaveTankTrailEffect
 * ----------------------------
 *  Leave trail behind the tank if the tank moved enough distance.
 *  This is done by creating a new Effect struct and attaching the pointer to Player's etrailTail.
 *  Also play sound whenever a new trail is created.
 */
void leaveTankTrailEffect(void)
{
    Player *p;
    for (p = stage.pHead.next; p != NULL ; p = p->next)
    {
        if (p->isBody != 1)
        {
            continue;
        }

        if (p->trailDistance <= 0)
        {
            p->trailDistance = PLAYER_TRAIL_DISTANCE;

            Effect *e = malloc(sizeof(Effect));
			memset(e, 0, sizeof(Effect));

            p->etrailTail->next = e;
	        p->etrailTail = e;
            
            e->texture = tankTrailTexture;
            e->x = p->x;
            e->y = p->y;
            e->angle = p->angle;
            e->health = FPS * 3;
            
            if (p->trailSound == 1)
            {
                if (p->playerIndex == 0)
                {
                    playSound(SND_TANK_MOVE, CH_PLAYER1_TANK_TRAIL);
                }
                else if (p->playerIndex == 1)
                {
                    playSound(SND_TANK_MOVE, CH_PLAYER2_TANK_TRAIL);
                }
                else if (p->playerIndex == 2)
                {
                    playSound(SND_TANK_MOVE, CH_PLAYER3_TANK_TRAIL);
                }
                else if (p->playerIndex == 3)
                {
                    playSound(SND_TANK_MOVE, CH_PLAYER4_TANK_TRAIL);
                }
                p->trailSound = 0;
            } 
            else
            {
                p->trailSound += 1;
            }
            
        }
    }
}

/*
 * Function: leaveTankTrailEffect
 * ----------------------------
 *  Leave trail behind the bullet if the bullet moved enough distance.
 *  This is done by creating a new Effect struct and attaching the pointer to Bullet's etrailTail.
 */
void leaveBulletTrailEffect(void)
{
    Player *p;
	
	for (p = stage.pHead.next ; p != NULL ; p = p->next)
	{	
		if (p->isBody == 1)
		{
			continue;
		}

        Bullet *b;
        for (b = p->bHead.next; b != NULL ; b = b->next)
        {
            if (b->trailDistance <= 0)
            {
                b->trailDistance = BULLET_TRAIL_DISTANCE;

                Effect *e = malloc(sizeof(Effect));
                memset(e, 0, sizeof(Effect));

                b->etrailTail->next = e;
                b->etrailTail = e;

                switch(rand() % 4)
                {
                    case 0:
                        e->texture = bulletTrailTexture1;
                        break;
                    case 1:
                        e->texture = bulletTrailTexture2;
                        break;
                    case 2:
                        e->texture = bulletTrailTexture3;
                        break;
                    case 3:
                        e->texture = bulletTrailTexture4;
                        break;
                }
                e->x = b->x - (BULLET_HEIGHT * 0.5 * sin((PI/180) * b->angle));
                e->y = b->y + (BULLET_HEIGHT * 0.5 * cos((PI/180) * b->angle));
                e->angle = rand() % 360;
                e->health = FPS * 1;
            }
        }
    }
}

/*
 * Function: updateBulletShotEffect
 * ----------------------------
 *  Update the position of players' bullet-shot-effect position and angles.
 */
void updateBulletShotEffect(void)
{
    Player *p;
    for (p = stage.pHead.next; p != NULL ; p = p->next)
    {
        if (p->isBody == 0 && p->bulletShot.health > 0)
        {
            p->bulletShot.x = p->x + (PLAYER_BARREL_HEIGHT * sin((PI/180) * p->angle));
            p->bulletShot.y = p->y - (PLAYER_BARREL_HEIGHT * cos((PI/180) * p->angle));
            p->bulletShot.angle = p->angle;
        }
    }
}

/*
 * Function: doEffect
 * ----------------------------
 *  Execute functions related to effects one by one.
 */
void doEffect(void)
{
    leaveTankTrailEffect();
    leaveBulletTrailEffect();
    updateBulletShotEffect();
}

/*
 * Function: drawEffectUnder
 * ----------------------------
 *  Draw all the effects that should be under the tank.
 *  This includes the tank trail.
 */
void drawEffectUnder(void)
{
    Player *p;
    for (p = stage.pHead.next; p != NULL ; p = p->next)
    {
        Effect *e;

        if (p->isBody == 1)
        {
            for (e = p->etrailHead.next; e != NULL ; e = e->next)
            {
                blitRotated(e->texture, e->x, e->y, e->angle);
            }
        }
    }
}

/*
 * Function: drawEffectOver
 * ----------------------------
 *  Draw all the effects that should be above the tank.
 *  This includes the bullet trail.
 *  The function also frees the bullet trails that is outdated.
 */
void drawEffectOver(void)
{
    Player *p;
	
	for (p = stage.pHead.next ; p != NULL ; p = p->next)
	{	
		if (p->isBody == 1)
		{
			continue;
		}

        if (p->bulletShot.health > 0)
        {
            blitRotated(p->bulletShot.texture, p->bulletShot.x, p->bulletShot.y, p->bulletShot.angle);
            p->bulletShot.health -= 1;
        }

        Bullet *b;
        for (b = p->bHead.next; b != NULL ; b = b->next)
        {
            Effect *e;
            Effect *prev = &(b->etrailHead);

            for (e = b->etrailHead.next; e != NULL ; e = e->next)
            {
                blitRotated(e->texture, e->x, e->y, e->angle);

                if (--e->health <= 0)
                {
                    if (e == b->etrailTail)
                    {
                        b->etrailTail = prev;
                    }

                    prev->next = e->next;
                    free(e);
                    e = prev;
                }
                
                prev = e;
            }
        }
    }
}