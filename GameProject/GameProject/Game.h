#pragma once
#include <utils.h>
#include <map>
#include "Audio.h"
using namespace utils;

#pragma region gameInformation
// TODO: Set your name and group in the title here
std::string g_WindowTitle{ "Pokestyle Game Project - Rahimov, Javid & Rondia, Louise - 1DAE12" };


//	       \:.             .:/
//	        \``._________.''/ 
//	         \             / 
//	 .--.--, / .':.   .':. \	
//	/__:  /  | '::' . '::' |
//	   / /   |`.   ._.   .'|
//	  / /    |.'         '.|
//	 /___-_-,|.\  \   /  /.|
//	      // |''\.;   ;,/ '|
//	      `==|:=         =:|	
//	         `.          .'
//             :-._____.-:
//	          `''       `''

// Change the window dimensions here
float g_WindowWidth{ 1080 };
float g_WindowHeight{ 720 };
#pragma endregion gameInformation

#pragma region ownDeclarations
// Declare your own global variables here

bool	g_IsBattleOn{};		// press B to switch from battle to map view and mechanics
bool	g_IsOverworldOn{ 1 };		// press B to switch from battle to map view and mechanics
bool	g_IsBattleTransitionOn{};
bool	g_IsDoneDrawing{ true };

float	g_GlobalTime{};

struct Mushroom {
	float	x;
	float	y;
	float	size;
	int		speed;
	bool	isFixed;
};

Texture		g_Mushroom{};
const int	g_NrMushrooms{ 150 };
Mushroom	g_MushroomsPos[g_NrMushrooms]{};
bool		g_IsDrawingMushroom{};
float		g_MushroomsThreshold{};

struct Sounds {
	Mix_Music* godmungussBattleMusic{};
};

Sounds g_GlobalSounds{};

const int	g_DefaultFontSize{ 150 };
int			g_PokemonIdFromOverworld{};
bool		g_IsBlinking{};
float		g_BlinkTime{};

// Declare your own functions here

void InitMushrooms();

void TurnOnBattle(int pokemonId);
void TurnOffBattle();
void UpdateBattleOverworldStati();
void UpdateMushroomsPos(float elapsedSec);

void DrawBattleTransitionAnimation();
void DrawBlinkBattle();
void DrawAnimationCurtainCall();
void DrawAnimationHorStripes();
void DrawAnimationHorStripesSpecial();
void DrawAnimationDoubleSnake();
void DrawAnimationMushrooms();

#pragma endregion ownDeclarations

#pragma region gameFunctions											
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling
