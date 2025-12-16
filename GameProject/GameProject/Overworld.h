#pragma once
#include <utils.h>
#include <iostream>
#include <map>
#include "Audio.h"
using namespace utils;

#pragma region ownDeclarations

//		--- CONST VARIABLES ---

const int		g_NrScenes{ 6 };
const int		g_NrNPC{ 3 };

const Color4f	g_White(.9f, .9f, .9f, .5f);
const Color4f	g_Black(.2f, .2f, .2f, .5f);
const Color4f	g_Red(1.f, .0f, .0f, .5f);
const Color4f	g_Blue(.0f, 0.f, 1.f, .5f);

//		--- ENUM & STRUCTS ---

struct Camera;
struct Character;
struct Player;
struct NPC;

struct Frame {
	int	start{};
	int	index{};
	int	end{};
};

struct AnimTextureFrames {
	Frame	sea{ 0, 0, 8 };
	Frame	rock{ 0, 0, 8 };
	Frame	flower{ 0, 0, 5 };
	Frame	smoke{ 0, 0, 12 };
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

	Door	doors[5];
	int		nrDoors{};
	std::map<std::string, int> entryPoints{}; // key = name of entry point, value is target tile

	static int		nrCols;
	static int		nrRows;
	static float	tileSize; // is it really better here or can be global ?
	static bool		isCave;

	char* animTextureMap{};
	int					animTextureMapSize{};
	std::string			animTextureMapPath{};
	AnimTextureFrames	animTextureFrames{};

	int* collisionMap{};
	int					collisionMapSize{};
	std::string			collisionMapPath{};

	void	LoadStatic();
	void	Init();
	void	InitCollisionMap();
	void	InitAnimTextureMap();


	void	DrawSea() const;
	void	DrawRocks() const;
	void	DrawFlowers() const;
	void	DrawSmoke() const;
	void	DrawMap() const;
	void	DrawFgMap() const;

	void	UpdateMapPos(float elapsedSec, const Camera& camera);
	void	UpdateAnimTextureFrames();
};

struct Character {
	int			curTile{};
	int			targetTile{};
	Point2f		targetPos{};
	float		offsetTile{};
	bool		isMoving{};
	float		stepProgress{};
	bool		isGod{};

	Rectf		dst{};
	Rectf		src{ 0.f, 0.f, 16.f, 24.f };
	Point2f		dir{};

	AnimFrame	curAnimFrame{}; // frame 2D pos in sprite sheet and nr frames
	// change to only one frame, maybe 2d and start == row and add index
	Frame		frame{};
	float		frameTime{};
	static Texture texture;

	void		Draw() const;
	void		UpdateFrame(float elapsedSec, float frameRate = 1 / 8.f);
	void		UpdateAnimFrameState();
	void		UpdatePos(float elapsedSec, float speed, float maxDist);
	void		StopWalkingAndReset(int& curKey, int& nextKey);
};


struct Player : Character {
	bool isFrozen{};

	void Init(int entryPoint);
};

struct NPC : Character {
	bool isMoto{};
	int	startTile{};
	int sceneIndex{};
	int path[100]{};
	Point2f pathDir[100];
	int pathIndex{};
	int pathLength{};
	bool isLooping{};
	bool isWalkingTowardsPlayer{};
	bool hasBattled{};

	static int battleRange;

	void Init(int sceneIndex, int tile, const Rectf& dimensions, bool isMoving = 1, bool isMoto = 0);
	void Walk();
	void Drive();
	void UpdateMotoFrame();
	void FollowPath();
	void DrawPath() const;
	void EngageBattle(Player& player);
	bool IsPlayerInRange(const Player& player);
};

struct Camera {
	Point2f pos{};
	Point2f	backupPos{};
	static float	zoom;
	bool	isBlinking{};

	void Init(const Scene& scene);
	void UpdatePos(float elapsedSec, const Scene& scene, const Player& player);
};

struct World {
	Scene	scenes[g_NrScenes]{};
	int		curSceneIndex{ 0 }; // to debug, set to the scene you want to start at
	const float moveSpeed{ 220.f };
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
	Mix_Music* overworldMusic{};
	Mix_Music* caveMusic{};

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

Texture		g_AnimTextures{};
Texture		g_WaterShadowTexture{};

SDL_Keycode g_CurKey{};
SDL_Keycode g_NextKey{};

float		g_LoadingScreenCooldown{};
float		g_AnimTextureTime{};
float		g_SmokeTime{};
float		g_Time{};
float		g_BlinkTime{};


//		--- FUNCTIONS ---

//		INIT

void	InitOverworld();
void	InitScenes();
void 	InitCharacters(const Scene& scene, int sceneIndex);
void	InitAnimFrames();
void	InitAudioFiles();
void	InitNPCPath(NPC& npc, int tile, Point2f dir);

//		END

void	FreeOverworld();

//		DRAW

void	DrawOverworld();
void	DrawLoadingScreen();
void	DrawBlinkBattle();

//		INPUT HANDLING

void		HandleKeyDownOverworld(SDL_Keycode key);
void		HandleKeyUpOverworld(SDL_Keycode key);
void		OnKeyDownEventOnce(SDL_Keycode key);
SDL_Keycode	UpdateCurKey();

//		UPDATE

void	UpdateOverworld(float elapsedSec);
void	UpdateScene(Camera& camera, Player& player);
void	HandlePlayerWalk();
void	CheckSoundEffect(SDL_Keycode key);
void	PlayMusicOverworld();
void	CheckBattleInGrass();
void	EndBattleOverworld();

//		UTILS

int		TileFromPos(float x, float y);
int		TileFromPos(const Point2f& pos);
Point2f	PosFromTile(int index);
Point2f	PosFromTile(int row, int col);
int		TargetTileFromKey(int curTile, SDL_Keycode key);
int		TargetTileFromDir(int curTile, const Point2f& dir);
Point2f	TargetPosFromKey(const Rectf& rect, SDL_Keycode key);
Point2f	TargetPosFromKey(const Point2f& rect, SDL_Keycode key);
int		TileDist(int start, int end, float tileSize);
Point2f	DirFromKey(SDL_Keycode key);

Point2f	GetBottomLeftRect(const Rectf& rect);
bool	IsPosInCenterX(float pos);
bool	IsPosInCenterY(float pos);

bool	IsWalkable(int index);
bool	IsTallGrass(int index);
bool	IsPlayerOnTile(int index);
bool	IsNPCOnTile(int index);
bool	IsGoingOutsideMap();

void	Turn(Point2f& dir, float angle);
void	Turn90(Point2f& dir);
Door	GetDoor();
Scene& GetScene();


// DEBUG

void	PrintTileIndex(float x, float y);
void	ErrorLoadMsg(const std::string& path, const std::string& name = "file");
void	DrawTiles();
void	DrawCollisions();
void	DrawCurrentAndTargetTiles();

#pragma endregion ownDeclarations
