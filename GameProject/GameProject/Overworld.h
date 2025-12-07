#pragma once
#include <utils.h>
#include <iostream>
#include <map>
#include "Audio.h"
using namespace utils;

#pragma region ownDeclarations

//		--- CONST VARIABLES ---

const int	g_NrScenes{ 3 };
const float g_MoveSpeed{ 250.f };

const Color4f g_White(.9f, .9f, .9f, .5f);
const Color4f g_Black(.2f, .2f, .2f, .5f);
const Color4f g_Red(1.f, .0f, .0f, .5f);

//		--- ENUM & STRUCTS ---


struct AnimFrame {
	int row{};
	int col{};
	int nrFrames{};
};

struct Character {
	int			curTile{};
	int			targetTile{};
	Point2f		targetPos{};
	float		offsetTile{};
	bool		isMoving{};

	Rectf		dst{};
	Rectf		src{ 0.f, 0.f, 16.f, 24.f };
	Point2f		dir{};

	AnimFrame	curAnimFrame;
	Point2f		frameDimensions{ 16.f, 24.f };
	int			frameStartIndex{};
	int			frameIndex{};
	Texture		texture{};
};

struct Door {
	std::string	id{};
	int			targetSceneId{};
	std::string	targetEntryId{};
};

// each door has an id, a target scene and its entry point
// each scene has an id and several entry points linked to their spawn location 
// maybe give them each a name and call them from map, same for doors

struct Scene {
	Texture	texture{};
	Texture	fgTexture{};
	Rectf	dst{};
	Point2f	startOffset{};
	float	screenWidth{};
	float	screenHeight{};
	int		id{}; 
	std::map<std::string, int> entryPoints{}; // key = name of entry point, value is target tile
	Door	doors[5]; // max doors = 5 (variable ?)
	int		nrDoors{};
};

struct Camera {
	Point2f pos{};
	float	zoom{ 4.f };
};

struct World {
	Scene	scenes[g_NrScenes]{};
	int		currentSceneIndex{1}; // to debug, set to the scene you want to start at
};

struct KeyPressed {
	bool left{};
	bool right{};
	bool up{};
	bool down{};
};

struct Sounds {
	Mix_Chunk* collision{};
	Mix_Chunk* grass{};
	float		collisionCooldown{};
	float		grassCooldown{};
};


//		--- VARIABLES ---

World		g_World{};
Character	g_Character{};
Camera		g_Camera{};
KeyPressed	g_KeyPressed{};

int			g_NrCols{};
int			g_NrRows{};

std::map<std::string, AnimFrame> g_AnimFrames{};

float		g_FrameTime{};
float		g_TileSize{ 16.f };

int* g_CollisionMaps[g_NrScenes]{};
float		g_CollisionMapSize{};
std::string	g_CollisionMapPaths[g_NrScenes]{};

SDL_Keycode g_CurKey{};
SDL_Keycode g_NextKey{};

float		g_Progression{};
float		g_MoveDist{};

Sounds		g_Sounds{};

float		g_LoadingScreenCooldown{};
float		g_Time{};

//		--- FUNCTIONS ---

//		INIT

void	InitOverworld();
void	InitScenes();
void	InitScene(Scene* pScene);
void	InitCamera();
void	InitCharacter();
void	InitAnimFrames();
void	InitCollisionMapPaths();
void	InitCollisionMap();
void	InitAudioFiles();

//		END

void	FreeOverworld();

//		DRAW

void	DrawOverworld();
void	DrawMap();
void	DrawFgMap();
void	DrawCharacter();
void	DrawTiles();
void	DrawCollisions();
void	DrawLoadingScreen();

//		INPUT HANDLING

void	HandleKeyDownOverworld(SDL_Keycode key);
void	HandleKeyUpOverworld(SDL_Keycode key);
void	OnKeyDownEventOnce(SDL_Keycode key);
SDL_Keycode	UpdateCurKey();

//		UPDATE

void	UpdateOverworld(float elapsedSec);
void	UpdateMapPos(float elapsedSec);
void	UpdateCameraPos(float elapsedSec);
void	UpdateCharacterPos(float elapsedSec);
void	StopWalkingAndReset();
void	HandleWalk();
void	UpdateAnimFrameState();
void	UpdateCharacterFrameInTime(float elapsedSec);
void	UpdateScene();
void	CheckSoundEffect(SDL_Keycode key);
void	CheckBattleInGrass();

//		UTILS

Point2f	GetBottomLeftInRect(const Rectf& rect);
int		TileFromPos(float x, float y);
int		TileFromPos(const Point2f& pos);
Point2f	PosFromTile(int index);
Point2f	PosFromTile(int row, int col);
int		TargetTileFromKey(int curTile, SDL_Keycode key);
Point2f	TargetPosFromKey(Rectf rect, SDL_Keycode key);
Point2f	TargetPosFromKey(Point2f rect, SDL_Keycode key);
Point2f	DirFromKey(SDL_Keycode key);
bool	IsPosInCenterX(float pos);
bool	IsPosInCenterY(float pos);
void	PrintTileIndex(float x, float y);
void	ErrorLoadMsg(const std::string& path, const std::string& name = "file");
bool	IsWalkable(int index);
bool	IsTallGrass(int index);
Door	GetDoor();
bool	IsGoingOutsideMap();



#pragma endregion ownDeclarations
