/*
Copyright (C) 2021 Parallel Realities
*/

#include "text.h"

static void initFont(int fontType, char *filename);
static int drawTextWrapped(char *text, int x, int y, int r, int g, int b, int fontType, int align, int maxWidth, int draw);
static void drawTextLine(char *text, int x, int y, int r, int g, int b, int fontType, int align);
void calcTextDimensions(char *text, int fontType, int *w, int *h);
static int nextGlyph(const char *str, int *i, char *glyphBuffer);

static SDL_Color white = {255, 255, 255, 255};
static TTF_Font *fonts[FONT_MAX];
static SDL_Rect glyphs[FONT_MAX][MAX_GLYPHS];
static SDL_Texture *fontTextures[FONT_MAX];
static char *characters = "Ö&|_# POfileorTBFS:handWCpygt2015-6,JwsbuGNUL3.Emj@c/\"IV\\RMD8+v?x;=%!AYq()'kH[]KzQX4Z79*àéí¡Çóè·úïçüºòÉÒÍÀ°æåøÆÅØ<>öÄäßÜá¿ñÁÊûâîôÈêùœÙìëęąłćżńśźŻŚŁĆÖ";

void initFonts(void)
{
	initFont(FONT_PARTY, "fonts/EnterCommand.ttf");
}

static void initFont(int fontType, char *filename)
{
	SDL_Surface *surface, *text;
	SDL_Rect dest;
	int i, n;
	char glyphBuffer[MAX_GLYPH_SIZE];

	memset(&glyphs[fontType], 0, sizeof(SDL_Rect) * MAX_GLYPHS);

	fonts[fontType] = TTF_OpenFont(filename, FONT_SIZE);

	surface = SDL_CreateRGBSurface(0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0xff);

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

	dest.x = dest.y = 0;

	i = 0;

	while ((n = nextGlyph(characters, &i, glyphBuffer)) != 0)
	{
		if (n >= MAX_GLYPHS)
		{
			printf("Glyph '%s' index exceeds array size (%d >= %d)\n", glyphBuffer, n, MAX_GLYPHS);
			exit(1);
		}

		text = TTF_RenderUTF8_Blended(fonts[fontType], glyphBuffer, white);

		TTF_SizeText(fonts[fontType], glyphBuffer, &dest.w, &dest.h);

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

		glyphs[fontType][n] = dest;

		SDL_FreeSurface(text);

		dest.x += dest.w;
	}

	fontTextures[fontType] = toTexture(surface, 1);
}

SDL_Texture *getTextTexture(char *text, int type)
{
	SDL_Surface *surface;

	surface = TTF_RenderUTF8_Blended(fonts[type], text, white);

	return toTexture(surface, 1);
}

void drawText(char *text, int x, int y, int r, int g, int b, int fontType, int align, int maxWidth)
{
	if (maxWidth > 0)
	{
		drawTextWrapped(text, x, y, r, g, b, fontType, align, maxWidth, 1);
	}
	else
	{
		drawTextLine(text, x, y, r, g, b, fontType, align);
	}
}

static int drawTextWrapped(char *text, int x, int y, int r, int g, int b, int fontType, int align, int maxWidth, int doDraw)
{
	char word[MAX_WORD_LENGTH], line[MAX_LINE_LENGTH], glyphBuffer[MAX_GLYPH_SIZE];
	int i, n, wordWidth, lineWidth, len;

	i = 0;

	memset(word, 0, MAX_WORD_LENGTH);
	memset(line, 0, MAX_LINE_LENGTH);

	n = 0;

	lineWidth = wordWidth = 0;

	len = strlen(text);

	while ((n = nextGlyph(text, &i, glyphBuffer)) != 0)
	{
		wordWidth += glyphs[fontType][n].w;

		if (n != ' ')
		{
			strcat(word, glyphBuffer);
		}

		if (n == ' ' || i == len)
		{
			if (lineWidth + wordWidth >= maxWidth)
			{
				if (doDraw)
				{
					drawTextLine(line, x, y, r, g, b, fontType, align);
				}

				memset(line, 0, MAX_LINE_LENGTH);

				y += glyphs[fontType][' '].h;

				lineWidth = 0;
			}
			else if (lineWidth != 0)
			{
				strcat(line, " ");
			}

			strcat(line, word);

			lineWidth += wordWidth;

			memset(word, 0, MAX_WORD_LENGTH);

			wordWidth = 0;
		}
	}

	if (doDraw)
	{
		drawTextLine(line, x, y, r, g, b, fontType, align);
	}

	return y + glyphs[fontType][' '].h;
}

static void drawTextLine(char *text, int x, int y, int r, int g, int b, int fontType, int align)
{
	int i, w, h, n;
	SDL_Rect *glyph, dest;

	if (align != TEXT_ALIGN_LEFT)
	{
		calcTextDimensions(text, fontType, &w, &h);

		if (align == TEXT_ALIGN_CENTER)
		{
			x -= (w / 2);
		}
		else if (align == TEXT_ALIGN_RIGHT)
		{
			x -= w;
		}
	}

	SDL_SetTextureColorMod(fontTextures[fontType], r, g, b);

	i = 0;

	while ((n = nextGlyph(text, &i, NULL)) != 0)
	{
		glyph = &glyphs[fontType][n];

		dest.x = x;
		dest.y = y;
		dest.w = glyph->w;
		dest.h = glyph->h;

		SDL_RenderCopy(app.renderer, fontTextures[fontType], glyph, &dest);

		x += glyph->w;
	}
}

void calcTextDimensions(char *text, int fontType, int *w, int *h)
{
	int i, n;
	SDL_Rect *g;

	*w = *h = 0;

	i = 0;

	while ((n = nextGlyph(text, &i, NULL)) != 0)
	{
		g = &glyphs[fontType][n];

		*w += g->w;
		*h = MAX(g->h, *h);
	}
}

int getWrappedTextHeight(char *text, int fontType, int maxWidth)
{
	return drawTextWrapped(text, 0, 0, 255, 255, 255, fontType, TEXT_ALIGN_LEFT, maxWidth, 0);
}

static int nextGlyph(const char *str, int *i, char *glyphBuffer)
{
	int len;
	unsigned int bit;
	const char *p;

	bit = (unsigned char) str[*i];

	if (bit < ' ')
	{
		return 0;
	}

	len = 1;

	if (bit >= 0xF0)
	{
		bit  = (int)(str[*i]     & 0x07) << 18;
		bit |= (int)(str[*i + 1] & 0x3F) << 12;
		bit |= (int)(str[*i + 2] & 0x3F) << 6;
		bit |= (int)(str[*i + 3] & 0x3F);

		len = 4;
	}
	else if (bit >= 0xE0)
	{
		bit  = (int)(str[*i]     & 0x0F) << 12;
		bit |= (int)(str[*i + 1] & 0x3F) << 6;
		bit |= (int)(str[*i + 2] & 0x3F);

		len = 3;
	}
	else if (bit >= 0xC0)
	{
		bit  = (int)(str[*i]     & 0x1F) << 6;
		bit |= (int)(str[*i + 1] & 0x3F);

		len = 2;
	}

	/* only fill the buffer if it's been supplied */
	if (glyphBuffer != NULL)
	{
		p = str + *i;

		memset(glyphBuffer, 0, MAX_GLYPH_SIZE);

		memcpy(glyphBuffer, p, len);
	}

	*i = *i + len;

	return bit;
}
