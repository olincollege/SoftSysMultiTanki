/*
Copyright (C) 2018 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "sound.h"

static void loadSounds(void);

static Mix_Chunk *sounds[SND_MAX];
static Mix_Music *music;

void initSounds(void)
{
	memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);
	
	music = NULL;
	
	loadSounds();

	Mix_Volume(CH_PLAYER1_TANK_TRAIL, 25);
	Mix_Volume(CH_PLAYER2_TANK_TRAIL, 25);
	Mix_Volume(CH_PLAYER1_TANK, 75);
	Mix_Volume(CH_PLAYER2_TANK, 75);
	Mix_Volume(CH_PLAYER1_BULLET, 50);
	Mix_Volume(CH_PLAYER2_BULLET, 50);
}

void loadMusic(char *filename)
{
	if (music != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}

	music = Mix_LoadMUS(filename);
}

void playMusic(int loop)
{
	Mix_PlayMusic(music, (loop) ? -1 : 0);
}

void playSound(int id, int channel)
{
	Mix_PlayChannel(channel, sounds[id], 0);
}

static void loadSounds(void)
{
	sounds[SND_TANK_MOVE] = Mix_LoadWAV("sound/impactWood_light_001.ogg");
	sounds[SND_BULLET_FIRE] = Mix_LoadWAV("sound/impactWood_heavy_004.ogg");
	sounds[SND_BULLET_REFLECT] = Mix_LoadWAV("sound/impactWood_medium_002.ogg");
	sounds[SND_BULLET_EXPLOSION] = Mix_LoadWAV("sound/impactPunch_heavy_004.ogg");
	sounds[SND_TANK_EXPLOSION] = Mix_LoadWAV("sound/impactMining_000.ogg");
	sounds[SND_COUNTDOWN] = Mix_LoadWAV("sound/impactMetal_light_003.ogg");
	sounds[SND_GAME_START] = Mix_LoadWAV("sound/impactBell_heavy_000.ogg");
}
