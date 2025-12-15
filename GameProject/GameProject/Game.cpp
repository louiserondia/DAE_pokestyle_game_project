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
void	HandleKeyUpBattle(SDL_Keycode key);
void	PrintTileIndex(float x, float y);
void	InitAlphabet();
void	DrawTextFromString(const std::string& str, Point2f pos, int fontSize = g_DefaultFontSize, bool isBlack = 1);
void	EndBattleOverworld();
void	PlayMusicOverworld();
void	PlayMusicBattle();


//Basic game functions

#pragma region gameFunctions											
void Start()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	InitAudio();
	InitOverworld();
	InitBattle();
	InitAlphabet();
	PlayMusicOverworld();
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
	else if (key == SDLK_m) {
		Mix_VolumeMusic(75);
	}
		
	if (g_IsOverworldOn)
		HandleKeyUpOverworld(key);

	if (g_IsBattleOn) HandleKeyUpBattle(key);
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
}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here

	//Mix_FreeMusic(g_GlobalSounds.godmungussBattleMusic);

void	UpdateBattleOverworldStati() {
	if (!g_IsBattleTransitionOn) return;

	if (g_IsDoneDrawing) {
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

void	DrawAnimationCurtainCall() {
	const int nrCols{ 12 };
	const int squareSize{ static_cast<int>(g_WindowWidth / nrCols) };
	const int nSquares{ static_cast<int>(g_GlobalTime * 7500) / squareSize };
	const int nrRows{ static_cast<int>(g_WindowHeight / squareSize) };

	for (int index{}; index < nSquares; ++index) {
		const int row{ GetRow(index, nrCols) };
		int col{ GetCol(index, nrCols) };
		if (index & 1)
			col = nrCols - col;

		SetColor(0.f, 0.f, 0.f, 1.f);
		FillRect(col * squareSize, row * squareSize, squareSize, squareSize);
	}
	if (nSquares > nrCols * nrRows)
		g_IsDoneDrawing = true;
}

void	DrawAnimationHorStripesSpecial() {
	const float height{ 10.f };
	const float speed{ 1000.f };
	const int nLines{ static_cast<int>(g_GlobalTime * speed / height) };
	const int nRows{ static_cast<int>(g_WindowHeight / height) };

	for (int index{}; index < nLines; ++index) {
		float start{ g_GlobalTime * speed - index * 10 - g_WindowWidth };
		if (index & 1)
			start = g_WindowWidth - (g_GlobalTime * speed - index * 10);

		SetColor(0.f, 0.f, 0.f, 1.f);
		FillRect(start, index * height, g_WindowWidth, height);
	}
	if (nLines > nRows * 4)
		g_IsDoneDrawing = true;
}

void	DrawAnimationHorStripes() {
	const float height{ 10.f };
	const float speed{ 1000.f };
	const int nLines{ static_cast<int>(g_GlobalTime * speed / height) };
	const int nRows{ static_cast<int>(g_WindowHeight / height) };

	for (int index{}; index < nLines; ++index) {
		float start{ index & 1 ? g_WindowWidth : 0.f };
		const float width{ g_GlobalTime * speed - index * 10 };

		SetColor(0.f, 0.f, 0.f, 1.f);
		FillRect(start, index * height, index & 1 ? -width : width, height);
	}
	if (nLines > nRows * 3)
		g_IsDoneDrawing = true;
}

void	DrawAnimationDoubleSnake() {
	const int nrCols{ 10 };
	const int squareSize{ static_cast<int>(g_WindowWidth / nrCols) };
	const int nSquares{ static_cast<int>(g_GlobalTime * 3500) / squareSize };
	const int nrRows{ static_cast<int>(g_WindowHeight / squareSize) };

	for (int index{}; index < nSquares; ++index) {
		const int row{ GetRow(index, nrCols) };
		const int invRow{ nrRows - row };
		int col{ GetCol(index, nrCols) };
		int invCol{ col };

		if (row & 1)
			col = nrCols - col - 1;
		else
			invCol = nrCols - invCol - 1;

		SetColor(0.f, 0.f, 0.f, 1.f);
		if (index > nSquares - 4) {
			FillRect(col * squareSize, row * squareSize, squareSize / (5.f - (nSquares - index)), squareSize);
			FillRect(invCol * squareSize, invRow * squareSize, squareSize / (5.f - (nSquares - index)), squareSize);
		}
		else {
			FillRect(col * squareSize, row * squareSize, squareSize, squareSize);
			FillRect(invCol * squareSize, invRow * squareSize, squareSize, squareSize);
		}
	}
	if (nSquares > ((nrCols + 1) * (nrRows + 1)) / 2)
		g_IsDoneDrawing = true;
}

void	DrawBattleTransitionAnimation() {
	if (!g_IsBattleTransitionOn) return;

	//DrawAnimationCurtainCall();
	//DrawAnimationHorStripes();
	//DrawAnimationHorStripesSpecial();
	DrawAnimationDoubleSnake();
}

void TurnOnBattle() {
	g_GlobalTime = 0.f;
	g_IsOverworldOn = false;
	g_IsBattleTransitionOn = true;
	g_IsDoneDrawing = false;
	PlayMusicBattle(); // should be after is done drawing
	// also why no isbattleon = true
	// also should make an enum of state instead of 2 bools (isbattleon & overworld)
}

void TurnOffBattle() {
	g_IsBattleTransitionOn = true;
	g_IsDoneDrawing = false;
	g_GlobalTime = 0.f;
	EndBattleOverworld();
	PlayMusicOverworld();
}

#pragma endregion ownDefinitions