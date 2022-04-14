#include "collision.h"

void collisionWallsBullets(void)
{
    MapTile *m;

    for (m = stage.oHead.next ; m != NULL ; m = m->next)
	{
		Bullet *b;
	
        for (b = stage.bHead.next ; b != NULL ; b = b->next)
        {

        }
	}
}