#include "pch.h"
#include "Game.h"
#include <iostream>
#include <windows.h>

void	InitOverworld();
void	DrawOverworld();
void	FreeOverworld();
void	FreeBattle();
void	HandleKeyDownOverworld(SDL_Keycode key);
void	HandleKeyUpOverworld(SDL_Keycode key);
void	UpdateOverworld(float elapsedSec);
void	InitBattle();
void	DrawBattle();
void	UpdateBattle(float elapsedSec);
void	HandleMouseUpBattle(const SDL_MouseButtonEvent& e);
void	PrintTileIndex(float x, float y);

//Basic game functions

#pragma region gameFunctions											
void Start()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	InitAudio();
	InitOverworld();
	InitBattle();
}

void Draw()
{
	ClearBackground(0.f, 0.f, 0.f);

	if (g_IsBattleOn) DrawBattle();
	else DrawOverworld();
	DrawBattleTransitionAnimation();
}

void Update(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (g_IsOverworldOn) UpdateOverworld(elapsedSec);
	if (g_IsBattleOn) UpdateBattle(elapsedSec);
	UpdateBattleOverworldStati();

	g_GlobalTime += elapsedSec;
}

void End()
{
	FreeOverworld();
	FreeBattle();
	EndAudio();
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{
	if (g_IsOverworldOn)
		HandleKeyDownOverworld(key);
}

void OnKeyUpEvent(SDL_Keycode key)
{
	if (key == SDLK_b) {
		if (g_IsBattleOn)
			TurnOffBattle();
		else 
			TurnOnBattle();
	}
	if (g_IsOverworldOn)
		HandleKeyUpOverworld(key);
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	if (g_IsOverworldOn)
		PrintTileIndex(e.x, e.y);
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	if (g_IsBattleOn)
		HandleMouseUpBattle(e);
}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here

void	UpdateBattleOverworldStati() {
	if (!g_IsBattleTransitionOn) return;

	const int nrCols{ 10 };
	const int squareSize{ static_cast<int>(g_WindowWidth / nrCols) };
	const int nrRows{ static_cast<int>(std::ceil(g_WindowHeight / squareSize)) };
	const int nSquares{ static_cast<int>(g_GlobalTime * 7500) / squareSize };

	if (nSquares > nrCols * nrRows) {
		g_IsBattleTransitionOn = false;
		g_GlobalTime = 0.f;
		if (g_IsBattleOn) {
			g_IsOverworldOn = true;
			g_IsBattleOn = false;
		}
		else {
			g_IsBattleOn = true;
		}
		return;
	}
}

void	DrawBattleTransitionAnimation() {
	if (!g_IsBattleTransitionOn) return;

	const int nrCols{ 10 };
	const int squareSize{ static_cast<int>(g_WindowWidth / nrCols) };
	const int nSquares{ static_cast<int>(g_GlobalTime * 7500) / squareSize };

	for (int index{}; index < nSquares; ++index) {
		const int row{ GetRow(index, nrCols) }, col{ GetCol(index, nrCols) };
		SetColor(0.f, 0.f, 0.f, 1.f);
		FillRect(col * squareSize, row * squareSize, squareSize, squareSize);
	}
}


void TurnOnBattle() {
	g_GlobalTime = 0.f;
	g_IsOverworldOn = false;
	g_IsBattleTransitionOn = true;
}

void TurnOffBattle() {
	g_IsBattleTransitionOn = true;
	g_GlobalTime = 0.f;
}

#pragma endregion ownDefinitions