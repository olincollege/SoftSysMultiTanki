#include "effect.h"

static SDL_Texture *tankTrailTexture;
static SDL_Texture *bulletTrailTexture1;
static SDL_Texture *bulletTrailTexture2;
static SDL_Texture *bulletTrailTexture3;
static SDL_Texture *bulletTrailTexture4;

void initEffect(void)
{
    tankTrailTexture = loadTexture("gfx/tracksSmall_light.png");
    bulletTrailTexture1 = loadTexture("gfx/smoke_01_resized.png");
    bulletTrailTexture2 = loadTexture("gfx/smoke_02_resized.png");
    bulletTrailTexture3 = loadTexture("gfx/smoke_05_resized.png");
    bulletTrailTexture4 = loadTexture("gfx/smoke_06_resized.png");
}

void leaveTankTrail(void)
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
        }
    }
}

void leaveBulletTrail(void)
{
    Bullet *b;
    for (b = stage.bHead.next; b != NULL ; b = b->next)
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

void doEffect(void)
{
    leaveTankTrail();
    leaveBulletTrail();
}

void drawEffectUnder(void)
{
    Player *p;
    for (p = stage.pHead.next; p != NULL ; p = p->next)
    {
        Effect *e;
        //Effect *prev = &(p->etrailHead);

        if (p->isBody == 1)
        {
            for (e = p->etrailHead.next; e != NULL ; e = e->next)
            {
                blitRotated(e->texture, e->x, e->y, e->angle);

                // if (--e->health <= 0)
                // {
                //     if (e == p->etrailTail)
                //     {
                //         p->etrailTail = prev;
                //     }

                //     prev->next = e->next;
                //     free(e);
                //     e = prev;
                // }
                
                // prev = e;
            }
        }
    }
}

void drawEffectOver(void)
{
    Bullet *b;
    for (b = stage.bHead.next; b != NULL ; b = b->next)
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