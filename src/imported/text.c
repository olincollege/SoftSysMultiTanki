/*
Copyright (C) 2021 Parallel Realities
*/

#include "text.h"

static void initFont(char *filename);
static int drawTextWrapped(char *text, int x, int y, int r, int g, int b, int align, int maxWidth, int draw);
static void drawTextLine(char *text, int x, int y, int r, int g, int b, int align);
void calcTextDimensions(char *text, int *w, int *h);

static SDL_Color white = {255, 255, 255, 255};
static TTF_Font *font;
static SDL_Rect glyphs[NUM_GLYPHS];
static SDL_Texture *fontTexture;

void initFonts(void)
{
	initFont("fonts/EnterCommand.ttf");

	app.fontScale = 1;
}

static void initFont(char *filename)
{
	SDL_Surface *surface, *text;
	SDL_Rect dest;
	int i;
	char c[2];
	SDL_Rect *g;

	memset(&glyphs, 0, sizeof(SDL_Rect) * NUM_GLYPHS);

	font = TTF_OpenFont(filename, FONT_SIZE);

	surface = SDL_CreateRGBSurface(0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0xff);

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

	dest.x = dest.y = 0;

	for (i = ' ' ; i <= 'z' ; i++)
	{
		c[0] = i;
		c[1] = 0;

		text = TTF_RenderUTF8_Blended(font, c, white);

		TTF_SizeText(font, c, &dest.w, &dest.h);

		if (dest.x + dest.w >= FONT_TEXTURE_SIZE)
		{
			dest.x = 0;

			dest.y += dest.h + 1;

			if (dest.y + dest.h >= FONT_TEXTURE_SIZE)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Out of glyph space in %dx%d font atlas texture map.", FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE);
				exit(1);
			}
		}

		SDL_BlitSurface(text, NULL, surface, &dest);

		g = &glyphs[i];

		g->x = dest.x;
		g->y = dest.y;
		g->w = dest.w;
		g->h = dest.h;

		SDL_FreeSurface(text);

		dest.x += dest.w;
	}

	fontTexture = toTexture(surface, 1);
}

SDL_Texture *getTextTexture(char *text, int type)
{
	SDL_Surface *surface;

	surface = TTF_RenderUTF8_Blended(font, text, white);

	return toTexture(surface, 1);
}

void drawText(char *text, int x, int y, int r, int g, int b, int align, int maxWidth)
{
	if (maxWidth > 0)
	{
		drawTextWrapped(text, x, y, r, g, b, align, maxWidth, 1);
	}
	else
	{
		drawTextLine(text, x, y, r, g, b, align);
	}
}

static int drawTextWrapped(char *text, int x, int y, int r, int g, int b, int align, int maxWidth, int doDraw)
{
	char word[MAX_WORD_LENGTH], line[MAX_LINE_LENGTH];
	int i, n, wordWidth, lineWidth, character, len, newLine, clearWord;

	i = 0;

	memset(word, 0, MAX_WORD_LENGTH);
	memset(line, 0, MAX_LINE_LENGTH);

	character = text[i++];

	n = newLine = clearWord = lineWidth = wordWidth = 0;

	len = strlen(text);

	while (character)
	{
		if (!newLine)
		{
			wordWidth += glyphs[character].w * app.fontScale;

			if (character != ' ')
			{
				word[n++] = character;
			}
		}

		if (character == ' ' || i == len || newLine)
		{
			if (lineWidth + wordWidth >= maxWidth || newLine)
			{
				if (doDraw)
				{
					drawTextLine(line, x, y, r, g, b, align);
				}

				memset(line, 0, MAX_LINE_LENGTH);

				y += glyphs[' '].h * app.fontScale;

				lineWidth = 0;

				newLine = 0;
			}

			clearWord = 1;
		}

		character = text[i++];

		if (character == '\n')
		{
			newLine = 1;

			clearWord = 1;
		}

		if (clearWord)
		{
			clearWord = 0;

			if (lineWidth != 0)
			{
				strcat(line, " ");
			}

			strcat(line, word);

			lineWidth += wordWidth;

			memset(word, 0, MAX_WORD_LENGTH);

			wordWidth = 0;

			n = 0;
		}
	}

	if (doDraw)
	{
		drawTextLine(line, x, y, r, g, b, align);
	}

	return y + glyphs[' '].h * app.fontScale;
}

static void drawTextLine(char *text, int x, int y, int r, int g, int b, int align)
{
	int i, character, w, h;
	SDL_Rect *glyph, dest;

	if (align != TEXT_ALIGN_LEFT)
	{
		calcTextDimensions(text, &w, &h);

		if (align == TEXT_ALIGN_CENTER)
		{
			x -= (w / 2);
		}
		else if (align == TEXT_ALIGN_RIGHT)
		{
			x -= w;
		}
	}

	SDL_SetTextureColorMod(fontTexture, r, g, b);

	i = 0;

	character = text[i++];

	while (character)
	{
		glyph = &glyphs[character];

		dest.x = x;
		dest.y = y;
		dest.w = glyph->w * app.fontScale;
		dest.h = glyph->h * app.fontScale;

		SDL_RenderCopy(app.renderer, fontTexture, glyph, &dest);

		x += glyph->w * app.fontScale;

		character = text[i++];
	}
}

void calcTextDimensions(char *text, int *w, int *h)
{
	int i, character;
	SDL_Rect *g;

	*w = *h = 0;

	i = 0;

	character = text[i++];

	while (character)
	{
		g = &glyphs[character];

		*w += g->w * app.fontScale;
		*h = MAX(g->h * app.fontScale, *h);

		character = text[i++];
	}
}

int getWrappedTextHeight(char *text, int maxWidth)
{
	return drawTextWrapped(text, 0, 0, 255, 255, 255, TEXT_ALIGN_LEFT, maxWidth, 0);
}
