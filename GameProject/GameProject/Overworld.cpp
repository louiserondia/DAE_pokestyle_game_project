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
	g_MoveDist = g_TileSize;
	InitScenes();
	InitAnimFrames();
	InitCharacter();
	InitCamera();
	InitCollisionMapPaths();
	InitCollisionMap();
	InitAudioFiles();
}

void	InitScenes() {
	if (!TextureFromFile("Resources/map_three_island.png", g_World.scenes[0].texture))
		ErrorLoadMsg("Resources/map_three_island.png", "map 0");
	if (!TextureFromFile("Resources/map_three_island_houses_2.png", g_World.scenes[0].fgTexture))
		ErrorLoadMsg("Resources/map_three_island_houses_2.png", "fg map 0");
	g_World.scenes[0].entryPoints["Spawn"] = 183;
	g_World.scenes[0].entryPoints["East"] = 215;
	g_World.scenes[0].nrDoors = 1;
	g_World.scenes[0].doors[0] = Door{ "East", 1, "West" };

	if (!TextureFromFile("Resources/bridge.png", g_World.scenes[1].texture))
		ErrorLoadMsg("Resources/bridge.png", "map 1");
	if (!TextureFromFile("Resources/bridge_fg.png", g_World.scenes[1].fgTexture))
		ErrorLoadMsg("Resources/bridge_fg.png", "fg map 1");
	g_World.scenes[1].entryPoints["Spawn"] = 752;
	g_World.scenes[1].entryPoints["West"] = 752;
	g_World.scenes[1].entryPoints["NorthEast"] = 81;
	g_World.scenes[1].nrDoors = 2;
	g_World.scenes[1].doors[0] = Door{ "West", 0, "East" };
	g_World.scenes[1].doors[1] = Door{ "NorthEast", 2, "South" };

	if (!TextureFromFile("Resources/kindle.png", g_World.scenes[2].texture))
		ErrorLoadMsg("Resources/kindle.png", "map 2");
	if (!TextureFromFile("Resources/kindle_fg.png", g_World.scenes[2].fgTexture))
		ErrorLoadMsg("Resources/kindle_fg.png", "fg map 2");
	g_World.scenes[2].entryPoints["South"] = 2844;
	g_World.scenes[2].entryPoints["North"] = 11;
	g_World.scenes[2].entryPoints["Spawn"] = 2844;
	g_World.scenes[2].nrDoors = 2;
	g_World.scenes[2].doors[0] = Door{ "South", 1, "NorthEast" };
	g_World.scenes[2].doors[1] = Door{ "North", 0, "East" };

	Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };
	InitScene(pScene);
}

void	InitScene(Scene* pScene) {
	const float width{ pScene->texture.width },
		height{ pScene->texture.height },
		screenWidth{ pScene->texture.width * g_Camera.zoom },
		screenHeight{ pScene->texture.height * g_Camera.zoom };

	pScene->screenWidth = screenWidth;
	pScene->screenHeight = screenHeight;
	pScene->startOffset.x = std::max(0.f, (g_WindowWidth - pScene->screenWidth) / 2);
	pScene->startOffset.y = std::max(0.f, (g_WindowHeight - pScene->screenHeight) / 2);

	pScene->dst = Rectf{ 0.f, 0.f, screenWidth, screenHeight };
	g_NrCols = static_cast<int>(screenWidth / g_TileSize);
	g_NrRows = static_cast<int>(screenHeight / g_TileSize);
}

void	InitAnimFrames() {
	g_AnimFrames["walkdown"] = AnimFrame{ 0, 0, 4 };
	g_AnimFrames["walkup"] = AnimFrame{ 0, 4, 4 };
	g_AnimFrames["walkleft"] = AnimFrame{ 0, 8, 4 };
	g_AnimFrames["walkright"] = AnimFrame{ 0, 12, 4 };
}

void	InitCharacter() {
	if (!TextureFromFile("Resources/character.png", g_Character.texture))
		std::cout << "Couldn't load character texture at Resources/character.png";
	Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };

	g_Character.curTile = pScene->entryPoints["Spawn"];
	g_Character.dst = Rectf{
		GetCol(g_Character.curTile, g_NrCols) * g_TileSize,
		GetRow(g_Character.curTile, g_NrCols) * g_TileSize - g_TileSize / 2,
		g_TileSize,
		g_TileSize * 1.5f
	};
	g_Character.targetTile = g_Character.curTile;
	g_Character.curAnimFrame = g_AnimFrames["walkdown"];
	g_Character.frameStartIndex = g_Character.curAnimFrame.col;
	g_Character.frameIndex = 1;
	g_Character.src.left = (g_Character.frameStartIndex + g_Character.frameIndex) * g_Character.frameDimensions.x;
	g_Character.dir = Point2f{ 0.f, 1.f };
}

void	InitCamera() {
	Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };

	Point2f	posCharacterMid{ g_Character.dst.left + (g_Character.dst.width / 2) - g_WindowWidth / 2, g_Character.dst.top - g_WindowHeight / 2 };

	if (posCharacterMid.x < 0) posCharacterMid.x = 0;
	else if (posCharacterMid.x + g_WindowWidth > pScene->dst.width) posCharacterMid.x = pScene->dst.width - g_WindowWidth;
	if (posCharacterMid.y < 0) posCharacterMid.y = 0;
	else if (posCharacterMid.y + g_WindowHeight > pScene->dst.height) posCharacterMid.y = pScene->dst.height - g_WindowHeight;

	g_Camera.pos = posCharacterMid;

}

void	InitCollisionMapPaths() {
	g_CollisionMapPaths[0] = "../Resources/map_three_island_2.txt";
	g_CollisionMapPaths[1] = "../Resources/bridge.txt";
	g_CollisionMapPaths[2] = "../Resources/kindle.txt";
}

void	InitCollisionMap() {
	const int sceneIndex{ g_World.currentSceneIndex };
	std::string path{ g_CollisionMapPaths[sceneIndex] };
	g_CollisionMapSize = static_cast<float>(g_NrCols * g_NrRows);
	g_CollisionMaps[sceneIndex] = new int[static_cast<size_t>(g_CollisionMapSize)];

	std::ifstream	file(path);
	if (!file)
		ErrorLoadMsg(path);

	int index{};
	char ch{};
	while (file.get(ch) && index < static_cast<int>(g_CollisionMapSize)) {
		if (ch == '0' || ch == '1' || ch == '2') {
			g_CollisionMaps[sceneIndex][index] = ch - '0';
			++index;
		}
	}
	//Print2DArray(g_CollisionMaps[sceneIndex], g_CollisionMapSize, g_NrCols);
}

void InitAudioFiles() {
	LoadSoundEffect(g_CollisionSound, "../Resources/collision.wav");
}

//		END

void	FreeOverworld() {
	DeleteTexture(g_World.scenes[0].texture);
	DeleteTexture(g_World.scenes[0].fgTexture);
	DeleteTexture(g_Character.texture);
	Mix_FreeChunk(g_CollisionSound);

	for (int index{}; index < g_NrScenes; ++index) {
		delete[] g_CollisionMaps[index];
	}
}

//		DRAW

void	DrawOverworld() {
	DrawMap();
	//DrawTiles();
	//DrawCollisions();
	DrawCharacter();
	DrawFgMap();
	DrawLoadingScreen();
}

void	DrawMap() {
	const Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };
	DrawTexture(pScene->texture, pScene->dst);
}

void	DrawFgMap() {
	const Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };
	DrawTexture(pScene->fgTexture, pScene->dst);
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

void DrawLoadingScreen() {
	if (g_LoadingScreenCooldown > 1.f)
		return;

	SetColor(0.f, 0.f, 0.f, cosf(g_LoadingScreenCooldown * g_Pi));
	FillRect(0.f, 0.f, g_WindowWidth, g_WindowHeight);
}


//		INPUT HANDLING

void	HandleKeyDownOverworld(SDL_Keycode key) {
	if (key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_UP || key == SDLK_DOWN) {

		if (!g_Character.isMoving && key == SDLK_LEFT && g_Character.dir.x != -1.f)
			g_Character.dir = Point2f{ -1.f, 0.f };
		else if (!g_Character.isMoving && key == SDLK_RIGHT && g_Character.dir.x != 1.f)
			g_Character.dir = Point2f{ 1.f, 0.f };
		else if (!g_Character.isMoving && key == SDLK_UP && g_Character.dir.y != -1.f)
			g_Character.dir = Point2f{ 0.f, -1.f };
		else if (!g_Character.isMoving && key == SDLK_DOWN && g_Character.dir.y != 1.f)
			g_Character.dir = Point2f{ 0.f, 1.f };
		else if (g_CurKey != key)
			g_NextKey = key;
		UpdateAnimFrameState();
		CheckSoundEffect(key);
	}
}

void	UpdateCurKey() {
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	int targetTile{ TargetTileFromKey(g_Character.curTile, g_CurKey) };
	if (targetTile == -1)
		targetTile = TargetTileFromKey(g_Character.curTile, g_NextKey);
	if (targetTile == -1)
		targetTile = g_Character.curTile;

	if (!pStates[SDL_GetScancodeFromKey(g_CurKey)])
		g_CurKey = 0;

	if (g_Character.curTile == g_Character.targetTile && g_NextKey && IsWalkable(targetTile)) {
		g_CurKey = g_NextKey;
		g_NextKey = 0;
		g_Character.dir = DirFromKey(g_CurKey);
		g_Character.targetTile = targetTile;
		g_Character.targetPos = TargetPosFromKey(g_Character.dst, g_CurKey);
		g_Character.isMoving = true;
		UpdateAnimFrameState();
	}
}

//		UPDATE

void	UpdateOverworld(float elapsedSec) {
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	//std::cout << " x : " << g_Character.dst.left << std::endl;
	//std::cout << " y : " << g_Character.dst.top << std::endl;

	UpdateCharacterPos(elapsedSec);
	UpdateCharacterFrameInTime(elapsedSec);
	UpdateMapPos(elapsedSec);
	UpdateCameraPos(elapsedSec);
	UpdateCurKey();
	UpdateScene();

	g_FrameTime += elapsedSec;
	g_Time += elapsedSec;
	g_SoundEffectCooldown += elapsedSec;
	g_LoadingScreenCooldown += elapsedSec;
}

void UpdateCharacterPos(float elapsedSec) {
	if (!g_Character.isMoving)
		return;
	const float dx{ g_Character.dir.x * g_MoveSpeed * elapsedSec },
		dy{ g_Character.dir.y * g_MoveSpeed * elapsedSec };

	if (g_Progression + abs(dx + dy) < g_MoveDist) {
		g_Progression += abs(dx + dy);
		g_Character.dst.left += dx;
		g_Character.dst.top += dy;
	}
	else {
		g_Character.dst.left = g_Character.targetPos.x;
		g_Character.dst.top = g_Character.targetPos.y;
		g_Progression = 0.f;
		g_Character.curTile = g_Character.targetTile;

		const int targetTile{ TargetTileFromKey(g_Character.curTile, g_CurKey) };

		if (!g_CurKey || !IsWalkable(targetTile)) {
			g_Character.isMoving = false;
		}
		else {
			g_Character.targetTile = targetTile;
			g_Character.targetPos = TargetPosFromKey(g_Character.dst, g_CurKey);
		}
	}
}

void UpdateCameraPos(float elapsedSec) {
	if (!g_Character.isMoving)
		return;

	const Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };

	if (g_Character.dst.left + (g_Character.dst.width / 2) - g_WindowWidth / 2 >= 0.f
		&& g_Character.dst.left + (g_Character.dst.width / 2) + g_WindowWidth / 2 <= pScene->screenWidth)
	{
		g_Camera.pos.x = g_Character.dst.left + (g_Character.dst.width / 2) - g_WindowWidth / 2;
	}
	if (g_Character.dst.top - g_WindowHeight / 2 >= 0.f
		&& g_Character.dst.top + g_WindowHeight / 2 <= pScene->screenHeight)
	{
		g_Camera.pos.y = g_Character.dst.top - g_WindowHeight / 2;
	}
}

void	UpdateMapPos(float elapsedSec) {
	Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };

	pScene->dst.left = -g_Camera.pos.x;
	pScene->dst.top = -g_Camera.pos.y;
}

void	UpdateAnimFrameState() {
	if (g_Character.dir.x == 1.f)
		g_Character.curAnimFrame = g_AnimFrames["walkright"];
	else if (g_Character.dir.x == -1.f)
		g_Character.curAnimFrame = g_AnimFrames["walkleft"];
	else if (g_Character.dir.y == -1.f)
		g_Character.curAnimFrame = g_AnimFrames["walkup"];
	else if (g_Character.dir.y == 1.f)
		g_Character.curAnimFrame = g_AnimFrames["walkdown"];
}

void	UpdateCharacterFrameInTime(float elapsedSec) {
	g_Character.frameStartIndex = g_Character.curAnimFrame.col;
	g_Character.src.left = (g_Character.frameStartIndex + g_Character.frameIndex) * g_Character.frameDimensions.x;

	const float frameRate{ 1.f / 8 };

	if (g_FrameTime > frameRate) {
		g_FrameTime = 0.f;
		if (g_Character.isMoving)
			g_Character.frameIndex = (g_Character.frameIndex + 1) % g_Character.curAnimFrame.nrFrames;
		else
			g_Character.frameIndex = 1;
	}
}

void	CheckSoundEffect(SDL_Keycode key) {
	const float cooldown{ .6f };
	const int targetTile{ TargetTileFromDir(g_Character.curTile, g_Character.dir) };

	if (!IsWalkable(targetTile) && g_SoundEffectCooldown > cooldown) {
		PlaySoundEffect(g_CollisionSound);
		g_SoundEffectCooldown = 0.f;
	}
}

Door GetDoor() {
	Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };
	const int sceneIndex{ g_World.currentSceneIndex };
	const int row{ GetRow(g_Character.curTile, g_NrCols) },
		col{ GetCol(g_Character.curTile, g_NrCols) };

	if (!sceneIndex) {
		return pScene->doors[0]; // east
	}
	else if (sceneIndex == 1) {
		if (row == 0)
			return pScene->doors[1]; // north east
		else
			return pScene->doors[0]; // west
	}
	else if (sceneIndex == 2) {
		if (row == 0)
			return pScene->doors[1]; // north 
		else if (row == g_NrRows - 1)
			return pScene->doors[0]; // south
	}
	return pScene->doors[0];
}

void UpdateScene() {
	if (!IsGoingOutsideMap())
		return;

	g_LoadingScreenCooldown = 0.f;
	g_Character.isMoving = false; 
	// theres an issue here, going right is fine but left since hitbox is on the left, he still walks one tile
	// for up and down it walks half a tile because detection is in the middle
	//maybe check is updating then dont move or teleport back

	const Door door{ GetDoor() };

	g_World.currentSceneIndex = door.targetSceneId;
	Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] }; // new scene

	InitScene(pScene);

	g_Character.curTile = pScene->entryPoints[door.targetEntryId];
	g_Character.dst.left = GetCol(g_Character.curTile, g_NrCols) * g_TileSize;
	g_Character.dst.top = GetRow(g_Character.curTile, g_NrCols) * g_TileSize - g_TileSize / 2;
	g_Character.targetTile = g_Character.curTile;

	InitCamera();
	InitCollisionMap();
}

//		UTILS

Point2f	GetBottomLeftInRect(const Rectf& rect) {
	return Point2f{ rect.left, rect.top + rect.height };
}

int	TileFromPos(float x, float y) {
	return GetIndex(static_cast<int>(y / g_TileSize), static_cast<int>(x / g_TileSize), g_NrCols);
}

int	TileFromPos(const Point2f& pos) {
	return GetIndex(static_cast<int>(pos.y / g_TileSize), static_cast<int>(pos.x / g_TileSize), g_NrCols);
}

Point2f	PosFromTile(int index) {
	return PosFromTile(GetRow(index, g_NrCols), GetCol(index, g_NrCols));
}

Point2f	PosFromTile(int row, int col) {
	return Point2f{ col * g_TileSize, row * g_TileSize };
}

Point2f	DirFromKey(SDL_Keycode key) {
	if (key == SDLK_LEFT)
		return Point2f{ -1.f, 0.f };
	if (key == SDLK_RIGHT)
		return Point2f{ 1.f, 0.f };
	if (key == SDLK_UP)
		return Point2f{ 0.f, -1.f };
	else
		return Point2f{ 0.f, 1.f };
}

int		TargetTileFromKey(int curTile, SDL_Keycode key) {
	if (key == SDLK_LEFT)
		return curTile - 1;
	else if (key == SDLK_RIGHT)
		return curTile + 1;
	else if (key == SDLK_UP)
		return curTile - g_NrCols;
	else if (key == SDLK_DOWN)
		return curTile + g_NrCols;
	return -1;
}

int		TargetTileFromDir(int curTile, Point2f dir) {
	if (dir.x == -1.f)
		return curTile - 1;
	else if (dir.x == 1.f)
		return curTile + 1;
	else if (dir.y == -1.f)
		return curTile - g_NrCols;
	else
		return curTile + g_NrCols;
}

Point2f		TargetPosFromKey(Rectf rect, SDL_Keycode key) {
	return TargetPosFromKey(Point2f{ rect.left, rect.top }, key);
}

Point2f		TargetPosFromKey(Point2f pos, SDL_Keycode key) {
	if (key == SDLK_LEFT)
		return Point2f{ pos.x - g_MoveDist, pos.y };
	else if (key == SDLK_RIGHT)
		return Point2f{ pos.x + g_MoveDist, pos.y };
	else if (key == SDLK_UP)
		return Point2f{ pos.x, pos.y - g_MoveDist };
	else if (key == SDLK_DOWN)
		return Point2f{ pos.x, pos.y + g_MoveDist };
	return Point2f{ -1.f, -1.f };
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

void	DrawTiles() {
	Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };

	for (int row{}; row < g_NrRows; ++row) {
		for (int col{}; col < g_NrCols; ++col) {
			SetColor(g_White);
			DrawRect(
				pScene->dst.left + col * g_TileSize,
				pScene->dst.top + row * g_TileSize,
				g_TileSize,
				g_TileSize,
				3.f
			);
		}
	}
}

void	DrawCollisions() {
	Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };

	for (int row{}; row < g_NrRows; ++row) {
		for (int col{}; col < g_NrCols; ++col) {

			SetColor(!IsWalkable(GetIndex(row, col, g_NrCols)) ? g_Red : g_White);
			FillRect(
				pScene->dst.left + col * g_TileSize,
				pScene->dst.top + row * g_TileSize,
				g_TileSize,
				g_TileSize
			);
		}
	}
}

void PrintTileIndex(float x, float y) {
	std::cout << "Tile index from position [" << x << ", " << y << "] : " << TileFromPos(x + g_Camera.pos.x, y + g_Camera.pos.y) << std::endl;
}

void ErrorLoadMsg(const std::string& path, const std::string& name) {
	std::cout << "Couldn't load " << name << " at " << path << std::endl;
}

bool IsWalkable(int index) {
	return !(g_CollisionMaps[g_World.currentSceneIndex][index] == 1);
}

bool IsTallGrass(int index) {
	return g_CollisionMaps[g_World.currentSceneIndex][index] == 2;
}

// make start of walk animation when collision (changes leg each time)

bool IsGoingOutsideMap() {
	const Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };
	const Point2f curPos{ g_Character.dst.left, g_Character.dst.top + g_TileSize };

	Point2f targetPos{ TargetPosFromKey(curPos, g_CurKey) };
	if (targetPos.x == -1.f)
		targetPos = TargetPosFromKey(curPos, g_NextKey);
	if (targetPos.x == -1.f)
		targetPos = curPos;

	return(targetPos.x < 0 && curPos.x <= 0.f)
		|| (targetPos.x > pScene->screenWidth && curPos.x >= pScene->screenWidth)
		|| (targetPos.y < 0 && curPos.y <= 0.f)
		|| (targetPos.y > pScene->screenHeight && curPos.y >= pScene->screenHeight);
}

#pragma endregion ownDeclarations