/*
	Copyright 2022 SeungU Lyu
*/

#include "server.h"

/*
 * Function: doAI
 * ----------------------------
 *  Decides key inputs for the AI players.
 *  Currently it just randomly aims at other player, randomly shoots bullets, and randomly move.
 */
void doAI(void)
{
    int x;
    int y;
    int moveValue = 10;
    int dead[app.maxPlayer];
    int bulletShootChance = 60;

    Player *p;

    for (p = stage.pHead.next ; p != NULL ; p = p->next)
    {
        if (p->isBody && p->isDead > 0)
        {
            dead[p->playerIndex] = 1;
        }
        else if (p->isBody)
        {
            dead[p->playerIndex] = 0;
        }
    }

    for (int i = 0; i < app.maxPlayer; i++)
    {
        if (app.playerIndex == i)
        {
            continue;
        }

        if ((app.playerInputs[i].AIaim == 0 || dead[app.playerInputs[i].AItarget] == 1) && dead[i] == 0)
        {
            int count = 0;
            for (int j = 0; j < app.maxPlayer; j ++)
            {
                if (dead[j] == 1 && i != j)
                {
                    count += 1;
                }
            }

            if (count == app.maxPlayer - 1)
            {
                app.playerInputs[i].AItarget = i;
            }
            else
            {
                app.playerInputs[i].AItarget = rand() % 4;
                while(app.playerInputs[i].AItarget == i || dead[app.playerInputs[i].AItarget] == 1)
                {
                    app.playerInputs[i].AItarget = rand() % 4;
                }
            }
            app.playerInputs[i].AIaim = FPS * 1;
            //printf("%i, %i \n", i, app.playerInputs[i].AItarget);
        }
        else if (app.playerInputs[i].AIaim > 0)
        {
            app.playerInputs[i].AIaim -= 1;
        }

        if (app.playerInputs[i].AItarget == i)
        {
            x = rand() % SCREEN_WIDTH;
            y = rand() % SCREEN_HEIGHT;
        }
        else {
            for (p = stage.pHead.next ; p != NULL ; p = p->next)
            {
                if (p->isBody == 1 && p->playerIndex == app.playerInputs[i].AItarget)
                {
                    x = p->x;
                    y = p->y;
                }
            }
        }
        
        if (app.playerInputs[i].AImove == 0)
        {
            app.playerInputs[i].keyboard[SDL_SCANCODE_W] = 0;
            app.playerInputs[i].keyboard[SDL_SCANCODE_A] = 0;
            app.playerInputs[i].keyboard[SDL_SCANCODE_S] = 0;
            app.playerInputs[i].keyboard[SDL_SCANCODE_D] = 0;

            switch(rand() % 4)
            {
                case 0:
                    app.playerInputs[i].keyboard[SDL_SCANCODE_W] = 1;
                    break;
                case 1:
                    app.playerInputs[i].keyboard[SDL_SCANCODE_A] = 1;
                    break;
                case 2:
                    app.playerInputs[i].keyboard[SDL_SCANCODE_S] = 1;
                    break;
                case 3:
                    app.playerInputs[i].keyboard[SDL_SCANCODE_D] = 1;
                    break;
            }

            float value = 0.5 + (float)(rand() % 10) * 0.1;
            app.playerInputs[i].AImove = FPS * value;
        }
        else
        {
            app.playerInputs[i].AImove -= 1;
        }

        if (x > app.playerInputs[i].mouse.x)
        {
            app.playerInputs[i].mouse.x = app.playerInputs[i].mouse.x + moveValue > x ? x : app.playerInputs[i].mouse.x + moveValue;
        } 
        else
        {
            app.playerInputs[i].mouse.x = app.playerInputs[i].mouse.x - moveValue < x ? x : app.playerInputs[i].mouse.x - moveValue;
        }
        
        if (y > app.playerInputs[i].mouse.y)
        {
            app.playerInputs[i].mouse.y = app.playerInputs[i].mouse.y + moveValue > y ? y : app.playerInputs[i].mouse.y + moveValue;
        } 
        else
        {
            app.playerInputs[i].mouse.y = app.playerInputs[i].mouse.y - moveValue < y ? y : app.playerInputs[i].mouse.y - moveValue;
        }

        if (rand() % bulletShootChance > bulletShootChance - 2)
        {
            app.playerInputs[i].mouse.button[SDL_BUTTON_LEFT] = 1;
        }
        else
        {
            app.playerInputs[i].mouse.button[SDL_BUTTON_LEFT] = 0;
        }
    }
}