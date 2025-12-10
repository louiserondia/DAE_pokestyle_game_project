#pragma once
#include <utils.h>
#include <iostream>
#include <map>
using namespace utils;

struct GlyphInfo {
	int pos{};
	int advance{};
};

const Color4f g_White{ 1.f, 1.f, 1.f, 1.f };
const Color4f g_Black{ 0.f, 0.f, 0.f, 1.f };

Texture g_AlphabetB{};
Texture g_AlphabetW{};
TTF_Font* g_Font{};
std::string g_AlphabetString{};

std::map<char, GlyphInfo> g_AlphabetMap{};

float			g_AlphabetSize{};
const int		g_DefaultFontSize{ 100 };

void LoadFont();
void InitAlphabet();
void DrawTextFromString(const std::string& str, Point2f pos, int fontSize = g_DefaultFontSize, bool isBlack = 1);
void FreeFontAndAlphabets();

// Draw a text from a string, the default font size is 100 but can be srinked and set to white or black

// ex :
//	DrawTextFromString("Hello World", Point2f{ 0.f, 0.f });
//	DrawTextFromString("Hello World", Point2f{ 0.f, 0.f }, 80);
//	DrawTextFromString("Hello World", Point2f{ 0.f, 0.f }, 80, 0);
