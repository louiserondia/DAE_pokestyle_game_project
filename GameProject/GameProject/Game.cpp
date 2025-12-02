#include "pch.h"
#include "Game.h"
#include <iostream>
#include <windows.h>

void	InitOverworld();
void	DrawOverworld();
void	FreeOverworld();
void	HandleKeyDownOverworld(SDL_Keycode key);
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
	InitOverworld();
	InitBattle();
}

void Draw()
{
	ClearBackground(0.f, 0.f, 0.f);

	if (g_IsBattleOn) DrawBattle();
	else DrawOverworld();
}

void Update(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (!g_IsBattleOn) UpdateOverworld(elapsedSec);
	else UpdateBattle(elapsedSec);
}

void End()
{
	FreeOverworld();
	// we need to delete all the textures here !!
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{
	if (key == SDLK_b)
		g_IsBattleOn = !g_IsBattleOn;
	HandleKeyDownOverworld(key);
}

void OnKeyUpEvent(SDL_Keycode key)
{
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	PrintTileIndex(e.x, e.y);
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	HandleMouseUpBattle(e);
}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here


#pragma endregion ownDefinitions