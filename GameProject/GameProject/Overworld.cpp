#include "pch.h"
#include "Core.h"
#include "Overworld.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>


#pragma region ownDeclarations

//		INITIALIZATIONS

void	InitOverworld() {
	g_TileSize *= g_Camera.zoom;
	InitScenes();
	InitCamera();
	InitAnimFrames();
	InitCharacter();
	InitCollisionMap();
}

void	InitScenes() {
	if (!TextureFromFile("Resources/map_three_island.png", g_World.scenes[0].texture))
		std::cout << "Couldn't load map texture at Resources/map_three_island.png";
	Scene* pScene{ &g_World.scenes[0] };

	const float width{ pScene->texture.width },
		height{ pScene->texture.height },
		screenWidth{ pScene->texture.width * g_Camera.zoom },
		screenHeight{ pScene->texture.height * g_Camera.zoom };

	pScene->screenWidth = screenWidth;
	pScene->screenHeight = screenHeight;
	pScene->startOffset.x = std::max(0.f, (g_WindowWidth - pScene->screenWidth) / 2);
	pScene->startOffset.y = std::max(0.f, (g_WindowHeight - pScene->screenHeight) / 2);

	//std::cout << "start offset y : " << pScene->startOffset.y;
	pScene->dst = Rectf{ 0.f, 0.f, screenWidth, screenHeight };
	g_NrCols = pScene->texture.width / g_TileSize;
	g_NrRows = pScene->texture.height / g_TileSize;
}

void	InitCamera() {
	Scene* pScene{ &g_World.scenes[0] };

	g_Camera.pos.x = -pScene->startOffset.x;
	g_Camera.pos.y = -pScene->startOffset.y;
}

void	InitAnimFrames() {
	g_AnimFrames["idledown"] = AnimFrame{ 0, 0, 1 };
	g_AnimFrames["idleup"] = AnimFrame{ 0, 3, 1 };
	g_AnimFrames["idleleft"] = AnimFrame{ 0, 6, 1 };
	g_AnimFrames["idleright"] = AnimFrame{ 0, 9, 1 };
	g_AnimFrames["walkdown"] = AnimFrame{ 0, 1, 2 };
	g_AnimFrames["walkup"] = AnimFrame{ 0, 4, 2 };
	g_AnimFrames["walkleft"] = AnimFrame{ 0, 7, 2 };
	g_AnimFrames["walkright"] = AnimFrame{ 0, 10, 2 };
}

void	InitCharacter() {
	if (!TextureFromFile("Resources/character.png", g_Character.texture))
		std::cout << "Couldn't load character texture at Resources/character.png";

	g_Character.dst = Rectf{
		8 * g_TileSize,
		8 * g_TileSize,
		g_TileSize * 2,
		g_TileSize * 3
	};
	g_Character.curTile = TileFromPos(g_Character.dst.left, g_Character.dst.top);
	g_Character.targetTile = g_Character.curTile;
	g_Character.curAnimFrame = g_AnimFrames["idledown"];
	g_Character.dir = Point2f{ 0.f, 1.f };
}

void	InitCollisionMap() {
	std::ifstream	file("Resources/map_three_island.txt");
	if (!file) {

	}
}

//		END

void	FreeOverworld() { // FreeAll instead ?
	DeleteTexture(g_World.scenes[0].texture);
	DeleteTexture(g_Character.texture);
}

//		DRAW

void	DrawOverworld() {
	DrawMap();
	DrawCharacter();
}

void	DrawMap() {
	const Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };
	DrawTexture(pScene->texture, pScene->dst);
}

void	DrawCharacter() {
	const Rectf rect{
		g_Character.dst.left - g_Camera.pos.x,
		g_Character.dst.top - g_Camera.pos.y,
		g_Character.dst.width ,
		g_Character.dst.height
	};
	DrawTexture(g_Character.texture, rect, g_Character.src);
}

//		INPUT HANDLING

void	HandleKeyDownOverworld(SDL_Keycode key) {
	if (key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_UP || key == SDLK_DOWN) {
		if (g_CurKey != key)
			g_NextKey = key;
	}
}

void	UpdateCurKey() {

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (!pStates[SDL_GetScancodeFromKey(g_CurKey)])
		g_CurKey = 0;

	if (g_Character.curTile == g_Character.targetTile && g_NextKey) {
		g_CurKey = g_NextKey;
		g_NextKey = 0;
		g_Character.dir = GetDirFromKey(g_CurKey);
		g_Character.targetTile = GetTargetTile(g_Character.curTile, g_CurKey);
		g_Character.isMoving = true;
	}
	else if (g_Character.curTile == g_Character.targetTile && g_CurKey) {
		g_Character.targetTile = GetTargetTile(g_Character.curTile, g_CurKey);
		g_Character.isMoving = true;
	}
}

//		UPDATE

void	UpdateOverworld(float elapsedSec) {
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	UpdateCharacterPos(elapsedSec);
	UpdateCharacterFrame(elapsedSec);
	UpdateMapPos(elapsedSec);
	UpdateCameraPos(elapsedSec);
	UpdateCurKey();

	g_FrameTime += elapsedSec;
}

void UpdateCharacterPos(float elapsedSec) {
	const int newTile{ GetIndex(g_Character.dst.top / g_TileSize, g_Character.dst.left / g_TileSize, g_NrCols) };

	if (g_Character.isMoving && newTile != g_Character.targetTile) {
		g_Character.dst.left += g_Character.dir.x * g_MoveSpeed * elapsedSec;
		g_Character.dst.top += g_Character.dir.y * g_MoveSpeed * elapsedSec;
	}
	else if (g_Character.isMoving) {
		if (!g_CurKey)
			g_Character.isMoving = false;
		g_Character.curTile = g_Character.targetTile;
	}
}

void UpdateCameraPos(float elapsedSec) {
	if (!g_Character.isMoving)
		return;

	const Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };
	const float
		screenWidth{ pScene->texture.width * g_Camera.zoom },
		screenHeight{ pScene->texture.height * g_Camera.zoom };
	const float
		dX{ g_Character.dir.x * g_MoveSpeed * elapsedSec },
		dY{ g_Character.dir.y * g_MoveSpeed * elapsedSec };


	if (IsPosInCenterX(g_Character.dst.left - g_Camera.pos.x - dX)
		&& g_Camera.pos.x + dX >= 0.f
		&& g_Camera.pos.x + dX <= screenWidth - g_WindowWidth)
	{
		g_Camera.pos.x += dX;
	}
	if (IsPosInCenterY(g_Character.dst.top - g_Camera.pos.y - dY)
		&& g_Camera.pos.y + dY >= 0.f
		&& g_Camera.pos.y + dY <= screenHeight - g_WindowHeight)
	{
		g_Camera.pos.y += dY;
	}
}

void	UpdateMapPos(float elapsedSec) {
	Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };

	pScene->dst.left = -g_Camera.pos.x;
	pScene->dst.top = -g_Camera.pos.y;
}

void	UpdateCharacterFrame(float elapsedSec) {

	//	maybe i can call the update startindex when i udpate is moving instead of all the time
	// and update frame for animation all the time

	if (g_Character.isMoving) {
		if (g_Character.dir.x == 1.f)
			g_Character.curAnimFrame = g_AnimFrames["walkright"];
		else if (g_Character.dir.x == -1.f)
			g_Character.curAnimFrame = g_AnimFrames["walkleft"];
		else if (g_Character.dir.y == -1.f)
			g_Character.curAnimFrame = g_AnimFrames["walkup"];
		else if (g_Character.dir.y == 1.f)
			g_Character.curAnimFrame = g_AnimFrames["walkdown"];
	}
	else {
		if (g_Character.dir.x == 1.f)
			g_Character.curAnimFrame = g_AnimFrames["idleright"];
		else if (g_Character.dir.x == -1.f)
			g_Character.curAnimFrame = g_AnimFrames["idleleft"];
		else if (g_Character.dir.y == -1.f)
			g_Character.curAnimFrame = g_AnimFrames["idleup"];
		else if (g_Character.dir.y == 1.f)
			g_Character.curAnimFrame = g_AnimFrames["idledown"];
	}

	g_Character.frameStartIndex = GetIndex(g_Character.curAnimFrame.row, g_Character.curAnimFrame.col, g_CharacterNrFrames);
	g_Character.src.left = GetCol(g_Character.frameStartIndex + g_Character.frameIndex, g_CharacterNrFrames) * g_Character.frameDimensions.x;

	const float frameRate{ 1.f / 8 };

	if (g_FrameTime > frameRate) {
		g_FrameTime = 0.f;
		g_Character.frameIndex = (g_Character.frameIndex + 1) % g_Character.curAnimFrame.nrFrames;
	}
}

//		UTILS

int	TileFromPos(float x, float y) {
	return GetIndex(static_cast<int>(x / g_TileSize), static_cast<int>(y / g_TileSize), g_NrCols);
}

int	TileFromPos(const Point2f& pos) {
	return GetIndex(static_cast<int>(pos.x / g_TileSize), static_cast<int>(pos.y / g_TileSize), g_NrCols);
}

Point2f	PosFromTile(int index) {
	return PosFromTile(GetCol(index, g_NrCols), GetRow(index, g_NrCols));
}

Point2f	PosFromTile(int row, int col) {
	return Point2f{ row * g_TileSize, col * g_TileSize };
}

Point2f	GetDirFromKey(SDL_Keycode key) {
	if (key == SDLK_LEFT)
		return Point2f{ -1.f, 0.f };
	if (key == SDLK_RIGHT)
		return Point2f{ 1.f, 0.f };
	if (key == SDLK_UP)
		return Point2f{ 0.f, -1.f };
	else
		return Point2f{ 0.f, 1.f };
}

int		GetTargetTile(int curTile, SDL_Keycode key) {
	if (key == SDLK_LEFT)
		return curTile - 1;
	if (key == SDLK_RIGHT)
		return curTile + 1;
	if (key == SDLK_UP)
		return curTile - g_NrCols;
	else
		return curTile + g_NrCols;
}

bool	IsPosInCenterX(float pos) {
	const float centerX = g_WindowWidth / 2.f;
	const float epsilon = 10.f;

	return std::abs(pos - centerX) < epsilon;
}

bool	IsPosInCenterY(float pos) {
	const float centerY = g_WindowHeight / 2.f;
	const float epsilon = g_TileSize;

	return std::abs(pos - centerY) < epsilon;
}

#pragma endregion ownDeclarations