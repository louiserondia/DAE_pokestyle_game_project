#pragma once
#include <utils.h>
#include <iostream>
#include <map>
#include "Audio.h"
using namespace utils;

#pragma region ownDeclarations

//		--- CONST VARIABLES ---

const int	g_NrScenes{ 3 };
const int	g_NrNPC{ 1 };

const Color4f g_White(.9f, .9f, .9f, .5f);
const Color4f g_Black(.2f, .2f, .2f, .5f);
const Color4f g_Red(1.f, .0f, .0f, .5f);

//		--- ENUM & STRUCTS ---

struct Frame {
	int	start{};
	int	index{};
	int	end{};
};

struct AnimTextureFrames {
	Frame	sea{ 0, 0, 8 };
	Frame	rock{ 0, 0, 8 };
	Frame	flower{ 0, 0, 5 };
};

struct AnimFrame {
	int row{};
	int col{};
	int nrFrames{};
};

struct Door {
	std::string	id{};
	int			targetSceneId{};
	std::string	targetEntryId{};

	// each door has an id, a target scene and its entry point
	// each scene has an id and several entry points linked to their spawn location 
	// maybe give them each a name and call them from map, same for doors
};

struct Scene {
	std::string name{};

	Texture	texture{};
	Texture	fgTexture{};

	Rectf	dst{};
	Point2f	startOffset{};
	float	screenWidth{};
	float	screenHeight{};
	int		nrRows{};
	int		nrCols{};

	Door	doors[5];
	int		nrDoors{};
	std::map<std::string, int> entryPoints{}; // key = name of entry point, value is target tile

	float	tileSize{ 16.f };

	char* animTextureMap{};
	int					animTextureMapSize{};
	std::string			animTextureMapPath{};
	AnimTextureFrames	animTextureFrames{};

	int* collisionMap{};
	float				collisionMapSize{};
	std::string			collisionMapPath{};

	void	LoadStatic();
	void	Init();
};

struct Character {
	int			curTile{};
	int			targetTile{};
	Point2f		targetPos{};
	float		offsetTile{};
	bool		isMoving{};
	float		progression{};

	Rectf		dst{};
	Rectf		src{ 0.f, 0.f, 16.f, 24.f };
	Point2f		dir{};

	AnimFrame	curAnimFrame{}; // frame 2D pos in sprite sheet and nr frames
	// change to only one frame, maybe 2d and start == row and add index
	Frame		frame{};
	float		frameTime{};

	void		Draw();
	void		UpdateFrame(float elapsedSec, float frameRate = 1 / 8.f);
	void		UpdateAnimFrameState();
	void UpdatePos(float elapsedSec, float speed, float maxDist);

};

// change everything so character is a special npc and make functions as methods  like update pos and others i pass an npc

struct Player : Character {
	void Init(const Scene& scene);
	//void UpdatePos(float elapsedSec, float speed, float maxDist);
};

struct NPC : Character {
	bool isMvtVertical{};
	int	startTile{};

	void Init(int tile, const Rectf& dimensions, const Scene& scene);
	void Walk();
	//void Walk(float elapsedSec, float moveSpeed);
};

struct Camera {
	Point2f pos{};
	float	zoom{ 4.f };
};

struct World {
	Scene	scenes[g_NrScenes]{};
	int		curSceneIndex{ 0 }; // to debug, set to the scene you want to start at
	const float moveSpeed{ 250.f };
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
Player		g_Player{};
Camera		g_Camera{};
NPC			g_NPC[g_NrNPC]{};
Sounds		g_Sounds{};
KeyPressed	g_KeyPressed{};

std::map<std::string, AnimFrame> g_AnimFrames{};

Texture		g_NPCTexture{};
Texture		g_AnimTextures{};
Texture		g_WaterShadowTexture{};

SDL_Keycode g_CurKey{};
SDL_Keycode g_NextKey{};

float		g_LoadingScreenCooldown{};
float		g_AnimTextureTime{};
float		g_Time{};


//		--- FUNCTIONS ---

//		INIT

void	InitOverworld();
void	InitScenes();
void	InitCamera();
void	InitCharacters();
void	InitAnimFrames();
void	InitCollisionMap();
void	InitAnimTextureMap();
void	InitAudioFiles();

//		END

void	FreeOverworld();

//		DRAW

void	DrawOverworld();
void	DrawSea();
void	DrawRocks();
void	DrawFlowers();
void	DrawMap();
void	DrawFgMap();
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
void	StopWalkingAndReset();
void	HandlePlayerWalk();
void	UpdateAnimTextureFrames();
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
int		TargetTileFromDir(int curTile, const Point2f& dir);
Point2f	TargetPosFromKey(const Rectf& rect, SDL_Keycode key);
Point2f	TargetPosFromKey(const Point2f& rect, SDL_Keycode key);
Point2f	DirFromKey(SDL_Keycode key);
bool	IsPosInCenterX(float pos);
bool	IsPosInCenterY(float pos);
void	PrintTileIndex(float x, float y);
void	ErrorLoadMsg(const std::string& path, const std::string& name = "file");
bool	IsWalkable(int index);
bool	IsTallGrass(int index);
bool	IsPlayerOnTile(int index);
bool	IsNPCOnTile(int index);
Door	GetDoor();
bool	IsGoingOutsideMap();
Point2f Turn(Point2f dir, float angle);

#pragma endregion ownDeclarations
