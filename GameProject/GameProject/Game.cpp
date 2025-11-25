#include "pch.h"
#include "Game.h"
#include <iostream>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

//Basic game functions

#pragma region gameFunctions											
void Start()
{

	// MAP & TILES PART INITIALIZATION

	InitWorld();
	InitAnimFrames();
	InitCharacter();


	utils::TextureFromFile("Resources/Mt.Moon1.png", g_Level1Texture);
	utils::TextureFromFile("Resources/MC.png", g_MCTexture);
	InitializeTiles();


	// BATTLE PART INITIALIZATION

	PlaySound(TEXT("C:\\School\\2025-2026\\semester1\\Programming1\\week4\\1DAE12_L04_RAHIMOV_JAVID\\CodingCraftmanship04\\Resources\\Godmoongus8Bit2.wav"), NULL, SND_FILENAME | SND_ASYNC);

	utils::TextureFromFile("Resources/Background.png", g_BackgroundTexture);
	utils::TextureFromFile("Resources/LaxMan.png", g_LaxManTexture);
	utils::TextureFromFile("Resources/Green2.png", g_GreenTexture);
	utils::TextureFromFile("Resources/Godmoonguss.png", g_GodmoongussTexture);
	utils::TextureFromFile("Resources/Attack.png", g_AttackTexture);
	utils::TextureFromFile("Resources/LaxAttack.png", g_LaxAttackTexture);
	utils::TextureFromFile("Resources/GodmoongussAttack.png", g_GodmoongussAttackTexture);
	utils::TextureFromFile("Resources/Wait.png", g_WaitTexture);
	utils::TextureFromFile("Resources/Item.png", g_ItemTexture);
	utils::TextureFromFile("Resources/Switch.png", g_SwitchTexture);
	utils::TextureFromFile("Resources/Run.png", g_RunTexture);
	utils::TextureFromFile("Resources/ItemsDone.png", g_ItemDoneTexture);
	utils::TextureFromFile("Resources/NotFirstTurn.png", g_NotFirstTurnTexture);
	utils::TextureFromFile("Resources/Faint.png", g_FaintTexture);

	std::cout << "press every other button before fight \n" << "Music and sprites by Jasper Bouchet" << std::endl;
}

void Draw()
{
	// MAP & TILES PART DRAW
	ClearBackground(0.f, 0.f, 0.f);
	if (g_IsBattleOn) {
		DrawEverything();
		/*	DrawMap1();
			DrawMC();*/
	}
	// BATTLE PART DRAW
	else {
		DrawMap();
		DrawCharacter();
	}
}

void Update(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	UpdateCharacterDirSpeed(pStates, elapsedSec);
	UpdateMapPos(elapsedSec);


	// BATTLE PART UPDATE
	if (g_IsBattleOn) {

		if (g_Move)
		{
			Attack(elapsedSec);
		}
		if (g_Item == true)
		{
			Item(elapsedSec);
		}
		if (g_Switch == true)
		{
			Switch(elapsedSec);
		}
		if (g_Run == true)
		{
			RunAway(elapsedSec);
		}
	}
}

void End()
{
	FreeWorld();
	// we need to delete all the textures here !!
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{
	if (key == SDLK_b)
		g_IsBattleOn = !g_IsBattleOn;
}

void OnKeyUpEvent(SDL_Keycode key)
{
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	// BATTLE PART MOUSE EVENT

	const float mouseX{ float(e.x) };
	const float mouseY{ float(e.y) };

	if (g_IsBattleOn) {
		if (!(g_Move or g_Switch or g_Item or g_Run))
		{
			if (mouseX >= g_Rects.fightButton.left && mouseX <= (g_Rects.fightButton.left + g_Rects.fightButton.width) &&
				mouseY >= g_Rects.fightButton.top && mouseY <= (g_Rects.fightButton.top + g_Rects.fightButton.height))
			{
				g_Move = true;
				std::cout << "it works" << std::endl;

			}
			else if (mouseX >= g_Rects.pokemonButton.left && mouseX <= (g_Rects.pokemonButton.left + g_Rects.pokemonButton.width) &&
				mouseY >= g_Rects.pokemonButton.top && mouseY <= (g_Rects.pokemonButton.top + g_Rects.pokemonButton.height))
			{
				g_Switch = true;
				std::cout << "it works too" << std::endl;
			}
			else if (mouseX >= g_Rects.itemButton.left && mouseX <= (g_Rects.itemButton.left + g_Rects.itemButton.width) &&
				mouseY >= g_Rects.itemButton.top && mouseY <= (g_Rects.itemButton.top + g_Rects.itemButton.height))
			{
				g_Item = true;
				std::cout << "it works aswell" << std::endl;
			}
			else if (mouseX >= g_Rects.runButton.left && mouseX <= (g_Rects.runButton.left + g_Rects.runButton.width) &&
				mouseY >= g_Rects.runButton.top && mouseY <= (g_Rects.runButton.top + g_Rects.runButton.height))
			{
				g_Run = true;
				std::cout << "it works also" << std::endl;
			}
		}
	}
}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here

// MAP & TILES PART FUNCTIONS

void	InitWorld() {
	if (!TextureFromFile("Resources/map_three_island.png", g_World.scenes[0].texture))
		std::cout << "Couldn't load map texture at Resources/map_three_island.png";
	g_World.scenes[0].zoom = 1.f;

	const float width{ g_World.scenes[0].texture.width }, height{ g_World.scenes[0].texture.height },
		screenWidth{ g_World.scenes[0].texture.width / g_World.scenes[0].zoom };

	g_World.scenes[0].center = Point2f{ 136.f - screenWidth / 2, 222.f - screenWidth / 2 };
	g_World.scenes[0].src = Rectf{ g_World.scenes[0].center.x, g_World.scenes[0].center.y, screenWidth, screenWidth };
	g_World.scenes[0].dst = Rectf{ 0.f, 0.f, g_WindowWidth, g_WindowHeight };
}

void	InitAnimFrames() {
	g_AnimFrames["idlefront"] = AnimFrame{ 0, 0, 1 };
	g_AnimFrames["idleback"] = AnimFrame{ 0, 3, 1 };
	g_AnimFrames["idleleft"] = AnimFrame{ 0, 6, 1 };
	g_AnimFrames["idleright"] = AnimFrame{ 0, 9, 1 };
	g_AnimFrames["walkfront"] = AnimFrame{ 0, 1, 2 };
	g_AnimFrames["walkback"] = AnimFrame{ 0, 4, 2 };
	g_AnimFrames["walkleft"] = AnimFrame{ 0, 7, 2 };
	g_AnimFrames["walkright"] = AnimFrame{ 0, 10, 2 };
}

void	InitCharacter() {
	if (!TextureFromFile("Resources/character.png", g_Character.texture))
		std::cout << "Couldn't load character texture at Resources/character.png";
	g_Character.dst = Rectf{ g_WindowWidth / 2 - g_TileSize / 2, g_WindowWidth / 2 - g_TileSize / 2,  g_TileSize , g_TileSize * 1.5f };
}

void	FreeWorld() { // FreeAll instead ?
	DeleteTexture(g_World.scenes[0].texture);
	DeleteTexture(g_Character.texture);
}

void	DrawMap() {
	const Scene curScene{ g_World.scenes[g_World.currentSceneIndex] };
	DrawTexture(curScene.texture, curScene.dst, curScene.src);
}

void	DrawCharacter() {
	DrawTexture(g_Character.texture, g_Character.dst, g_Character.src);
}

void	UpdateMapPos(float elapsedSec) {
	const float screenWidth{ g_World.scenes[0].texture.width / g_World.scenes[0].zoom };
	Scene* pScene{ &g_World.scenes[g_World.currentSceneIndex] };

	pScene->center.x += g_Character.dir.x * g_Character.vx * elapsedSec;
	pScene->center.y += g_Character.dir.y * g_Character.vy * elapsedSec;
	pScene->src.left = pScene->center.x;
	pScene->src.top = pScene->center.y;
}

void	UpdateCharacterDirSpeed(const Uint8* pStates, float elapsedSec) {
	if (pStates[SDL_SCANCODE_RIGHT]) {
		g_Character.vx = g_MoveSpeed;
		g_Character.dir = Point2f{ 1.f, 0.f };
	}
	else if (pStates[SDL_SCANCODE_LEFT]) {
		g_Character.vx = g_MoveSpeed;
		g_Character.dir = Point2f{ -1.f, 0.f };
	}
	else if (pStates[SDL_SCANCODE_UP]) {
		g_Character.vy = g_MoveSpeed;
		g_Character.dir = Point2f{ 0.f, -1.f };
	}
	else if (pStates[SDL_SCANCODE_DOWN]) {
		g_Character.vy = g_MoveSpeed;
		g_Character.dir = Point2f{ 0.f, 1.f };
	}
	else {
		g_Character.vx = 0.f;
		g_Character.vy = 0.f;
	}
}

void	UpdateCharacterFrame(float elapsedSec) {

}


void DrawMap1()
{
	const Rectf
		destinationMap1
	{
		-958.f,
		-2974.f,
		g_Map1DestinationSizeWidth,
		g_Map1DestinationSizeHeight,
	},
	sourceMap1
	{
		1.f,
		1.f,
		g_Map1SourceSizeWidth,
		g_Map1SourceSizeHeight,
	};
	utils::DrawTexture(g_Level1Texture, destinationMap1, sourceMap1);
}
void DrawMC()
{
	const Rectf
		destinationMC
	{
		428.f,
		380.f,
		g_MCDestinationSize,
		g_MCDestinationSize,
	},
	sourceMC
	{
		69.f,
		1.f,
		g_MCSourceSizeWidth,
		g_MCSourceSizeHeight,
	};
	utils::DrawTexture(g_MCTexture, destinationMC, sourceMC);
}

void InitializeTiles()
{
	int
		rowsTotal{ 72 },
		colsTotal{ 80 };
	float
		yPos{ 0.f },
		xPos{ 0.f };
	for (int row{ 0 }; row < rowsTotal; ++row)
	{
		for (int col{ 0 }; col < colsTotal; ++col)
		{
			ArrTiles[row * colsTotal + col] = Point2f{ xPos, yPos };
			xPos += 48.f;
		}
		yPos += 48.f;
		xPos = 0.f;
	}

}

// BATTLE PART FUNCTIONS


void DrawEverything()
{

	// Put your own draw statements here
	const Rectf
		destinationBackground
	{
		1.f,
		1.f,
		g_WindowWidth,
		g_WindowHeight,
	},
	destinationLaxMan
	{
		g_LaxManX,
		g_LaxManY,
		g_WindowWidth * 0.35125f,
		g_WindowHeight * 0.39f,
	},
	destinationGodmoonguss
	{
		g_GodmoongussX,
		g_GodmoongussY,
		g_WindowWidth * 0.35125f,
		g_WindowHeight * 0.39f,
	},
	destinationAttack
	{
		g_AttackX,
		g_AttackY,
		g_WindowWidth * 0.1f,
		g_WindowHeight * 0.1f,
	},
	destinationTextBlock
	{
		TextBlockX,
		TextBlockY,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	};

	utils::DrawTexture(g_BackgroundTexture, destinationBackground);
	utils::DrawTexture(g_LaxManTexture, destinationLaxMan);
	utils::DrawTexture(g_GodmoongussTexture, destinationGodmoonguss);
	utils::DrawTexture(g_AttackTexture, destinationAttack);

	if (g_GMoongussAttackTextureIsOn == true)
	{
		utils::DrawTexture(g_GodmoongussAttackTexture, destinationTextBlock);
	}
	if (g_LaxAttackTextureIsOn == true)
	{
		utils::DrawTexture(g_LaxAttackTexture, destinationTextBlock);
	}
	if (g_WaitTextBlock == true)
	{
		utils::DrawTexture(g_WaitTexture, destinationTextBlock);
	}
	if (g_ItemTextureIsOn == true)
	{
		utils::DrawTexture(g_ItemTexture, destinationTextBlock);
	}
	if (g_SwitchTextureIsOn == true)
	{
		utils::DrawTexture(g_SwitchTexture, destinationTextBlock);
	}
	if (g_RunTextureIsOn == true)
	{
		utils::DrawTexture(g_RunTexture, destinationTextBlock);
	}
	if (g_ItemDoneTextureIsOn == true)
	{
		utils::DrawTexture(g_ItemDoneTexture, destinationTextBlock);
	}
	if (g_NotFirstTurnTextureIsOn == true)
	{
		utils::DrawTexture(g_NotFirstTurnTexture, destinationTextBlock);
	}
	if (g_FaintTextureIsOn == true)
	{
		utils::DrawTexture(g_FaintTexture, destinationTextBlock);
	}
	SetColor(0.27f, 0.27f, 0.27f);
	FillRect(g_Rects.HpBar1.left, g_Rects.HpBar1.top, g_HPBarWidth1, g_Rects.HpBar1.height);
	FillRect(g_Rects.HpBar2.left, g_Rects.HpBar2.top, g_HPBarWidth2, g_Rects.HpBar2.height);
	SetColor(0.f, 0.f, 0.f);
	DrawRect(g_Rects.HpBar1.left, g_Rects.HpBar1.top, g_Rects.HpBar1.width, g_Rects.HpBar1.height);
	DrawRect(g_Rects.HpBar2.left, g_Rects.HpBar2.top, g_Rects.HpBar2.width, g_Rects.HpBar2.height, 2.f);
	utils::DrawTexture(g_GreenTexture, destinationBackground);


}

void Attack(float elapsedSec)
{
	float speedHPBar{ 1.f };
	static float PhaseFaintCounter{ 0.f };

	switch (Attackphase)
	{
	case AttackPhase::phase_lax_forward:
		LaxForward(elapsedSec);
		g_LaxAttackTextureIsOn = true;
		break;

	case AttackPhase::phase_lax_backward:
		LaxBackward(elapsedSec);
		break;

	case AttackPhase::phase_attack:
		AttackEffect(elapsedSec, g_GodmoongussX, g_GodmoongussY);
		break;
	case AttackPhase::phase_gmoonguss_forward:
		GMoongussForward(elapsedSec);
		break;
	case AttackPhase::phase_gmoonguss_backward:
		GMoongussBackward(elapsedSec);
		break;
	case AttackPhase::phase_hpbar1_down:
		HPBar1Down(elapsedSec);
		break;
	case AttackPhase::phase_wait:
		g_LaxAttackTextureIsOn = false;
		Wait(elapsedSec);
		break;
	case AttackPhase::phase_gmoongusscounter_backward:
		GMoongussBackward(elapsedSec);
		g_GMoongussAttackTextureIsOn = true;
		break;
	case AttackPhase::phase_gmoongusscounter_forward:
		GMoongussForward(elapsedSec);
		break;
	case AttackPhase::phase_attackcounter:
		AttackEffect(elapsedSec, g_LaxManX, g_LaxManY);
		break;
	case AttackPhase::phase_laxcounter_backward:
		LaxBackward(elapsedSec);
		break;

	case AttackPhase::phase_laxcounter_forward:
		LaxForward(elapsedSec);
		break;
	case AttackPhase::phase_hpbar2_down:
		HPBar2Down(elapsedSec);
		break;
	case AttackPhase::phase_done:
		g_GMoongussAttackTextureIsOn = false;
		if (g_Move)
		{
			Attackphase = AttackPhase::phase_lax_forward;
			g_Move = false;
			g_notFirstTurn = true;
		}
		break;
	}
}
void Item(float elapsedSec)
{
	static bool onlyOnce{ false };
	static float phaseWaitCounter{ 0.f };
	static float phaseDoneCounter{ 0.f };
	static float phaseFaintCounter{ 0.f };
	float speedHPBar{ 1.f };
	if (g_notFirstTurn == true)
	{
		if (onlyOnce == false)
		{
			switch (Itemphase)
			{
			case ItemPhase::phase_hpbar2_up:
				HPBar2Up(elapsedSec);
				g_ItemTextureIsOn = true;
				break;
			case ItemPhase::phase_itemwait:
				g_ItemTextureIsOn = false;
				Wait(elapsedSec);
				break;
			case ItemPhase::phase_itemgmoongusscounter_backward:
				GMoongussBackward(elapsedSec);
				g_GMoongussAttackTextureIsOn = true;
				break;
			case ItemPhase::phase_itemgmoongusscounter_forward:
				GMoongussForward(elapsedSec);
				break;
			case ItemPhase::phase_itemattackcounter:
				AttackEffect(elapsedSec, g_LaxManX, g_LaxManY);
				break;
			case ItemPhase::phase_itemlaxcounter_backward:
				LaxBackward(elapsedSec);
				break;
			case ItemPhase::phase_itemlaxcounter_forward:
				LaxForward(elapsedSec);
				break;
			case ItemPhase::phase_itemhpbar2_down:
				HPBar2Down(elapsedSec);
				break;
			case ItemPhase::phase_itemdone:
				g_GMoongussAttackTextureIsOn = false;
				if (g_Item)
				{
					g_Item = false;
					onlyOnce = true;
					if (g_HPBarWidth2 <= 0.f)
					{
						g_FaintTextureIsOn = true;
					}
				}
				break;
			}
		}
		else
		{
			g_ItemDoneTextureIsOn = true;
			phaseDoneCounter += 1.f;
			if (phaseDoneCounter >= 180)
			{
				g_ItemDoneTextureIsOn = false;
				phaseDoneCounter = 0;
				g_Item = false;

			}
		}
	}
	else
	{

		g_NotFirstTurnTextureIsOn = true;
		phaseDoneCounter += 1.f;
		if (phaseDoneCounter >= 180)
		{
			phaseDoneCounter = 0;
			g_NotFirstTurnTextureIsOn = false;
			g_Item = false;

		}
	}
}
void Switch(float elapsedSec)
{
	const float SwitchIncrementation{ 50.f };
	static float g_PhaseWaitCounter{ 0.f };
	g_SwitchTextureIsOn = true;
	g_PhaseWaitCounter += SwitchIncrementation * elapsedSec;
	if (g_PhaseWaitCounter >= 100)
	{
		g_PhaseWaitCounter = 0;
		g_SwitchTextureIsOn = false;
		g_Switch = false;
	}
}
void RunAway(float elapsedSec)
{
	const float RunIncrementation{ 50.f };
	static float g_PhaseWaitCounter{ 0.f };
	g_RunTextureIsOn = true;
	g_PhaseWaitCounter += RunIncrementation * elapsedSec;
	if (g_PhaseWaitCounter >= 100)
	{
		g_PhaseWaitCounter = 0;
		g_RunTextureIsOn = false;
		g_Run = false;
	}
}
void LaxForward(float elapsedSec)
{
	static float savedPosition = -1;

	if (savedPosition < 0) {
		savedPosition = g_LaxManX;
	}

	float target = savedPosition + g_MovementLength;

	if (g_LaxManX < target) {
		g_LaxManX += g_SpeedLax * elapsedSec;
		if (g_LaxManX > target) {
			g_LaxManX = target;
			savedPosition = -1;
			if (Attackphase == AttackPhase::phase_lax_forward || Attackphase == AttackPhase::phase_laxcounter_forward)
				Attackphase = static_cast<AttackPhase>(static_cast<int>(Attackphase) + 1);
			else if (Itemphase == ItemPhase::phase_itemlaxcounter_forward)
				Itemphase = static_cast<ItemPhase>(static_cast<int>(Itemphase) + 1);
		}
	}
}
void LaxBackward(float elapsedSec)
{
	static float savedPosition = -1;

	if (savedPosition < 0) {
		savedPosition = g_LaxManX;
	}

	float target = savedPosition - g_MovementLength;

	if (g_LaxManX > target) {
		g_LaxManX -= g_SpeedLax * elapsedSec;
		if (g_LaxManX < target) {
			g_LaxManX = target;
			savedPosition = -1;
			if (Attackphase == AttackPhase::phase_lax_backward || Attackphase == AttackPhase::phase_laxcounter_backward)
				Attackphase = static_cast<AttackPhase>(static_cast<int>(Attackphase) + 1);
			else if (Itemphase == ItemPhase::phase_itemlaxcounter_backward)
				Itemphase = static_cast<ItemPhase>(static_cast<int>(Itemphase) + 1);
		}
	}
}
void AttackEffect(float elapsedSec, float attackPositionX, float attackPositionY)
{
	float attackIncrimintation{ 60.f };
	float attackDistance{ 100.f };
	g_SpeedAttack += attackIncrimintation * elapsedSec;
	if (g_SpeedAttack < 20.f)
	{
		g_AttackX = attackPositionX;
		g_AttackY = attackPositionY;
	}
	else if (g_SpeedAttack < 40.f)
	{
		g_AttackX = attackPositionX + attackDistance;
		g_AttackY = attackPositionY + attackDistance;
	}
	else if (g_SpeedAttack < 60.f)
	{
		g_AttackX = attackPositionX + (attackDistance * 2);
		g_AttackY = attackPositionY + (attackDistance * 2);
	}
	else
	{
		g_SpeedAttack = 0.f;
		g_AttackX = (g_WindowWidth * -0.99375f);
		g_AttackY = (g_WindowHeight * -0.025f);
		if (Attackphase == AttackPhase::phase_attack || Attackphase == AttackPhase::phase_attackcounter)
			Attackphase = static_cast<AttackPhase>(static_cast<int>(Attackphase) + 1);
		else if (Itemphase == ItemPhase::phase_itemattackcounter)
			Itemphase = static_cast<ItemPhase>(static_cast<int>(Itemphase) + 1);
	}
}
void GMoongussForward(float elapsedSec)
{
	static float savedPosition = -1;

	if (savedPosition < 0) {
		savedPosition = g_GodmoongussX;
	}

	float target = savedPosition + g_MovementLength;

	if (g_GodmoongussX < target) {
		g_GodmoongussX += g_SpeedGmoonguss * elapsedSec;
		if (g_GodmoongussX > target) {
			g_GodmoongussX = target;
			savedPosition = -1;
			if (Attackphase == AttackPhase::phase_gmoongusscounter_forward || Attackphase == AttackPhase::phase_gmoonguss_forward)
				Attackphase = static_cast<AttackPhase>(static_cast<int>(Attackphase) + 1);
			else if (Itemphase == ItemPhase::phase_itemgmoongusscounter_forward)
				Itemphase = static_cast<ItemPhase>(static_cast<int>(Itemphase) + 1);
		}
	}

}
void GMoongussBackward(float elapsedSec)
{

	static float savedPosition = -1;

	if (savedPosition < 0) {
		savedPosition = g_GodmoongussX;
	}

	float target = savedPosition - g_MovementLength;

	if (g_GodmoongussX > target) {
		g_GodmoongussX -= g_SpeedGmoonguss * elapsedSec;
		if (g_GodmoongussX < target) {
			g_GodmoongussX = target;
			savedPosition = -1;
			if (Attackphase == AttackPhase::phase_gmoongusscounter_backward || Attackphase == AttackPhase::phase_gmoonguss_backward)
				Attackphase = static_cast<AttackPhase>(static_cast<int>(Attackphase) + 1);
			else if (Itemphase == ItemPhase::phase_itemgmoongusscounter_backward)
				Itemphase = static_cast<ItemPhase>(static_cast<int>(Itemphase) + 1);
		}
	}
}
void HPBar1Down(float elapsedSec)
{

	g_HPBarWidth1 -= g_SpeedHPBar * elapsedSec;
	if (g_HPBarWidth1 <= g_HPBar1Target)
	{
		g_HPBarWidth1 = g_HPBar1Target;
		g_HPBar1Target = g_HPBar1Target - (g_HPBar1HitAmmount);
		Attackphase = static_cast<AttackPhase>(static_cast<int>(Attackphase) + 1);
	}
}
void Wait(float elapsedSec)
{
	g_WaitTextBlock = true;
	float waitIncrementation{ 50.f };
	g_PhaseWaitCounter += waitIncrementation * elapsedSec;

	if (g_PhaseWaitCounter >= 100)
	{
		g_PhaseWaitCounter = 0;
		if (Attackphase == AttackPhase::phase_wait)
			Attackphase = static_cast<AttackPhase>(static_cast<int>(Attackphase) + 1);
		else if (Itemphase == ItemPhase::phase_itemwait)
			Itemphase = static_cast<ItemPhase>(static_cast<int>(Itemphase) + 1);
		g_WaitTextBlock = false;
	}
}
void HPBar2Down(float elapsedSec)
{
	g_HPBarWidth2 -= g_SpeedHPBar * elapsedSec;
	if (g_HPBarWidth2 <= g_HPBar2Target)
	{
		g_HPBarWidth2 = g_HPBar2Target;
		g_HPBar2Target = g_HPBar2Target - (g_HPBar2HitAmmount);
		if (Attackphase == AttackPhase::phase_hpbar2_down)
			Attackphase = static_cast<AttackPhase>(static_cast<int>(Attackphase) + 1);
		else if (Itemphase == ItemPhase::phase_itemhpbar2_down)
			Itemphase = static_cast<ItemPhase>(static_cast<int>(Itemphase) + 1);
	}
}
void HPBar2Up(float elapsedSec)
{
	g_HPBarWidth2 += g_SpeedHPBar * elapsedSec;
	if (g_HPBarWidth2 >= g_Rects.HpBar2.width)
	{
		g_HPBarWidth2 = g_Rects.HpBar2.width;
		g_HPBar2Target = g_Rects.HpBar2.width - (g_HPBar2HitAmmount);
		Itemphase = static_cast<ItemPhase>(static_cast<int>(Itemphase) + 1);
	}
}
#pragma endregion ownDefinitions