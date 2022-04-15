#include "collision.h"

void collisionWallsBullets(void)
{
    Bullet *b;

    for (b = stage.bHead.next ; b != NULL ; b = b->next)
    {
        MapTile *m;
        for (m = stage.oHead.next ; m != NULL ; m = m->next)
        {
            float cp[2][2];

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
                break;
            }
        }
    }
}