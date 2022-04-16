#include "effect.h"

static SDL_Texture *trailTexture;

void initEffect(void)
{
    trailTexture = loadTexture("gfx/tracksSmall.png");
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

            e->texture = trailTexture;
            e->x = p->x;
            e->y = p->y;
            e->angle = p->angle;
        }
    }
}

void doEffect(void)
{
    leaveTankTrail();
}

void drawEffect(void)
{
    Player *p;
    for (p = stage.pHead.next; p != NULL ; p = p->next)
    {
        if (p->isBody == 1)
        {
            Effect *e;
            for (e = p->etrailHead.next; e != NULL ; e = e->next)
            {
                blitRotated(e->texture, e->x, e->y, e->angle);
            }
        }
    }
}