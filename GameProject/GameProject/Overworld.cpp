#include "pch.h"
#include "Core.h"
#include "Overworld.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>

void TurnOnBattle();

#pragma region ownDeclarations

//								                   ▄▄                  ▄▄ 
//	  ▄▄▄   	▀▀        ▀▀  ██                   ██                  ██ 
//	▄████   	██  ████▄ ██ ▀██▀▀   ▀▀█▄ ████▄ ▄████   ▄█▀█▄ ████▄ ▄████ 
//	  ███   	██  ██ ██ ██  ██    ▄█▀██ ██ ██ ██ ██   ██▄█▀ ██ ██ ██ ██ 
//	  ███ ██	██▄ ██ ██ ██▄ ██    ▀█▄██ ██ ██ ▀████   ▀█▄▄▄ ██ ██ ▀████ 

void	InitOverworld() {

	// init static
	if (!TextureFromFile("Resources/character.png", g_NPCTexture))
		std::cout << "Couldn't load character texture at Resources/character.png";
	if (!TextureFromFile("Resources/animated_tiles.png", g_AnimTextures))
		ErrorLoadMsg("Resources/animated_tiles.png");
	if (!TextureFromFile("Resources/water_shadow.png", g_WaterShadowTexture))
		ErrorLoadMsg("Resources/water_shadow.png");

	InitScenes();
	InitAnimFrames();
	InitCharacters();
	InitCamera();
	InitCollisionMap();
	InitAnimTextureMap();
	InitAudioFiles();
}

void	InitScenes() {
	Scene* pScene{ &g_World.scenes[0] };

	pScene->name = "three_island";
	pScene->entryPoints["Spawn"] = 183;
	pScene->entryPoints["East"] = 215;
	pScene->nrDoors = 1;
	pScene->doors[0] = Door{ "East", 1, "West" };
	pScene->LoadStatic();

	pScene = &g_World.scenes[1];

	pScene->name = "bridge";
	pScene->entryPoints["Spawn"] = 1085;
	pScene->entryPoints["West"] = 752;
	pScene->entryPoints["NorthEast"] = 81;
	pScene->nrDoors = 2;
	pScene->doors[0] = Door{ "West", 0, "East" };
	pScene->doors[1] = Door{ "NorthEast", 2, "South" };
	pScene->LoadStatic();

	pScene = &g_World.scenes[2];

	pScene->name = "kindle";
	pScene->entryPoints["South"] = 2844;
	pScene->entryPoints["North"] = 11;
	pScene->entryPoints["Spawn"] = 2844;
	pScene->nrDoors = 2;
	pScene->doors[0] = Door{ "South", 1, "NorthEast" };
	pScene->doors[1] = Door{ "North", 0, "East" };
	pScene->LoadStatic();

	g_World.scenes[g_World.curSceneIndex].Init();
}

void	Scene::LoadStatic() {
	if (!TextureFromFile("Resources/" + name + ".png", texture))
		ErrorLoadMsg("Resources/" + name + ".png");
	if (!TextureFromFile("Resources/" + name + "_fg.png", fgTexture))
		ErrorLoadMsg("Resources/" + name + "_fg.png");
	collisionMapPath = "../Resources/" + name + "_collisions.txt";
	animTextureMapPath = "../Resources/" + name + "_textures.txt";
}

void	Scene::Init() {
	tileSize = 16.f * g_Camera.zoom;
	screenWidth = texture.width * g_Camera.zoom;
	screenHeight = texture.height * g_Camera.zoom;
	startOffset.x = std::max(0.f, (g_WindowWidth - screenWidth) / 2);
	startOffset.y = std::max(0.f, (g_WindowHeight - screenHeight) / 2);

	dst = Rectf{ 0.f, 0.f, screenWidth, screenHeight };
	nrCols = static_cast<int>(screenWidth / tileSize);
	nrRows = static_cast<int>(screenHeight / tileSize);
}

void InitAnimFrames() {
	g_AnimFrames["walkdown"] = AnimFrame{ 0, 0, 4 };
	g_AnimFrames["walkup"] = AnimFrame{ 0, 4, 4 };
	g_AnimFrames["walkleft"] = AnimFrame{ 0, 8, 4 };
	g_AnimFrames["walkright"] = AnimFrame{ 0, 12, 4 };
}

void InitCharacters() {
	g_Player.Init(g_World.scenes[0]);
	g_NPC[0].Init(203, Rectf{ 0.f, 0.f, 16.f, 24.f }, g_World.scenes[g_World.curSceneIndex]);
}

void Player::Init(const Scene& scene) {
	curTile = scene.entryPoints.at("Spawn");
	dst = Rectf{
		GetCol(curTile, scene.nrCols) * scene.tileSize,
		GetRow(curTile, scene.nrCols) * scene.tileSize - scene.tileSize / 2,
		scene.tileSize,
		scene.tileSize * 1.5f
	};
	targetTile = curTile;
	curAnimFrame = g_AnimFrames["walkdown"];
	frame.start = curAnimFrame.col;
	frame.index = 1;
	src.left = (frame.start + frame.index) * src.width;
	dir = Point2f{ 0.f, 1.f };
}

void NPC::Init(int tile, const Rectf& dimensions, const Scene& scene) {
	curTile = tile;
	dst = Rectf{
		GetCol(curTile, scene.nrCols) * scene.tileSize,
		GetRow(curTile, scene.nrCols) * scene.tileSize - scene.tileSize / 2,
		scene.tileSize,
		scene.tileSize * 1.5f
	};
	curAnimFrame = g_AnimFrames["walkdown"]; // add to animframes one for the motorbike
	frame.start = 0;
	frame.index = 1;
	src = Rectf{
		(frame.start + frame.index) * dimensions.width,
		dimensions.top,
		dimensions.width,
		dimensions.height
	};
	dir = Point2f{ 0.f, 1.f };
}

void	InitCamera() {
	const Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	Point2f	posPlayerMid{ g_Player.dst.left + (g_Player.dst.width / 2) - g_WindowWidth / 2, g_Player.dst.top - g_WindowHeight / 2 };

	if (posPlayerMid.x < 0) posPlayerMid.x = 0;
	else if (posPlayerMid.x + g_WindowWidth > scene.dst.width) posPlayerMid.x = scene.dst.width - g_WindowWidth;
	if (posPlayerMid.y < 0) posPlayerMid.y = 0;
	else if (posPlayerMid.y + g_WindowHeight > scene.dst.height) posPlayerMid.y = scene.dst.height - g_WindowHeight;

	g_Camera.pos = posPlayerMid;
}

void	InitCollisionMap() {
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	std::string path{ scene.collisionMapPath };
	scene.collisionMapSize = static_cast<float>(scene.nrCols * scene.nrRows);
	scene.collisionMap = new int[static_cast<size_t>(scene.collisionMapSize)];

	std::ifstream	file(path);
	if (!file)
		ErrorLoadMsg(path);

	int index{};
	char ch{};
	while (file.get(ch) && index < static_cast<int>(scene.collisionMapSize)) {
		if (ch == '0' || ch == '1' || ch == '2') {
			scene.collisionMap[index] = ch - '0';
			++index;
		}
	}
	//Print2DArray(scene.collisionMap[sceneIndex], scene.collisionMapSize, scene.nrCols);
}


void	InitAnimTextureMap() {
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	std::string path{ scene.animTextureMapPath };

	scene.animTextureMapSize = scene.nrCols * scene.nrRows;
	scene.animTextureMap = new char[scene.animTextureMapSize];

	std::ifstream	file(path);
	if (!file)
		ErrorLoadMsg(path);

	int index{};
	char ch{};
	while (file.get(ch) && index < scene.animTextureMapSize) {
		if (ch == '0' || ch == '1' || ch == '2' || ch == 'm' || ch == 'r' || ch == 'f') {
			scene.animTextureMap[index] = ch;
			++index;
		}
	}
}

void InitAudioFiles() {
	LoadSoundEffect(g_Sounds.collision, "../Resources/collision.wav");
	LoadSoundEffect(g_Sounds.grass, "../Resources/grass.wav");
}

//		END

void	FreeOverworld() {
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	DeleteTexture(g_NPCTexture);
	DeleteTexture(g_WaterShadowTexture);
	DeleteTexture(g_AnimTextures);
	Mix_FreeChunk(g_Sounds.collision);
	Mix_FreeChunk(g_Sounds.grass);

	for (int index{}; index < g_NrScenes; ++index) {
		DeleteTexture(scene.texture);
		DeleteTexture(scene.fgTexture);
		delete[] scene.collisionMap;
		delete[] scene.animTextureMap;
	}
}

//			         ▄▄                     
//			         ██                     
//	▄▀▀██▄ 	      ▄████ ████▄  ▀▀█▄ ██   ██ 
//	  ▄██▀ 	      ██ ██ ██ ▀▀ ▄█▀██ ██ █ ██ 
//	▄███▄▄▄	 ██   ▀████ ██    ▀█▄██  ██▀██  
//

void	DrawOverworld() {
	DrawSea();
	DrawRocks();
	DrawMap();
	DrawFlowers();
	//DrawCollisions();
	//DrawTiles();
	g_Player.Draw();
	g_NPC[0].Draw();
	DrawFgMap();
	DrawLoadingScreen();
}

void DrawSea() {
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };
	const float size{ g_AnimTextures.width / scene.animTextureFrames.sea.end };
	const char* map{ scene.animTextureMap };

	for (int index{}; index < scene.animTextureMapSize; ++index) {
		if (scene.animTextureMap[index] != 'm' && scene.animTextureMap[index] != 'r')
			continue;

		const Rectf dst{
			GetCol(index, scene.nrCols) * scene.tileSize - g_Camera.pos.x,
			GetRow(index, scene.nrCols) * scene.tileSize - g_Camera.pos.y,
			scene.tileSize, scene.tileSize
		};
		const Rectf src{ scene.animTextureFrames.sea.index * size, 0.f, size, size };

		DrawTexture(g_AnimTextures, dst, src);
	}
}

void DrawRocks() {
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };
	const float size{ g_AnimTextures.width / scene.animTextureFrames.sea.end };

	for (int index{}; index < scene.animTextureMapSize; ++index) {
		if (scene.animTextureMap[index] != 'r')
			continue;

		Rectf dst{
			GetCol(index, scene.nrCols) * scene.tileSize - g_Camera.pos.x,
			GetRow(index, scene.nrCols) * scene.tileSize - g_Camera.pos.y,
			scene.tileSize * 2, scene.tileSize * 2
		};
		const Rectf src{ scene.animTextureFrames.rock.index * size, size, size, size };

		DrawTexture(g_WaterShadowTexture, dst);

		dst.left += scene.tileSize / 2;
		dst.top += scene.tileSize / 2;
		dst.width /= 2;
		dst.height /= 2;
		DrawTexture(g_AnimTextures, dst, src);
	}
}

void DrawFlowers() {
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };
	const float size{ g_AnimTextures.width / scene.animTextureFrames.sea.end };

	for (int index{}; index < scene.animTextureMapSize; ++index) {
		if (scene.animTextureMap[index] != 'f')
			continue;

		const Rectf dst{
			GetCol(index, scene.nrCols) * scene.tileSize - g_Camera.pos.x,
			GetRow(index, scene.nrCols) * scene.tileSize - g_Camera.pos.y,
			scene.tileSize, scene.tileSize
		};
		const Rectf src{ scene.animTextureFrames.flower.index * size, 2 * size, size, size };

		DrawTexture(g_AnimTextures, dst, src);
	}
}

void	DrawMap() {
	const Scene& scene{ g_World.scenes[g_World.curSceneIndex] };
	DrawTexture(scene.texture, scene.dst);
}

void	DrawFgMap() {
	const Scene& scene{ g_World.scenes[g_World.curSceneIndex] };
	DrawTexture(scene.fgTexture, scene.dst);
}

void	Character::Draw() {
	const Rectf rect{
		dst.left - g_Camera.pos.x,
		dst.top - g_Camera.pos.y,
		dst.width ,
		dst.height
	};
	DrawTexture(g_NPCTexture, rect, src);
}

void DrawLoadingScreen() {
	if (g_LoadingScreenCooldown > 1.f)
		return;

	SetColor(0.f, 0.f, 0.f, cosf(g_LoadingScreenCooldown * g_Pi));
	FillRect(0.f, 0.f, g_WindowWidth, g_WindowHeight);
}

//              ▀▀                     ██
//  ▄▄▄▄▄       ██  ████▄ ████▄ ██ ██ ▀██▀▀ ▄█▀▀▀
//   ▄▄██       ██  ██ ██ ██ ██ ██ ██  ██   ▀███▄
//  ▄▄▄██  ██   ██▄ ██ ██ ████▀ ▀██▀█  ██   ▄▄▄█▀
//                        ██                      
//                        ▀▀                      

void OnKeyDownEventOnce(SDL_Keycode key) {
	if (key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_UP || key == SDLK_DOWN) {

		// Turn character when key pressed if not in the right direction
		if (!g_Player.isMoving && key == SDLK_LEFT && g_Player.dir.x != -1.f)
			g_Player.dir = Point2f{ -1.f, 0.f };
		else if (!g_Player.isMoving && key == SDLK_RIGHT && g_Player.dir.x != 1.f)
			g_Player.dir = Point2f{ 1.f, 0.f };
		else if (!g_Player.isMoving && key == SDLK_UP && g_Player.dir.y != -1.f)
			g_Player.dir = Point2f{ 0.f, -1.f };
		else if (!g_Player.isMoving && key == SDLK_DOWN && g_Player.dir.y != 1.f)
			g_Player.dir = Point2f{ 0.f, 1.f };

		// updates key pressed if not already pressed
		else {
			if (key == SDLK_LEFT && !g_KeyPressed.left) {
				g_KeyPressed.left = true;
				g_NextKey = key;
			}
			else if (key == SDLK_RIGHT && !g_KeyPressed.right) {
				g_KeyPressed.right = true;
				g_NextKey = key;
			}
			else if (key == SDLK_UP && !g_KeyPressed.up) {
				g_KeyPressed.up = true;
				g_NextKey = key;
			}
			else if (key == SDLK_DOWN && !g_KeyPressed.down) {
				g_KeyPressed.down = true;
				g_NextKey = key;
			}

			UpdateAnimFrameState();
		}
	}
}

void	HandleKeyDownOverworld(SDL_Keycode key) {
	OnKeyDownEventOnce(key);
}

void	HandleKeyUpOverworld(SDL_Keycode key) {
	if (key == SDLK_LEFT) g_KeyPressed.left = false;
	else if (key == SDLK_RIGHT) g_KeyPressed.right = false;
	else if (key == SDLK_UP) g_KeyPressed.up = false;
	else if (key == SDLK_DOWN) g_KeyPressed.down = false;
}

SDL_Keycode	UpdateCurKey() {
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (g_NextKey) {
		SDL_Keycode temp{ g_NextKey };
		g_NextKey = NULL;
		return temp;
	}
	if (pStates[SDL_GetScancodeFromKey(g_CurKey)])
		return g_CurKey;
	return NULL;
}


//		                       ▄▄                   
//                             ██        ██         
//   █  ██      ██ ██ ████▄ ▄████  ▀▀█▄ ▀██▀▀ ▄█▀█▄ 
//   █████      ██ ██ ██ ██ ██ ██ ▄█▀██  ██   ██▄█▀ 
//      ██ ██   ▀██▀█ ████▀ ▀████ ▀█▄██  ██   ▀█▄▄▄ 
//                    ██                            
//                    ▀▀                            

void	UpdateOverworld(float elapsedSec) {
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	const Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	g_Player.UpdatePos(elapsedSec, g_World.moveSpeed, scene.tileSize);
	HandleWalk();
	UpdatePlayerFrameInTime(elapsedSec);
	UpdateAnimTextureFrames();
	UpdateCameraPos(elapsedSec);
	UpdateMapPos(elapsedSec);
	UpdateScene();

	g_FrameTime += elapsedSec;
	g_Time += elapsedSec;
	g_Sounds.collisionCooldown += elapsedSec;
	g_Sounds.grassCooldown += elapsedSec;
	g_LoadingScreenCooldown += elapsedSec;
	g_AnimTextureTime += elapsedSec;
}

void Player::UpdatePos(float elapsedSec, float speed, float maxDist) {
	if (!isMoving)
		return;

	const float
		dx{ dir.x * speed * elapsedSec },
		dy{ dir.y * speed * elapsedSec };

	if (progression + abs(dx + dy) < maxDist) {
		progression += abs(dx + dy);
		dst.left += dx;
		dst.top += dy;
	}
	else {
		dst.left = targetPos.x;
		dst.top = targetPos.y - dst.height / 3; // was g_tilesize / 2, check if ok
		progression = 0.f;
		curTile = targetTile;

		//should create bool for when finished movement (or use progression)
		// and then call outside of here (maybe in handle walk) update curkey and ismoving
		g_CurKey = UpdateCurKey();
		if (!IsWalkable(TargetTileFromKey(curTile, g_CurKey)))
			isMoving = false;
		CheckBattleInGrass();
	}
}

void HandleWalk() {
	if (!g_CurKey)
		g_CurKey = UpdateCurKey();

	if (g_CurKey) {
		g_Player.dir = DirFromKey(g_CurKey);
		g_Player.targetTile = TargetTileFromKey(g_Player.curTile, g_CurKey);
		CheckSoundEffect(g_CurKey);

		if (!IsWalkable(TargetTileFromKey(g_Player.curTile, g_CurKey))) {
			g_Player.targetTile = g_Player.curTile;
			g_CurKey = UpdateCurKey();
		}
		else {
			g_Player.targetPos = PosFromTile(g_Player.targetTile);
			g_Player.isMoving = true;
		}
		// now walk is handled in UpdatePlayerPos
	}
	else
		g_Player.isMoving = false;
	UpdateAnimFrameState();
}

void UpdateCameraPos(float elapsedSec) {
	if (!g_Player.isMoving)
		return;

	const Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	if (g_Player.dst.left + (g_Player.dst.width / 2) - g_WindowWidth / 2 >= 0.f
		&& g_Player.dst.left + (g_Player.dst.width / 2) + g_WindowWidth / 2 <= scene.screenWidth)
	{
		g_Camera.pos.x = g_Player.dst.left + (g_Player.dst.width / 2) - g_WindowWidth / 2;
	}
	if (g_Player.dst.top - g_WindowHeight / 2 >= 0.f
		&& g_Player.dst.top + g_WindowHeight / 2 <= scene.screenHeight)
	{
		g_Camera.pos.y = g_Player.dst.top - g_WindowHeight / 2;
	}
}

void	UpdateMapPos(float elapsedSec) {
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	scene.dst.left = -g_Camera.pos.x;
	scene.dst.top = -g_Camera.pos.y;
}

void	UpdateAnimFrameState() {
	if (g_Player.dir.x == 1.f)
		g_Player.curAnimFrame = g_AnimFrames["walkright"];
	else if (g_Player.dir.x == -1.f)
		g_Player.curAnimFrame = g_AnimFrames["walkleft"];
	else if (g_Player.dir.y == -1.f)
		g_Player.curAnimFrame = g_AnimFrames["walkup"];
	else if (g_Player.dir.y == 1.f)
		g_Player.curAnimFrame = g_AnimFrames["walkdown"];
}

void	UpdatePlayerFrameInTime(float elapsedSec) {
	g_Player.frame.start = g_Player.curAnimFrame.col;
	g_Player.src.left = (g_Player.frame.start + g_Player.frame.index) * g_Player.src.width;

	const float frameRate{ 1.f / 8 };

	if (g_FrameTime > frameRate) {
		g_FrameTime = 0.f;
		if (g_Player.isMoving)
			g_Player.frame.index = (g_Player.frame.index + 1) % g_Player.curAnimFrame.nrFrames;
		else
			g_Player.frame.index = 1;
	}
}

void	UpdateAnimTextureFrames() {
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };
	const float frameRate{ 1.f / 4 };

	if (g_AnimTextureTime > frameRate) {
		g_AnimTextureTime = 0.f;
		scene.animTextureFrames.sea.index = (scene.animTextureFrames.sea.index + 1) % scene.animTextureFrames.sea.end;
		scene.animTextureFrames.rock.index = (scene.animTextureFrames.rock.index + 1) % scene.animTextureFrames.rock.end;
		scene.animTextureFrames.flower.index = (scene.animTextureFrames.flower.index + 1) % scene.animTextureFrames.flower.end;
	}
}

void	CheckSoundEffect(SDL_Keycode key) {
	const float collisionCooldown{ .6f };
	const float grassCooldown{ .3f };
	const int targetTile{ TargetTileFromKey(g_Player.curTile, g_CurKey) };

	if (!IsWalkable(targetTile) && g_Sounds.collisionCooldown > collisionCooldown) {
		Mix_Volume(-1, 64);
		PlaySoundEffect(g_Sounds.collision);
		g_Sounds.collisionCooldown = 0.f;
	}
	else if (IsTallGrass(targetTile) && g_Sounds.grassCooldown > grassCooldown) {
		Mix_Volume(-1, 10);
		PlaySoundEffect(g_Sounds.grass);
		g_Sounds.grassCooldown = 0.f;
	}
}

void UpdateScene() {
	if (!IsGoingOutsideMap())
		return;

	g_LoadingScreenCooldown = 0.f;
	g_Player.isMoving = false;

	const Door door{ GetDoor() };

	g_World.curSceneIndex = door.targetSceneId;
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] }; // new scene

	scene.Init();

	g_Player.curTile = scene.entryPoints[door.targetEntryId];
	g_Player.dst.left = GetCol(g_Player.curTile, scene.nrCols) * scene.tileSize;
	g_Player.dst.top = GetRow(g_Player.curTile, scene.nrCols) * scene.tileSize - scene.tileSize / 2;
	g_Player.targetTile = g_Player.curTile;
	g_Player.targetPos = Point2f{ g_Player.dst.left, g_Player.dst.top };

	InitCamera();
	InitCollisionMap();
	InitAnimTextureMap();
}

void StopWalkingAndReset() {
	g_CurKey = NULL;
	g_NextKey = NULL;
	g_Player.isMoving = false;
	g_Player.targetTile = g_Player.curTile;
	g_Player.targetPos = PosFromTile(g_Player.curTile); // those 3 lines could be a reset pos function
}

void CheckBattleInGrass() {
	if (IsTallGrass(g_Player.curTile)) {
		const int randNum{ rand() % 10 };

		if (!randNum) {
			StopWalkingAndReset();
			TurnOnBattle();
		}
	}
}


//								  ▄▄     
//                       ██   ▀▀  ██       
//   ██▀▀▀▀       ██ ██ ▀██▀▀ ██  ██ ▄█▀▀▀ 
//   ▀▀▀▀██       ██ ██  ██   ██  ██ ▀███▄ 
//   ████▀▀  ██   ▀██▀█  ██   ██▄ ██ ▄▄▄█▀ 



Point2f	GetBottomLeftInRect(const Rectf& rect) {
	return Point2f{ rect.left, rect.top + rect.height };
}

int	TileFromPos(float x, float y) {
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	return GetIndex(static_cast<int>(y / scene.tileSize), static_cast<int>(x / scene.tileSize), scene.nrCols);
}

int	TileFromPos(const Point2f& pos) {
	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	return GetIndex(static_cast<int>(pos.y / scene.tileSize), static_cast<int>(pos.x / scene.tileSize), scene.nrCols);
}

Point2f	PosFromTile(int index) {
	const int nrCols{ g_World.scenes[g_World.curSceneIndex].nrCols };

	return PosFromTile(GetRow(index, nrCols), GetCol(index, nrCols));
}

Point2f	PosFromTile(int row, int col) {
	const float tileSize{ g_World.scenes[g_World.curSceneIndex].tileSize };

	return Point2f{ col * tileSize, row * tileSize };
}

Point2f	DirFromKey(SDL_Keycode key) {
	if (key == SDLK_LEFT)
		return Point2f{ -1.f, 0.f };
	if (key == SDLK_RIGHT)
		return Point2f{ 1.f, 0.f };
	if (key == SDLK_UP)
		return Point2f{ 0.f, -1.f };
	if (key == SDLK_DOWN)
		return Point2f{ 0.f, 1.f };
	return Point2f{ 0.f, 0.f };
}

int		TargetTileFromKey(int curTile, SDL_Keycode key) {
	const int nrCols{ g_World.scenes[g_World.curSceneIndex].nrCols };

	if (key == SDLK_LEFT)
		return curTile - 1;
	if (key == SDLK_RIGHT)
		return curTile + 1;
	if (key == SDLK_UP)
		return curTile - nrCols;
	if (key == SDLK_DOWN)
		return curTile + nrCols;
	return curTile;
}

Point2f		TargetPosFromKey(const Rectf& rect, SDL_Keycode key) {
	return TargetPosFromKey(Point2f{ rect.left, rect.top }, key);
}

Point2f		TargetPosFromKey(const Point2f& pos, SDL_Keycode key) {
	const float tileSize{ g_World.scenes[g_World.curSceneIndex].tileSize };

	if (key == SDLK_LEFT)
		return Point2f{ pos.x - tileSize, pos.y };
	else if (key == SDLK_RIGHT)
		return Point2f{ pos.x + tileSize, pos.y };
	else if (key == SDLK_UP)
		return Point2f{ pos.x, pos.y - tileSize };
	else if (key == SDLK_DOWN)
		return Point2f{ pos.x, pos.y + tileSize };
	return pos;
}

bool	IsPosInCenterX(float pos) {
	const float centerX = g_WindowWidth / 2.f;
	const float epsilon = 10.f;

	return std::abs(pos - centerX) < epsilon;
}

bool	IsPosInCenterY(float pos) {
	const float centerY = g_WindowHeight / 2.f;
	const float epsilon = g_World.scenes[g_World.curSceneIndex].tileSize;

	return std::abs(pos - centerY) < epsilon;
}

void	DrawTiles() {
	const Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	for (int row{}; row < scene.nrRows; ++row) {
		for (int col{}; col < scene.nrCols; ++col) {
			SetColor(g_White);
			DrawRect(
				scene.dst.left + col * scene.tileSize,
				scene.dst.top + row * scene.tileSize,
				scene.tileSize,
				scene.tileSize,
				3.f
			);
		}
	}
}

void	DrawCollisions() {
	const Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	for (int row{}; row < scene.nrRows; ++row) {
		for (int col{}; col < scene.nrCols; ++col) {

			SetColor(!IsWalkable(GetIndex(row, col, scene.nrCols)) ? g_Red : g_White);
			FillRect(
				scene.dst.left + col * scene.tileSize,
				scene.dst.top + row * scene.tileSize,
				scene.tileSize,
				scene.tileSize
			);
		}
	}
}

void PrintTileIndex(float x, float y) {
	std::cout << "Tile index from position [" << x << ", " << y << "] : "
		<< TileFromPos(x + g_Camera.pos.x, y + g_Camera.pos.y) << std::endl;
}

void ErrorLoadMsg(const std::string& path, const std::string& name) {
	std::cout << "Couldn't load " << name << " at " << path << std::endl;
}

bool IsWalkable(int index) {
	return !(g_World.scenes[g_World.curSceneIndex].collisionMap[index] == 1);
}

bool IsTallGrass(int index) {
	return g_World.scenes[g_World.curSceneIndex].collisionMap[index] == 2;
}

Door GetDoor() {
	const int sceneIndex{ g_World.curSceneIndex };
	const Scene& scene{ g_World.scenes[sceneIndex] };
	const int row{ GetRow(g_Player.curTile, scene.nrCols) },
		col{ GetCol(g_Player.curTile, scene.nrCols) };

	if (!sceneIndex) {
		return scene.doors[0]; // east
	}
	else if (sceneIndex == 1) {
		if (row == 0)
			return scene.doors[1]; // north east
		else
			return scene.doors[0]; // west
	}
	else if (sceneIndex == 2) {
		if (row == 0)
			return scene.doors[1]; // north 
		else if (row == scene.nrRows - 1)
			return scene.doors[0]; // south
	}
	return scene.doors[0];
}

// make start of walk animation when collision (changes leg each time)

bool IsGoingOutsideMap() {
	const Scene& scene{ g_World.scenes[g_World.curSceneIndex] };
	const int targetTile{ TargetTileFromKey(g_Player.curTile, g_CurKey) };
	const Point2f targetPos{ PosFromTile(targetTile) };

	const int
		targetRow{ GetRow(targetTile, scene.nrCols) },
		targetCol{ GetCol(targetTile, scene.nrCols) },
		curRow{ GetRow(g_Player.curTile, scene.nrCols) },
		curCol{ GetCol(g_Player.curTile, scene.nrCols) };

	return ((targetRow != curRow && g_CurKey != SDLK_UP && g_CurKey != SDLK_DOWN)
		|| targetRow >= scene.nrRows || targetRow < 0);
}

#pragma endregion ownDeclarations