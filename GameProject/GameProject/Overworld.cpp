#include "pch.h"
#include "Core.h"
#include "Overworld.h"
#include <iostream>
#include <windows.h>
#include <mmsystem.h>


#pragma region ownDeclarations

//		INITIALIZATIONS

void	InitOverworld() {
	InitCamera();
	InitScenes();
	InitAnimFrames();
	InitCharacter();
}

void	InitCamera() {
	g_Camera.zoom = 2.f;
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
	pScene->startOffset = g_TileSize / 4;
	pScene->dst = Rectf{ 0.f, 0.f, screenWidth, screenHeight };
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
		g_TileSize * (g_Camera.zoom / 2),
		g_TileSize * 1.5f * (g_Camera.zoom / 2)
	};
	g_Character.curAnimFrame = g_AnimFrames["idledown"];
	g_Character.dir = Point2f{ 0.f, 1.f };
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

//		UPDATE

void	UpdateOverworld(float elapsedSec) {
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	UpdateCharacterDirSpeed(pStates, elapsedSec);
	UpdateCharacterFrame(pStates, elapsedSec);
	UpdateMapPos(elapsedSec);
	UpdateCameraPos(elapsedSec);

	g_FrameTime += elapsedSec;
}

void UpdateCameraPos(float elapsedSec) {
	const Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };
	const float screenWidth{ pScene->texture.width * g_Camera.zoom },
		screenHeight{ pScene->texture.height * g_Camera.zoom };
	const float dX{ g_Character.dir.x * g_Character.vx * elapsedSec },
		dY{ g_Character.dir.y * g_Character.vy * elapsedSec };

	g_Character.dst.left += dX;
	g_Character.dst.top += dY;

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

void	UpdateCharacterDirSpeed(const Uint8* pStates, float elapsedSec) {
	if (pStates[SDL_SCANCODE_RIGHT] && !IsBetweenTiles()) {
		g_Character.vx = g_MoveSpeed;
		g_Character.dir = Point2f{ 1.f, 0.f };
	}
	else if (pStates[SDL_SCANCODE_LEFT] && !IsBetweenTiles()) {
		g_Character.vx = g_MoveSpeed;
		g_Character.dir = Point2f{ -1.f, 0.f };
	}
	else if (pStates[SDL_SCANCODE_UP] && !IsBetweenTiles()) {
		g_Character.vy = g_MoveSpeed;
		g_Character.dir = Point2f{ 0.f, -1.f };
	}
	else if (pStates[SDL_SCANCODE_DOWN] && !IsBetweenTiles()) {
		g_Character.vy = g_MoveSpeed;
		g_Character.dir = Point2f{ 0.f, 1.f };
	}
	else {
		if (!IsBetweenTiles()) {
			g_Character.vx = 0.f;
			g_Character.vy = 0.f;
		}
	}
}

void	UpdateCharacterFrame(const Uint8* pStates, float elapsedSec) {

	// Update when moving or not (should i only change when there is update ? here it updates continuously the walk when i move)
	if (g_Character.vx || g_Character.vy) {
		if (g_Character.vx /* maybe check vx is useless */ && g_Character.dir.x == 1.f) {
			g_Character.curAnimFrame = g_AnimFrames["walkright"];
		}
		else if (g_Character.vx && g_Character.dir.x == -1.f) {
			g_Character.curAnimFrame = g_AnimFrames["walkleft"];
		}
		else if (g_Character.vy && g_Character.dir.y == -1.f) {
			g_Character.curAnimFrame = g_AnimFrames["walkup"];
		}
		else if (g_Character.vy && g_Character.dir.y == 1.f) {
			g_Character.curAnimFrame = g_AnimFrames["walkdown"];
		}
	}
	else {
		if (g_Character.dir.x == 1.f) {
			g_Character.curAnimFrame = g_AnimFrames["idleright"];
		}
		else if (g_Character.dir.x == -1.f) {
			g_Character.curAnimFrame = g_AnimFrames["idleleft"];
		}
		else if (g_Character.dir.y == -1.f) {
			g_Character.curAnimFrame = g_AnimFrames["idleup"];
		}
		else if (g_Character.dir.y == 1.f) {
			g_Character.curAnimFrame = g_AnimFrames["idledown"];
		}
	}
	g_Character.frameStartIndex = GetIndex(g_Character.curAnimFrame.row, g_Character.curAnimFrame.col, g_CharacterNrFrames);
	g_Character.src.left = GetCol(g_Character.frameStartIndex + g_Character.frameIndex, g_CharacterNrFrames) * g_Character.frameDimensions.x;

	const float frameRate{ 1.f / 4 };

	if (g_FrameTime > frameRate) {
		g_FrameTime = 0.f;
		g_Character.frameIndex = (g_Character.frameIndex + 1) % g_Character.curAnimFrame.nrFrames;
	}
}

//		UTILS


bool	IsPosInCenterX(float pos) {
	const float centerX = g_WindowWidth / 2.f;
	const float epsilon = 10.f;

	return std::abs(pos - centerX) < epsilon;
}

bool	IsPosInCenterY(float pos) {
	float pos1{ pos - 5.f };
	const float centerY = g_WindowHeight / 2.f;
	const float epsilon = g_TileSize;

	return std::abs(pos - centerY) < epsilon;
}

bool IsBetweenTiles() {
	const Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };

	return (static_cast<int>(g_Character.dst.left) % static_cast<int>(g_TileSize)
		|| static_cast<int>(g_Character.dst.top) % static_cast<int>(g_TileSize));
}

#pragma endregion ownDeclarations