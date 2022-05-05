/*
	Copyright 2022 SeungU Lyu
*/

#include "collision.h"

/*
 * Function: bulletSound
 * ----------------------------
 *  Play sound when the bullet hits the wall.
 *  Plays differnt sound according to whether the bullet is reflecting or exploding.
 *
 *  p: pointer to the Player who is shooting the bullet
 *  b: pointer to the Bullet which is hitting the wall
 */
void bulletSound(Player* p, Bullet* b)
{
    if (b->active == 0)
    {

    }
    else if (p->playerIndex == 0)
    {
        if (b->health == 0)
        {
            playSound(SND_BULLET_EXPLOSION, CH_PLAYER1_BULLET);
        }
        else
        {
            playSound(SND_BULLET_REFLECT, CH_PLAYER1_BULLET);
        }
    }
    else if (p->playerIndex == 1)
    {
        if (b->health == 0)
        {
            playSound(SND_BULLET_EXPLOSION, CH_PLAYER2_BULLET);
        }
        else
        {
            playSound(SND_BULLET_REFLECT, CH_PLAYER2_BULLET);
        }
    }
    else if (p->playerIndex == 2)
    {
        if (b->health == 0)
        {
            playSound(SND_BULLET_EXPLOSION, CH_PLAYER3_BULLET);
        }
        else
        {
            playSound(SND_BULLET_REFLECT, CH_PLAYER3_BULLET);
        }
    }
    else if (p->playerIndex == 3)
    {
        if (b->health == 0)
        {
            playSound(SND_BULLET_EXPLOSION, CH_PLAYER4_BULLET);
        }
        else
        {
            playSound(SND_BULLET_REFLECT, CH_PLAYER4_BULLET);
        }
    }
}

/*
 * Function: collisionWallsBullets
 * ----------------------------
 *  Checks bullet collision with walls.
 *  The way it checks collision is by creating line connecting bullet's center and the tip of the head
 *  and check if that intersect with each sides of the wall.
 *  This enables us to know which axis the bullet hit the wall and make it easier to calculate the angle of reflection.
 *  To prevent bug, if the bullet's center is inside the wall (penetrated through the wall) then the bullet's health is set to 0.
 */
void collisionWallsBullets(void)
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
            MapTile *m;
            for (m = stage.oHead.next ; m != NULL ; m = m->next)
            {
                float cp[2][2];
                cp[0][0] = m->x;
                cp[0][1] = m->y;
                cp[1][0] = m->x + GRID_SIZE;
                cp[1][1] = m->y + GRID_SIZE;

                if (getPointInsideRect(cp[0][0], cp[0][1], cp[1][0], cp[1][1], b->x, b->y) == 1)
                {
                    b->health = 0;
                    //bulletSound(p,b);
                    break;
                }

                cp[0][0] = m->x;
                cp[0][1] = m->y;
                cp[1][0] = m->x + GRID_SIZE;
                cp[1][1] = m->y;

                // blit(b->texture, cp[0][0], cp[0][1], 0);
                // blit(b->texture, cp[1][0], cp[1][1], 0);

                if (getLineIntersection(b->x, b->y, b->bp[0], b->bp[1], cp[0][0], cp[0][1], cp[1][0], cp[1][1], NULL, NULL))
                {
                    b->angle = getReflectedAngle(b->angle, 0);
                    b->dy = -b->dy;
                    b->health -= 1;
                    bulletSound(p,b);
                    break;
                }

                cp[0][0] = m->x;
                cp[0][1] = m->y + GRID_SIZE;
                cp[1][0] = m->x + GRID_SIZE;
                cp[1][1] = m->y + GRID_SIZE;

                if (getLineIntersection(b->x, b->y, b->bp[0], b->bp[1], cp[0][0], cp[0][1], cp[1][0], cp[1][1], NULL, NULL))
                {
                    b->angle = getReflectedAngle(b->angle, 0);
                    b->dy = -b->dy;
                    b->health -= 1;
                    bulletSound(p,b);
                    break;
                }

                cp[0][0] = m->x;
                cp[0][1] = m->y;
                cp[1][0] = m->x;
                cp[1][1] = m->y + GRID_SIZE;

                if (getLineIntersection(b->x, b->y, b->bp[0], b->bp[1], cp[0][0], cp[0][1], cp[1][0], cp[1][1], NULL, NULL))
                {
                    b->angle = getReflectedAngle(b->angle, 1);
                    b->dx = -b->dx;
                    b->health -= 1;
                    bulletSound(p,b);
                    break;
                }

                cp[0][0] = m->x + GRID_SIZE;
                cp[0][1] = m->y;
                cp[1][0] = m->x + GRID_SIZE;
                cp[1][1] = m->y + GRID_SIZE;

                if (getLineIntersection(b->x, b->y, b->bp[0], b->bp[1], cp[0][0], cp[0][1], cp[1][0], cp[1][1], NULL, NULL))
                {
                    b->angle = getReflectedAngle(b->angle, 1);
                    b->dx = -b->dx;
                    b->health -= 1;
                    bulletSound(p,b);
                    break;
                }
            }
        }
    }
}

/*
 * Function: collisionWallsBullets
 * ----------------------------
 *  Checks Player collision with walls.
 *  If player's next move overlaps with any of the wall, undo the move.
 */
void collisionPlayerWallsWithMove(void)
{
    Player *p;
    for (p = stage.pHead.next ; p != NULL ; p = p->next)
	{
        if (p->isBody != 1)
        {
            continue;
        }
        
        float startX = p->x;
        float startY = p->y;

		p->x += p->dx;

        MapTile* m;
        for (m = stage.oHead.next ; m != NULL ; m = m->next)
        {
            if (getRectOverlap(m->x, m->y, m->x + GRID_SIZE, m->y + GRID_SIZE, p->x - p->w/2, p->y - p->h/2, p->x + p->w/2, p->y + p->h/2))
            {
                p->x = p->dx > 0 ? m->x - p->w/2 : m->x + GRID_SIZE + p->w/2;
                break;
            }
        }
		p->y += p->dy;

        for (m = stage.oHead.next ; m != NULL ; m = m->next)
        {
            if (getRectOverlap(m->x, m->y, m->x + GRID_SIZE, m->y + GRID_SIZE, p->x - p->w/2, p->y - p->h/2, p->x + p->w/2, p->y + p->h/2))
            {
                p->y = p->dy > 0 ? m->y - p->h/2 : m->y + GRID_SIZE + p->h/2;
                break;
            }
        }

        float travelDistance = sqrtf(powf(p->x - startX, 2) + powf(p->y - startY, 2));

        p->trailDistance -= travelDistance;
		
		p->x = MIN(MAX(p->x, p->w / 2), SCREEN_WIDTH - p->w / 2);
		p->y = MIN(MAX(p->y, p->h / 2), SCREEN_HEIGHT - p->h / 2);
	}
}

/*
 * Function: collisionWallsBullets
 * ----------------------------
 *  Checks Player collision with bullets.
 *  If the bullet's center is inside the player's rectangle, set bullet health to 0 and trigger Player's isDead variable.
 */
void collisionPlayerBullets(void)
{
    Player *p;
    for (p = stage.pHead.next ; p != NULL ; p = p->next)
	{
        if (p->isBody != 1)
        {
            continue;
        }
        else if (p->isDead > 0)
        {
            continue;
        }
        
        Player *e;
        for (e = stage.pHead.next ; e != NULL ; e = e->next)
        {
            if (e->isBody == 1)
            {
                continue;
            }
            
            Bullet *b;

            for (b = e->bHead.next ; b != NULL ; b = b->next)
            {
                if (getPointInsideRect(p->x - p->w/2, p->y - p->h/2, p->x + p->w/2, p->y + p->h/2, b->x, b->y) == 1)
                {
                    b->health = 0;
                    p->isDead = PLAYER_EXPLOSION_TIME;
                }
            }
        }
	}
}