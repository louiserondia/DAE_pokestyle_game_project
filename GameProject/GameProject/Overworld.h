#pragma once
#include <utils.h>
#include <iostream>
#include <map>

using namespace utils;

#pragma region ownDeclarations

//		--- CONST VARIABLES ---

const int	g_NrScenes{ 1 };
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


// quand on press une key, elle est ajoutée comme next key
// si target et curTile sont == (ça inclut aussi le cas où y a pas de curKey), curKey = next key
// quand on lance la curKey, on ajoute 1 à targetTile dans la direction
// ensuite fonction en boucle déplace position vers target (avancement stocké dans une variable)
// quand l'avancement est fini, tile = target 
// si tile == target et curKey != null -> avancer encore 

struct Character {
	int			curTile{};
	int			targetTile{};
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

// make function to check if not enough to the side of map, then character moves and not map (for shops, black borders)
struct Scene {
	Texture	texture{};
	Rectf	dst{};
	Point2f	startOffset{};
	float	screenWidth{};
	float	screenHeight{};
};

struct Camera {
	Point2f pos{};
	float	zoom{ 4.f };
};

struct World {
	Scene	scenes[g_NrScenes]{};
	int		currentSceneIndex{};
};


//		--- VARIABLES ---

World		g_World{};
Character	g_Character{};
Camera		g_Camera{};

int			g_NrCols{};
int			g_NrRows{};

std::map<std::string, AnimFrame> g_AnimFrames{};

float		g_FrameTime{};
float		g_TileSize{ 8.f };

SDL_Keycode g_CurKey{};
SDL_Keycode g_NextKey{};

//		--- FUNCTIONS ---

//		INIT

void	InitOverworld();
void	InitScenes();
void	InitCamera();
void	InitCharacter();
void	InitAnimFrames();
void	InitCollisionMap();

//		END

void	FreeOverworld();

//		DRAW

void	DrawOverworld();
void	DrawMap();
void	DrawCharacter();

//		INPUT HANDLING

void	HandleKeyDownOverworld(SDL_Keycode key);
void	UpdateCurKey();

//		UPDATE

void	UpdateOverworld(float elapsedSec);
void	UpdateMapPos(float elapsedSec);
void	UpdateCameraPos(float elapsedSec);
void	UpdateCharacterPos(float elapsedSec);
void	UpdateCharacterFrame(float elapsedSec);

//		UTILS

int		TileFromPos(float x, float y);
int		TileFromPos(const Point2f& pos);
Point2f	PosFromTile(int index);
Point2f	PosFromTile(int row, int col);
int		GetTargetTile(int curTile, SDL_Keycode key);
Point2f	GetDirFromKey(SDL_Keycode key);
bool	IsPosInCenterX(float pos);
bool	IsPosInCenterY(float pos);



#pragma endregion ownDeclarations
