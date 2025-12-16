#include "pch.h"
#include "Core.h"
#include "Text.h"
#include <cstring>

void LoadFont() {
	g_Font = TTF_OpenFont("../Resources/pokemon_fire_red.ttf", g_DefaultFontSize);

	if (!g_Font) {
		std::cerr << "Failed to load font! " << TTF_GetError() << std::endl;
	}
}

void InitAlphabet() {

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	LoadFont();
	g_AlphabetString = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 _-?!.,;:/+=\"\
";
	int totalAdvance{};

	TextureFromString(g_AlphabetString, g_Font, g_White, g_AlphabetW);
	TextureFromString(g_AlphabetString, g_Font, g_Black, g_AlphabetB);

	for (char c : g_AlphabetString) {
		int advance{};

		if (TTF_GlyphMetrics(g_Font, c, NULL, NULL, NULL, NULL, &advance) == 0)
		{
			g_AlphabetMap[c].pos = totalAdvance;
			g_AlphabetMap[c].advance = advance;
			totalAdvance += advance;
		}
	}
}

void DrawTextFromString(const std::string& str, const Point2f& pos, int fontSize, bool isBlack) {
	Texture* pTexture{ isBlack ? &g_AlphabetB : &g_AlphabetW };
	const float fontRatio{ static_cast<float>(fontSize) / g_DefaultFontSize };

	Rectf dst{ pos.x, pos.y, 0.f, pTexture->height * fontRatio };

	for (char c : str) {
		if (!std::strchr(g_AlphabetString.c_str(), c))
			continue;

		const float advance{ static_cast<float>(g_AlphabetMap[c].advance) };
		const Rectf src{ static_cast<float>(g_AlphabetMap[c].pos), 0.f, advance, pTexture->height };

		dst.width = g_AlphabetMap[c].advance * fontRatio;
		DrawTexture(*pTexture, dst, src);

		dst.left += dst.width;
	}
}

void FreeFontAndAlphabets() {
	DeleteTexture(g_AlphabetB);
	DeleteTexture(g_AlphabetW);
	TTF_CloseFont(g_Font);
}
