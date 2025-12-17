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
void	InitBattle(int pokemonId);
void	DrawBattle();
void	UpdateBattle(float elapsedSec);
void	HandleKeyUpBattle(SDL_Keycode key);
void	PrintTileIndex(float x, float y);
void	InitAlphabet();
void	DrawTextFromString(const std::string& str, const Point2f& pos, int fontSize = g_DefaultFontSize, bool isBlack = 1);
void	EndBattleOverworld();
void	PlayMusicOverworld();
void	PlayMusicBattle();


//Basic game functions

#pragma region gameFunctions											
void Start()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	TextureFromFile("Resources/mushroom.png", g_Mushroom);

	InitMushrooms();
	InitAudio();
	InitOverworld();
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
	UpdateMushroomsPos(elapsedSec);

	g_GlobalTime += elapsedSec;
	g_BlinkTime += elapsedSec;
}

void End()
{
	FreeOverworld();
	EndAudio();
	DeleteTexture(g_Mushroom);
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
			TurnOnBattle(rand() % 4);
	}
	else if (key == SDLK_m) {
		Mix_VolumeMusic(0);
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

	if (g_IsBlinking && g_BlinkTime > .8f) {
		g_GlobalTime = 0.f;
		g_IsBlinking = false;
	}
	if (g_IsDoneDrawing) {
		g_IsBattleTransitionOn = false;
		g_GlobalTime = 0.f;
		g_IsBlinking = false;
		if (g_IsBattleOn) {
			g_IsOverworldOn = true;
			g_IsBattleOn = false;
			PlayMusicOverworld();
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
	if (nLines > nRows)
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

void DrawAnimationMushrooms() {
	for (int index{}; index < g_NrMushrooms; ++index) {
		const Rectf dst{ g_MushroomsPos[index].x, g_MushroomsPos[index].y, g_MushroomsPos[index].size, g_MushroomsPos[index].size };
		DrawTexture(g_Mushroom, dst);
	}
}

void InitMushrooms() {
	const float baseSpeed{ 400.f }, baseSize{ 200.f }, variationSpeed{350.f}, variationSize{ 150.f };
	g_MushroomsThreshold = g_WindowHeight - 200.f;

	for (int index{}; index < g_NrMushrooms; ++index) {
		g_MushroomsPos[index].x = rand() % static_cast<int>(g_WindowWidth) - g_MushroomsPos[index].size / 2;
		g_MushroomsPos[index].y = -g_MushroomsPos[index].size - rand() % 1500;
		g_MushroomsPos[index].speed = baseSpeed + static_cast<float>(rand()) / RAND_MAX * variationSpeed;
		g_MushroomsPos[index].size = baseSize + static_cast<float>(rand()) / RAND_MAX * variationSize;
		g_MushroomsPos[index].isFixed = false;
	}
}
void UpdateMushroomsPos(float elapsedSec) {
	if (!g_IsDrawingMushroom || g_IsDoneDrawing || g_IsBlinking) return;

	if (g_MushroomsThreshold < -300.f) {
		g_IsDoneDrawing = true;
		g_IsDrawingMushroom = false;
		return;
	}
	std::cout << "g_MushroomsThreshold : " << g_MushroomsThreshold << std::endl;

	for (int index{}; index < g_NrMushrooms; ++index) {
		if (g_MushroomsPos[index].isFixed) continue;

		const float delta{ elapsedSec * g_MushroomsPos[index].speed };
		g_MushroomsPos[index].y += delta;

		if (g_MushroomsPos[index].y >= g_MushroomsThreshold) {
			g_MushroomsThreshold -= delta * 0.6f;
			g_MushroomsPos[index].isFixed = true;
		}

	}
}

void DrawBlinkBattle() {
	const float pulsation{ 15.7f };

	SetColor(0.f, 0.f, 0.f, sinf(pulsation * g_BlinkTime - (pulsation / 2)) / 2 + .5f);
	FillRect(0.f, 0.f, g_WindowWidth, g_WindowHeight);
}


void	DrawBattleTransitionAnimation() {
	if (!g_IsBattleTransitionOn) return;

	if (g_IsBlinking) {
		DrawBlinkBattle();
	}
	else if (g_PokemonIdFromOverworld < 2) {
		DrawAnimationDoubleSnake();
	}
	else if (g_PokemonIdFromOverworld < 4) {
		DrawAnimationCurtainCall();
	}
	else if (g_PokemonIdFromOverworld == 4) {
		DrawAnimationMushrooms();
	}
}

/// <summary>
/// Starts the battle with a random pokemon. Tall grass, cave and npc will return a different type of pokemon
/// </summary>
/// <param name="pokemonId">0-1 : tall grass and npc, 2-3 : cave and npc, 4 : Godmoongus </param>
void TurnOnBattle(int pokemonId) {
	g_PokemonIdFromOverworld = pokemonId;
	g_IsOverworldOn = false;
	g_IsBattleTransitionOn = true;
	g_IsBlinking = true;
	g_BlinkTime = 0.f;
	g_IsDoneDrawing = false;

	if (!g_IsDrawingMushroom && pokemonId == 4) {
		g_IsDrawingMushroom = true;
		InitMushrooms();
	}
	InitBattle(g_PokemonIdFromOverworld);
	PlayMusicBattle();
}

void TurnOffBattle() {
	if (!g_IsDrawingMushroom && g_PokemonIdFromOverworld == 4) {
		g_IsDrawingMushroom = true;
		InitMushrooms();
	}
	g_IsBattleTransitionOn = true;
	g_IsBlinking = true;
	g_GlobalTime = 0.f;
	g_IsDoneDrawing = false;
	EndBattleOverworld();
	FreeBattle();
}

// should make an enum of state instead of 2 bools (isbattleon & overworld)

#pragma endregion ownDefinitions