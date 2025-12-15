#include "pch.h"
#include "Core.h"
#include "Overworld.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>

void TurnOnBattle();

int Scene::nrCols = 0;
int Scene::nrRows = 0;
bool Scene::isCave = false;
float Scene::tileSize = 0.0f;
float Camera::zoom = 4.f;
int NPC::battleRange = 2;
Texture Character::texture = Texture{};

#pragma region ownDeclarations

//								                   ▄▄                  ▄▄ 
//	  ▄▄▄   	▀▀        ▀▀  ██                   ██                  ██ 
//	▄████   	██  ████▄ ██ ▀██▀▀   ▀▀█▄ ████▄ ▄████   ▄█▀█▄ ████▄ ▄████ 
//	  ███   	██  ██ ██ ██  ██    ▄█▀██ ██ ██ ██ ██   ██▄█▀ ██ ██ ██ ██ 
//	  ███ ██	██▄ ██ ██ ██▄ ██    ▀█▄██ ██ ██ ▀████   ▀█▄▄▄ ██ ██ ▀████ 

void	InitOverworld() {

	if (!TextureFromFile("Resources/character.png", Character::texture))
		ErrorLoadMsg("Resources/character.png");
	if (!TextureFromFile("Resources/animated_tiles.png", g_AnimTextures))
		ErrorLoadMsg("Resources/animated_tiles.png");
	if (!TextureFromFile("Resources/water_shadow.png", g_WaterShadowTexture))
		ErrorLoadMsg("Resources/water_shadow.png");

	InitScenes();

	Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	InitAnimFrames();
	InitCharacters(scene);
	g_Camera.Init(scene);
	InitAudioFiles();
}

void	InitScenes() {
	Scene* pScene{ &g_World.scenes[0] };

	pScene->name = "three_island";
	pScene->entryPoints["Spawn"] = 183;
	pScene->entryPoints["East"] = 215;
	pScene->nrDoors = 2;
	pScene->doors[0] = Door{ "East", 1, "West" };
	pScene->doors[1] = Door{ "South", 0, "East" };
	pScene->LoadStatic();
	pScene->Init();
	pScene->InitCollisionMap();
	pScene->InitAnimTextureMap();

	pScene = &g_World.scenes[1];
	pScene->name = "bridge";
	pScene->entryPoints["Spawn"] = 983;
	pScene->entryPoints["West"] = 752;
	pScene->entryPoints["NorthEast"] = 81;
	pScene->nrDoors = 2;
	pScene->doors[0] = Door{ "West", 0, "East" };
	pScene->doors[1] = Door{ "NorthEast", 2, "South" };
	pScene->LoadStatic();
	pScene->Init();
	pScene->InitCollisionMap();
	pScene->InitAnimTextureMap();

	pScene = &g_World.scenes[2];

	pScene->name = "kindle";
	pScene->entryPoints["South"] = 2844;
	pScene->entryPoints["North"] = 11;
	pScene->entryPoints["Spawn"] = 11;
	pScene->nrDoors = 2;
	pScene->doors[0] = Door{ "South", 1, "NorthEast" };
	pScene->doors[1] = Door{ "North", 3, "South" };
	pScene->LoadStatic();
	pScene->Init();
	pScene->InitCollisionMap();
	pScene->InitAnimTextureMap();

	pScene = &g_World.scenes[3];

	pScene->name = "mountain";
	pScene->entryPoints["South"] = 3050;
	pScene->entryPoints["North"] = 428;
	pScene->entryPoints["Spawn"] = 428;
	pScene->nrDoors = 2;
	pScene->doors[0] = Door{ "South", 2, "North" };
	pScene->doors[1] = Door{ "North", 4, "South" };
	pScene->LoadStatic();
	pScene->Init();
	pScene->InitCollisionMap();
	pScene->InitAnimTextureMap();

	pScene = &g_World.scenes[4];

	pScene->name = "cave";
	pScene->entryPoints["South"] = 1881;
	pScene->entryPoints["North"] = 375;
	pScene->entryPoints["Spawn"] = 375;
	pScene->nrDoors = 2;
	pScene->doors[0] = Door{ "South", 3, "North" };
	pScene->doors[1] = Door{ "North", 5, "South" };
	pScene->LoadStatic();
	pScene->Init();
	pScene->InitCollisionMap();
	pScene->InitAnimTextureMap();

	pScene = &g_World.scenes[5];

	pScene->name = "spa";
	pScene->entryPoints["South"] = 1093;
	pScene->entryPoints["Spawn"] = 345;
	pScene->nrDoors = 1;
	pScene->doors[0] = Door{ "South", 4, "North" };
	pScene->LoadStatic();
	pScene->Init();
	pScene->InitCollisionMap();
	pScene->InitAnimTextureMap();

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
	Camera::zoom = name == "cave" ? 3.f : 4.f;
	tileSize = 16.f * Camera::zoom;

	screenWidth = texture.width * Camera::zoom;
	screenHeight = texture.height * Camera::zoom;
	startOffset.x = std::max(0.f, (g_WindowWidth - screenWidth) / 2);
	startOffset.y = std::max(0.f, (g_WindowHeight - screenHeight) / 2);

	dst = Rectf{ 0.f, 0.f, screenWidth, screenHeight };
	nrCols = static_cast<int>(screenWidth / tileSize);
	nrRows = static_cast<int>(screenHeight / tileSize);
}

void	Scene::InitCollisionMap() {

	std::string path{ collisionMapPath };
	collisionMapSize = nrCols * nrRows;
	collisionMap = new int[collisionMapSize];

	std::ifstream	file(path);
	if (!file)
		ErrorLoadMsg(path);

	int index{};
	char ch{};
	while (file.get(ch) && index < collisionMapSize) {
		if (ch == '0' || ch == '1' || ch == '2' || ch == '3') {
			collisionMap[index] = ch - '0';
			++index;
		}
	}
	//Print2DArray(collisionMap, collisionMapSize, nrCols);
}

void	Scene::InitAnimTextureMap() {
	std::string path{ animTextureMapPath };

	animTextureMapSize = nrCols * nrRows;
	animTextureMap = new char[animTextureMapSize];

	std::ifstream	file(path);
	if (!file)
		ErrorLoadMsg(path);

	int index{};
	char ch{};
	while (file.get(ch) && index < animTextureMapSize) {
		if (std::strchr("0123mrfs", ch)) {
			animTextureMap[index] = ch;
			++index;
		}
	}
	//Print2DArray(animTextureMap, animTextureMapSize, nrCols);
}

void InitAnimFrames() {
	g_AnimFrames["walkdown"] = AnimFrame{ 0, 0, 4 };
	g_AnimFrames["walkup"] = AnimFrame{ 0, 4, 4 };
	g_AnimFrames["walkleft"] = AnimFrame{ 0, 8, 4 };
	g_AnimFrames["walkright"] = AnimFrame{ 0, 12, 4 };
	g_AnimFrames["driveleft"] = AnimFrame{ 1, 0, 3 };
	g_AnimFrames["driveright"] = AnimFrame{ 1, 3, 3 };
}

void InitCharacters(const Scene& scene) {
	g_Player.Init(scene.entryPoints.at("Spawn"));
	//g_NPC[0].Init(199, Rectf{ 0.f, 24.f, 24.f, 24.f }); // moto guy
	g_NPC[0].Init(1, 817, Rectf{ 0.f, 48.f, 16.f, 24.f });
	g_NPC[1].Init(5, 257, Rectf{ 0.f, 72.f, 18.f, 24.f }, false);
	InitNPCPath(g_NPC[0], g_NPC[0].curTile, g_NPC[0].dir);

}

void Player::Init(int entryPoint) {
	curTile = entryPoint;
	std::cout << "tile size : " << Scene::tileSize << std::endl;
	dst = Rectf{
		GetCol(curTile, Scene::nrCols) * Scene::tileSize,
		GetRow(curTile, Scene::nrCols) * Scene::tileSize - Scene::tileSize / 2,
		Scene::tileSize,
		Scene::tileSize * 1.5f
	};
	targetTile = curTile;
	curAnimFrame = g_AnimFrames["walkdown"];
	frame.start = curAnimFrame.col;
	frame.index = 1;
	src.left = (frame.start + frame.index) * src.width;
	dir = Point2f{ 0.f, 1.f };
}

void NPC::Init(int sceneIndex, int tile, const Rectf& dimensions, bool isMoving) {
	startTile = tile;
	curTile = tile;
	targetTile = tile;
	this->sceneIndex = sceneIndex;
	if (sceneIndex == 5) isGod = true;
	dst = Rectf{
		GetCol(curTile, Scene::nrCols) * Scene::tileSize,
		GetRow(curTile, Scene::nrCols) * Scene::tileSize - Scene::tileSize / 2,
		dimensions.width * Camera::zoom,
		dimensions.height * Camera::zoom
	};
	//isMoto = true; // pass as argument
	isLooping = true;
	curAnimFrame = g_AnimFrames["walkdown"];
	if (isMoto)
		curAnimFrame = g_AnimFrames["driveright"];
	frame.start = 0;
	frame.index = 1;
	src = Rectf{
		(frame.start + frame.index) * dimensions.width,
		dimensions.top,
		dimensions.width,
		dimensions.height
	};
	this->isMoving = isMoving;
	dir = Point2f{ 1.f, 0.f };
}

void	Camera::Init(const Scene& scene) {
	Camera::zoom = scene.name == "cave" ? 3.f : 4.f;
	Point2f	posPlayerMid{ g_Player.dst.left + (g_Player.dst.width / 2) - g_WindowWidth / 2, g_Player.dst.top - g_WindowHeight / 2 };

	if (posPlayerMid.x < 0) {
		posPlayerMid.x = 0;
	}
	else if (posPlayerMid.x + g_WindowWidth > scene.dst.width) {
		posPlayerMid.x = scene.dst.width - g_WindowWidth;
	}
	if (posPlayerMid.y < 0) {
		posPlayerMid.y = 0;
	}
	else if (posPlayerMid.y + g_WindowHeight > scene.dst.height) {
		posPlayerMid.y = scene.dst.height - g_WindowHeight;
	}

	pos = posPlayerMid;
}

void InitAudioFiles() {
	LoadSoundEffect(g_Sounds.collision, "../Resources/collision.wav");
	LoadSoundEffect(g_Sounds.grass, "../Resources/grass.wav");
	LoadMusic(g_Sounds.overworldMusic, "../Resources/music_overworld.ogg");
	LoadMusic(g_Sounds.caveMusic, "../Resources/music_cave.ogg");
}

//		END

void	FreeOverworld() {
	DeleteTexture(Character::texture);
	DeleteTexture(g_WaterShadowTexture);
	DeleteTexture(g_AnimTextures);
	Mix_FreeChunk(g_Sounds.collision);
	Mix_FreeChunk(g_Sounds.grass);
	Mix_FreeMusic(g_Sounds.overworldMusic);
	Mix_FreeMusic(g_Sounds.caveMusic);

	for (int index{}; index < g_NrScenes; ++index) {
		DeleteTexture(g_World.scenes[index].texture);
		DeleteTexture(g_World.scenes[index].fgTexture);
		delete g_World.scenes[index].collisionMap;
		delete g_World.scenes[index].animTextureMap;
	}
}

//			         ▄▄                     
//			         ██                     
//	▄▀▀██▄ 	      ▄████ ████▄  ▀▀█▄ ██   ██ 
//	  ▄██▀ 	      ██ ██ ██ ▀▀ ▄█▀██ ██ █ ██ 
//	▄███▄▄▄	 ██   ▀████ ██    ▀█▄██  ██▀██  
//

void	DrawOverworld() {
	const Scene& scene{ GetScene() };

	scene.DrawSea();
	scene.DrawRocks();
	scene.DrawMap();
	scene.DrawFlowers();
	scene.DrawSmoke();
	//DrawCollisions();
	//DrawTiles()
	//DrawCurrentAndTargetTiles();
	for (NPC& npc : g_NPC) {
		if (npc.sceneIndex == g_World.curSceneIndex) {
			npc.Draw();
			//npc.DrawPath();
		}
	}
	g_Player.Draw();
	if (scene.fgTexture.height)
		scene.DrawFgMap();
	DrawLoadingScreen();
	DrawBlinkBattle();

}

void Scene::DrawSea() const {
	const float size{ g_AnimTextures.width / animTextureFrames.smoke.end };

	for (int index{}; index < animTextureMapSize; ++index) {
		if (animTextureMap[index] != 'm' && animTextureMap[index] != 'r') continue;

		const Rectf dst{
			GetCol(index, nrCols) * tileSize - g_Camera.pos.x,
			GetRow(index, nrCols) * tileSize - g_Camera.pos.y,
			tileSize, tileSize
		};
		const Rectf src{ animTextureFrames.sea.index * size, 0.f, size, size };

		DrawTexture(g_AnimTextures, dst, src);
	}
}

void Scene::DrawRocks() const {
	const float size{ g_AnimTextures.width / animTextureFrames.smoke.end };

	for (int index{}; index < animTextureMapSize; ++index) {
		if (animTextureMap[index] != 'r') continue;

		Rectf dst{
			GetCol(index, nrCols) * tileSize - g_Camera.pos.x,
			GetRow(index, nrCols) * tileSize - g_Camera.pos.y,
			tileSize * 2, tileSize * 2
		};
		const Rectf src{ animTextureFrames.rock.index * size, size, size, size };

		DrawTexture(g_WaterShadowTexture, dst);

		dst.left += tileSize / 2;
		dst.top += tileSize / 2;
		dst.width /= 2;
		dst.height /= 2;
		DrawTexture(g_AnimTextures, dst, src);
	}
}

void Scene::DrawFlowers() const {
	const float size{ g_AnimTextures.width / animTextureFrames.smoke.end };

	for (int index{}; index < animTextureMapSize; ++index) {
		if (animTextureMap[index] != 'f') continue;

		const Rectf dst{
			GetCol(index, nrCols) * tileSize - g_Camera.pos.x,
			GetRow(index, nrCols) * tileSize - g_Camera.pos.y,
			tileSize, tileSize
		};
		const Rectf src{ animTextureFrames.flower.index * size, 2 * size, size, size };

		DrawTexture(g_AnimTextures, dst, src);
	}
}

void Scene::DrawSmoke() const {
	const float size{ g_AnimTextures.width / animTextureFrames.smoke.end };

	for (int index{}; index < animTextureMapSize; ++index) {
		if (animTextureMap[index] != 's') continue;

		const Rectf dst{
			GetCol(index, nrCols) * tileSize - g_Camera.pos.x,
			GetRow(index, nrCols) * tileSize - g_Camera.pos.y - 10.f,
			tileSize, tileSize
		};
		const int newIndex{ (animTextureFrames.smoke.index + (index % animTextureFrames.smoke.end)) % animTextureFrames.smoke.end };
		const Rectf src{ newIndex * size, 3 * size, size, size };

		DrawTexture(g_AnimTextures, dst, src);
	}
}

void	Scene::DrawMap() const {
	DrawTexture(texture, dst);
}

void	Scene::DrawFgMap() const {
	DrawTexture(fgTexture, dst);
}

void	Character::Draw() const {
	const Rectf rect{
		dst.left - g_Camera.pos.x + (isGod ? Scene::tileSize / 2 : 0.f),
		dst.top - g_Camera.pos.y,
		dst.width,
		dst.height
	};
	DrawTexture(texture, rect, src);
}

void DrawLoadingScreen() {
	if (g_LoadingScreenCooldown > 1.f)
		return;

	SetColor(0.f, 0.f, 0.f, cosf(g_LoadingScreenCooldown * g_Pi));
	FillRect(0.f, 0.f, g_WindowWidth, g_WindowHeight);
}

void DrawBlinkBattle() {
	if (!g_Camera.isBlinking) return;

	const float pulsation{ 15.7f };

	SetColor(0.f, 0.f, 0.f, sinf(pulsation * g_BlinkTime - (pulsation / 2)) / 2 + .5f);
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

			g_Player.UpdateAnimFrameState();
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
	Scene& scene{ GetScene() };

	if (!g_Player.isFrozen) {
		HandlePlayerWalk();
		g_Player.UpdateFrame(elapsedSec);
		g_Player.UpdatePos(elapsedSec, g_World.moveSpeed, Scene::tileSize);
	}

	for (NPC& npc : g_NPC) {
		if (npc.isMoto) {
			npc.Drive();
			npc.UpdatePos(elapsedSec, g_World.moveSpeed, Scene::tileSize);
			npc.UpdateFrame(elapsedSec, 1 / 8.f);
		}
		else {
			//if (npc.isWalkingTowardsPlayer) npc.Walk();
			//else 
			npc.FollowPath();
			npc.UpdatePos(elapsedSec, g_World.moveSpeed / 3, Scene::tileSize);
			npc.UpdateFrame(elapsedSec, 1 / 4.f);
		}
		npc.EngageBattle(g_Player);
	}

	scene.UpdateAnimTextureFrames();
	g_Camera.UpdatePos(elapsedSec, scene, g_Player);
	scene.UpdateMapPos(elapsedSec, g_Camera);
	UpdateScene(g_Camera, g_Player);

	g_Time += elapsedSec;
	g_BlinkTime += elapsedSec;
	g_Sounds.collisionCooldown += elapsedSec;
	g_Sounds.grassCooldown += elapsedSec;
	g_LoadingScreenCooldown += elapsedSec;
	g_AnimTextureTime += elapsedSec;
	g_SmokeTime += elapsedSec;
}

void Character::UpdatePos(float elapsedSec, float speed, float maxDist) {
	if (!isMoving)
		return;

	const float
		dx{ dir.x * speed * elapsedSec },
		dy{ dir.y * speed * elapsedSec };

	if (stepProgress + abs(dx + dy) < maxDist) {
		stepProgress += abs(dx + dy);
		dst.left += dx;
		dst.top += dy;
	}
	else {
		dst.left = targetPos.x;
		dst.top = targetPos.y - dst.height / 3;
		stepProgress = 0.f;
		curTile = targetTile;
	}
}

void HandlePlayerWalk() {
	if (!g_CurKey)
		g_CurKey = UpdateCurKey();

	if (g_Player.isMoving && g_Player.stepProgress == 0.f) {
		g_CurKey = UpdateCurKey();
		const int targetTemp{ TargetTileFromKey(g_Player.curTile, g_CurKey) };

		if (!IsWalkable(targetTemp) || IsNPCOnTile(targetTemp))
			g_Player.isMoving = false;
		CheckBattleInGrass();
	}

	if (g_CurKey) {
		g_Player.dir = DirFromKey(g_CurKey);
		g_Player.targetTile = TargetTileFromKey(g_Player.curTile, g_CurKey);
		CheckSoundEffect(g_CurKey);

		// stop if trying to walk on illegal tiles
		if (!IsWalkable(g_Player.targetTile) || IsNPCOnTile(g_Player.targetTile)) {
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
	g_Player.UpdateAnimFrameState();
}

void NPC::Walk() {
	int nextTile{ TargetTileFromDir(curTile, dir) };

	if (IsWalkable(targetTile) && !IsPlayerOnTile(targetTile)) {
		targetTile = nextTile;
		targetPos = PosFromTile(targetTile);
		nextTile = TargetTileFromDir(curTile, dir);
		isMoving = true;
	}
	else isMoving = false;
	UpdateAnimFrameState();
}

void NPC::Drive() {
	if (stepProgress == 0.f) {
		const int targetTemp{ TargetTileFromDir(curTile, dir) };
		if (IsWalkable(targetTemp) && !IsPlayerOnTile(targetTemp)) {
			targetTile = targetTemp;
			targetPos = PosFromTile(targetTile);
			isMoving = true;
		}
		else
			isMoving = false;
	}
	UpdateMotoFrame();
}

void InitNPCPath(NPC& npc, int tile, Point2f dir) {
	// basic square path
	npc.isLooping = true;
	for (int index{}; index < 4; ++index) {
		npc.path[index * 2] = tile;
		npc.pathDir[index * 2] = dir;
		tile = TargetTileFromDir(tile, dir);
		npc.path[index * 2 + 1] = tile;
		npc.pathDir[index * 2 + 1] = dir;
		Turn90(dir);
		tile = TargetTileFromDir(tile, dir);
		npc.pathLength = index * 2 + 2;
	}
	// add dijkstra path
}

void NPC::FollowPath() {
	if (stepProgress == 0.f) {
		const int targetTemp{ path[pathIndex + 1 > pathLength && isLooping ? 0 : pathIndex + 1] };

		dir = pathDir[pathIndex + 1 > pathLength && isLooping ? 0 : pathIndex + 1];
		if (IsWalkable(targetTemp) && (!IsPlayerOnTile(targetTemp) || isWalkingTowardsPlayer)) {
			pathIndex++;
			if (pathIndex > pathLength && isLooping) pathIndex = 0;
			else if (pathIndex > pathLength) return; // should stop drawing ?

			targetTile = targetTemp;
			targetPos = PosFromTile(targetTile);
			isMoving = true;
		}
		else
			isMoving = false;
	}
	if (isMoto) UpdateMotoFrame();
	else UpdateAnimFrameState();
}

void Camera::UpdatePos(float elapsedSec, const Scene& scene, const Player& player) {
	if (player.isMoving) {
		if (player.dst.left + (player.dst.width / 2) - g_WindowWidth / 2 >= 0.f
			&& player.dst.left + (player.dst.width / 2) + g_WindowWidth / 2 <= scene.screenWidth)
		{
			pos.x = player.dst.left + (player.dst.width / 2) - g_WindowWidth / 2;
		}
		if (player.dst.top - g_WindowHeight / 2 >= 0.f
			&& player.dst.top + g_WindowHeight / 2 <= scene.screenHeight)
		{
			pos.y = player.dst.top - g_WindowHeight / 2;
		}
	}
}

void	Scene::UpdateMapPos(float elapsedSec, const Camera& camera) {
	dst.left = -camera.pos.x;
	dst.top = -camera.pos.y;
}

void	NPC::UpdateMotoFrame() {
	if (!isMoto) return;
	if (dir.x == 1.f)
		curAnimFrame = g_AnimFrames["driveright"];
	else if (dir.x == -1.f)
		curAnimFrame = g_AnimFrames["driveleft"];
}

void	Character::UpdateAnimFrameState() {
	if (dir.x == 1.f)
		curAnimFrame = g_AnimFrames["walkright"];
	else if (dir.x == -1.f)
		curAnimFrame = g_AnimFrames["walkleft"];
	else if (dir.y == -1.f)
		curAnimFrame = g_AnimFrames["walkup"];
	else if (dir.y == 1.f)
		curAnimFrame = g_AnimFrames["walkdown"];
}

void	Character::UpdateFrame(float elapsedSec, float frameRate) {
	frame.start = curAnimFrame.col;
	src.left = (frame.start + frame.index) * src.width;

	if (frameTime > frameRate) {
		frameTime = 0.f;
		if (isMoving)
			frame.index = (frame.index + 1) % curAnimFrame.nrFrames;
		else
			frame.index = 1;
	}
	frameTime += elapsedSec;
}

void	Scene::UpdateAnimTextureFrames() {
	const float frameRate{ 1.f / 4 };

	if (g_AnimTextureTime > frameRate) {
		g_AnimTextureTime = 0.f;
		animTextureFrames.sea.index = (animTextureFrames.sea.index + 1) % animTextureFrames.sea.end;
		animTextureFrames.rock.index = (animTextureFrames.rock.index + 1) % animTextureFrames.rock.end;
		animTextureFrames.flower.index = (animTextureFrames.flower.index + 1) % animTextureFrames.flower.end;
		animTextureFrames.smoke.index = (animTextureFrames.smoke.index + 1) % animTextureFrames.smoke.end;	
	}
}

void	PlayMusicOverworld() {
	if (Scene::isCave) PlayMusic(g_Sounds.caveMusic);
	else PlayMusic(g_Sounds.overworldMusic);
}

void	CheckSoundEffect(SDL_Keycode key) {
	const float collisionCooldown{ .6f };
	const float grassCooldown{ .3f };
	const int targetTile{ TargetTileFromKey(g_Player.curTile, g_CurKey) };

	if ((!IsWalkable(targetTile) || IsNPCOnTile(targetTile)) && g_Sounds.collisionCooldown > collisionCooldown) {
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

void UpdateScene(Camera& camera, Player& player) {
	Scene* pScene{ &GetScene() };
	if (!IsGoingOutsideMap())
		return;

	g_LoadingScreenCooldown = 0.f;
	player.isMoving = false;

	const Door door{ GetDoor() };

	g_World.curSceneIndex = door.targetSceneId;

	pScene = &g_World.scenes[g_World.curSceneIndex];
	pScene->Init();

	player.curTile = pScene->entryPoints[door.targetEntryId];
	player.dst.left = GetCol(player.curTile, Scene::nrCols) * Scene::tileSize;
	player.dst.top = GetRow(player.curTile, Scene::nrCols) * Scene::tileSize - Scene::tileSize / 2;
	player.dst.width = Scene::tileSize;
	player.dst.height = Scene::tileSize * 1.5f;
	player.targetTile = player.curTile;
	player.targetPos = Point2f{ player.dst.left, player.dst.top };

	camera.Init(*pScene);

}

void Character::StopWalkingAndReset(int& curKey, int& nextKey) {
	curKey = NULL;
	nextKey = NULL;
	isMoving = false;
	targetTile = curTile;
	targetPos = PosFromTile(curTile); // those 3 lines could be a reset pos function ?
}

void CheckBattleInGrass() {
	if (IsTallGrass(g_Player.curTile)) {
		const int randNum{ rand() % 10 };

		if (!randNum) {
			g_Player.StopWalkingAndReset(g_CurKey, g_NextKey);
			TurnOnBattle();
		}
	}
}

bool NPC::IsPlayerInRange(const Player& player) {
	int tile{ curTile };
	for (int index{}; index < battleRange; ++index) {
		tile = TargetTileFromDir(tile, dir);

		if (IsPlayerOnTile(tile)) return true;
	}

	if (isGod) {
		const Point2f npcPos{ PosFromTile(curTile) }, playerPos{ PosFromTile(player.curTile) };
		const float dx{ abs(npcPos.x - playerPos.x) }, dy{ abs(npcPos.y - playerPos.y) };
		if (dx <= 64.f && dy <= 192.f)
			return true;
	}
	return false;
}

void NPC::EngageBattle(Player& player) {
	if (hasBattled || !IsPlayerInRange(player)) return;

	if (!player.isFrozen) {
		player.StopWalkingAndReset(g_CurKey, g_NextKey);
		player.isFrozen = true;
		isWalkingTowardsPlayer = true;
	}

	if ((IsPlayerOnTile(targetTile) || isGod) && !g_Camera.isBlinking) {
		isMoving = false;
		g_BlinkTime = 0.f;
		g_Camera.isBlinking = true;
		g_Camera.backupPos = g_Camera.pos;
	}
	else if ((IsPlayerOnTile(targetTile) || isGod) && g_Camera.isBlinking && g_BlinkTime > .8f) {
		TurnOnBattle();
		g_Camera.isBlinking = false;
		g_Camera.pos = g_Camera.backupPos;
	}
}

void	EndBattleOverworld() {
	for (NPC& npc : g_NPC) {
		if (npc.isWalkingTowardsPlayer) {
			npc.hasBattled = true;
			npc.isWalkingTowardsPlayer = false;
			npc.isMoving = true;
			g_Player.isFrozen = false;
		}
	}
}

//								  ▄▄     
//                       ██   ▀▀  ██       
//   ██▀▀▀▀       ██ ██ ▀██▀▀ ██  ██ ▄█▀▀▀ 
//   ▀▀▀▀██       ██ ██  ██   ██  ██ ▀███▄ 
//   ████▀▀  ██   ▀██▀█  ██   ██▄ ██ ▄▄▄█▀ 



Point2f	GetBottomLeftRect(const Rectf& rect) {
	return Point2f{ rect.left, rect.top + rect.height };
}

int	TileFromPos(float x, float y) {
	return GetIndex(static_cast<int>(y / Scene::tileSize), static_cast<int>(x / Scene::tileSize), Scene::nrCols);
}

int	TileFromPos(const Point2f& pos) {
	return GetIndex(static_cast<int>(pos.y / Scene::tileSize), static_cast<int>(pos.x / Scene::tileSize), Scene::nrCols);
}

Point2f	PosFromTile(int index) {
	return PosFromTile(GetRow(index, Scene::nrCols), GetCol(index, Scene::nrCols));
}

Point2f	PosFromTile(int row, int col) {
	return Point2f{ col * Scene::tileSize, row * Scene::tileSize };
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
	if (key == SDLK_LEFT)
		return curTile - 1;
	if (key == SDLK_RIGHT)
		return curTile + 1;
	if (key == SDLK_UP)
		return curTile - Scene::nrCols;
	if (key == SDLK_DOWN)
		return curTile + Scene::nrCols;
	return curTile;
}

int		TargetTileFromDir(int curTile, const Point2f& dir) {
	if (dir == Point2f{ -1.f, 0.f })
		return  curTile - 1;
	if (dir == Point2f{ 1.f, 0.f })
		return  curTile + 1;
	if (dir == Point2f{ 0.f, -1.f })
		return  curTile - Scene::nrCols;
	if (dir == Point2f{ 0.f, 1.f })
		return curTile + Scene::nrCols;
	return curTile;
}

Point2f		TargetPosFromKey(const Rectf& rect, SDL_Keycode key) {
	return TargetPosFromKey(Point2f{ rect.left, rect.top }, key);
}

Point2f		TargetPosFromKey(const Point2f& pos, SDL_Keycode key) {
	if (key == SDLK_LEFT)
		return Point2f{ pos.x - Scene::tileSize, pos.y };
	else if (key == SDLK_RIGHT)
		return Point2f{ pos.x + Scene::tileSize, pos.y };
	else if (key == SDLK_UP)
		return Point2f{ pos.x, pos.y - Scene::tileSize };
	else if (key == SDLK_DOWN)
		return Point2f{ pos.x, pos.y + Scene::tileSize };
	return pos;
}

bool	IsPosInCenterX(float pos) {
	const float centerX = g_WindowWidth / 2.f;
	const float epsilon = 10.f;

	return std::abs(pos - centerX) < epsilon;
}

bool	IsPosInCenterY(float pos) {
	const float centerY = g_WindowHeight / 2.f;
	const float epsilon = Scene::tileSize;

	return std::abs(pos - centerY) < epsilon;
}

void	DrawTiles() {
	const Scene& scene{ g_World.scenes[g_World.curSceneIndex] };

	for (int row{}; row < Scene::nrRows; ++row) {
		for (int col{}; col < Scene::nrCols; ++col) {
			SetColor(g_White);
			DrawRect(
				scene.dst.left + col * Scene::tileSize,
				scene.dst.top + row * Scene::tileSize,
				Scene::tileSize,
				Scene::tileSize,
				3.f
			);
		}
	}
}

void	DrawCollisions() {
	const Scene& scene{ GetScene() };

	for (int row{}; row < Scene::nrRows; ++row) {
		for (int col{}; col < Scene::nrCols; ++col) {

			SetColor(!IsWalkable(GetIndex(row, col, Scene::nrCols)) ? g_Red : g_White);
			FillRect(
				scene.dst.left + col * Scene::tileSize,
				scene.dst.top + row * Scene::tileSize,
				Scene::tileSize,
				Scene::tileSize
			);
		}
	}
}

void DrawCurrentAndTargetTiles() {
	const Scene& scene{ GetScene() };

	SetColor(g_Red); // target
	for (const NPC& npc : g_NPC) {
		FillRect(PosFromTile(npc.targetTile).x - g_Camera.pos.x, PosFromTile(npc.targetTile).y - g_Camera.pos.y, Scene::tileSize, Scene::tileSize);
	}
	FillRect(PosFromTile(g_Player.targetTile).x - g_Camera.pos.x, PosFromTile(g_Player.targetTile).y - g_Camera.pos.y, Scene::tileSize, Scene::tileSize);

	SetColor(g_Blue); // current
	for (const NPC& npc : g_NPC) {
		FillRect(PosFromTile(npc.curTile).x - g_Camera.pos.x, PosFromTile(npc.curTile).y - g_Camera.pos.y, Scene::tileSize, Scene::tileSize);
	}
	FillRect(PosFromTile(g_Player.curTile).x - g_Camera.pos.x, PosFromTile(g_Player.curTile).y - g_Camera.pos.y, Scene::tileSize, Scene::tileSize);
}

void NPC::DrawPath() const {
	for (int i{}; i < 100; ++i)
	{
		SetColor(g_Red);
		FillRect(PosFromTile(path[i]).x - g_Camera.pos.x, PosFromTile(path[i]).y - g_Camera.pos.y, Scene::tileSize, Scene::tileSize);
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
	const Scene& scene{ GetScene() };

	return (index >= 0 && index < scene.collisionMapSize && scene.collisionMap[index] != 1);
}

bool IsPlayerOnTile(int index) {
	return (g_Player.curTile == index || g_Player.targetTile == index);
}

bool IsNPCOnTile(int index) {
	for (const NPC& npc : g_NPC) {
		if (npc.sceneIndex == g_World.curSceneIndex && (npc.curTile == index || npc.targetTile == index)) {
			return true;
		}
	}
	return false;
}

bool IsTallGrass(int index) {
	const Scene& scene{ GetScene() };

	return index >= 0 && index < scene.collisionMapSize && scene.collisionMap[index] == 2;
}

Scene& GetScene() {
	return g_World.scenes[g_World.curSceneIndex];
}

Door GetDoor() {
	const int sceneIndex{ g_World.curSceneIndex };
	const int row{ GetRow(g_Player.curTile, Scene::nrCols) },
		col{ GetCol(g_Player.curTile, Scene::nrCols) };
	const Scene& scene{ g_World.scenes[sceneIndex] };

	switch (sceneIndex)
	{
	case 0:
		if (col == Scene::nrCols - 1) return scene.doors[0];		// east
		else return scene.doors[1];									// south
	case 1:
		if (row == 0) return scene.doors[1];						// north east
		else return scene.doors[0];									// west
	case 2:
		if (row == 0) return scene.doors[1];						// north 
		else if (row == Scene::nrRows - 1) return scene.doors[0];	// south
	case 3:
		if (row == Scene::nrRows - 1) return scene.doors[0];		// south
		else return scene.doors[1];									// north
	case 4:
		if (row < Scene::nrRows / 2) return scene.doors[1];			// north
		else return scene.doors[0];									// south
	default:
		return scene.doors[0];
	}
}
// make start of walk animation when collision (changes leg each time)

bool IsGoingOutsideMap() {
	const int targetTile{ TargetTileFromKey(g_Player.curTile, g_CurKey) };
	const Point2f targetPos{ PosFromTile(targetTile) };

	const int
		targetRow{ GetRow(targetTile, Scene::nrCols) },
		targetCol{ GetCol(targetTile, Scene::nrCols) },
		curRow{ GetRow(g_Player.curTile, Scene::nrCols) },
		curCol{ GetCol(g_Player.curTile, Scene::nrCols) };

	//std::cout << "\n targetTile " << targetTile << std::endl;
	const int typeTile{ g_World.scenes[g_World.curSceneIndex].collisionMap[targetTile] };

	return ((targetRow != curRow && g_CurKey != SDLK_UP && g_CurKey != SDLK_DOWN)
		|| targetRow >= Scene::nrRows || targetRow < 0 || targetTile < 0 || typeTile == 3);
}

int TileDist(int start, int end, float tileSize) {
	Point2f startPos{ PosFromTile(start) }, endPos{ PosFromTile(end) };

	return  static_cast<int>(abs(startPos.x - endPos.x + startPos.y - endPos.y) / tileSize);
}

void Turn(Point2f& dir, float angle) {
	dir = Point2f{ dir.x * cosf(angle) - dir.y * sinf(angle), dir.x * sinf(angle) + dir.y * cosf(angle) };
}

void Turn90(Point2f& dir) {
	dir = Point2f{ -dir.y,  dir.x };
}

#pragma endregion ownDeclarations