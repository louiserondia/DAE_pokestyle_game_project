#pragma once
#include <utils.h>
#include <iostream>
#include <map>

using namespace utils;

#pragma region ownDeclarations

//		--- CONST VARIABLES ---

const int	g_NrScenes{ 1 };
const float	g_TileSize{ 32.f };
const int	g_CharacterNrFrames{ 12 }; // maybe should be nrCol and rows
const float g_MoveSpeed{ 100.f };

//		--- ENUM & STRUCTS ---

enum class CharacterAnimState {
	IdleDown,
	IdleUp,
	IdleLeft,
	IdleRight,
	WalkDown,
	WalkUp,
	WalkLeft,
	WalkRight,
};

struct AnimFrame {
	int row{};
	int col{};
	int nrFrames{};
};

struct Character {
	Rectf		dst{};
	Rectf		src{ 0.f, 0.f, 16.f, 24.f };
	Point2f		dir{};
	float		vx{};
	float		vy{};
	AnimFrame	curAnimFrame;
	Point2f		frameDimensions{ 16.f, 24.f };
	int			frameStartIndex{};
	int			frameIndex{};
	Texture		texture{};
};

// make function to check if not enough to the side of map, then character moves and not map (for shops, black borders)
struct Scene {
	Texture	texture{};
	Rectf	dst{};
	float	startOffset{};
	float	screenWidth{};
	float	screenHeight{};
};

struct Camera {
	Point2f pos{};
	float	zoom{};
};

struct World {
	Scene	scenes[g_NrScenes]{};
	int		currentSceneIndex{};
};


//		--- VARIABLES ---

const float g_WW{ 768.f };
const float g_WH{ 768.f };

World		g_World{};
Character	g_Character{};
Camera		g_Camera{};

std::map<std::string, AnimFrame> g_AnimFrames{};

float		g_FrameTime{};


//		--- FUNCTIONS ---

//		INIT

void	InitOverworld();
void	InitScenes();
void	InitCamera();
void	InitCharacter();
void	InitAnimFrames();

//		END

void	FreeOverworld();

//		DRAW

void	DrawOverworld();
void	DrawMap();
void	DrawCharacter();

//		UPDATE

void	UpdateOverworld(float elapsedSec);
void	UpdateMapPos(float elapsedSec);
void	UpdateCameraPos(float elapsedSec);
void	UpdateCharacterDirSpeed(const Uint8* pStates, float elapsedSec);
void	UpdateCharacterFrame(const Uint8* pStates, float elapsedSec);

//		UTILS

bool	IsBetweenTiles();
bool	IsPosInCenterX(float pos);
bool	IsPosInCenterY(float pos);



#pragma endregion ownDeclarations
