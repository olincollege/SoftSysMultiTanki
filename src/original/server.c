#include "server.h"

void doSendToServer(void)
{

}

void doReceiveFromServer(void)
{
    int x;
    int y;
    Player *p;
	
	for (p = stage.pHead.next ; p != NULL ; p = p->next)
    {
        if (p->isBody == 1 && p->playerIndex == app.playerIndex)
        {
            x = p->x;
            y = p->y;
        }
    }
    
    for (int i = 0; i < app.maxPlayer; i++)
    {
        if (app.playerIndex == i)
        {
            continue;
        }

        if (app.playerInputs[i].testInt == 0)
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

            app.playerInputs[i].testInt = FPS * 1;
        }
        else
        {
            app.playerInputs[i].testInt -= 1;
        }

        if (rand() % 30 > 28)
        {
            app.playerInputs[i].mouse.button[SDL_BUTTON_LEFT] = 1;
        }
        else
        {
            app.playerInputs[i].mouse.button[SDL_BUTTON_LEFT] = 0;
        }

        app.playerInputs[i].mouse.x = x;
        app.playerInputs[i].mouse.y = y;
    }
}