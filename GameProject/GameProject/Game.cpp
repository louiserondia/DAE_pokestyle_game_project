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
		DrawMap();
		/*	DrawMap1();
			DrawMC();*/
	}
	// BATTLE PART DRAW
	else {
		DrawEverything();
	}
}

void Update(float elapsedSec)
{
	// BATTLE PART UPDATE
	if (g_IsBattleOn) {

		if (g_Move)
		{
			Attack();
		}
		if (g_Item == true)
		{
			Item();
		}
		if (g_Switch == true)
		{
			Switch();
		}
		if (g_Run == true)
		{
			RunAway();
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
			if (mouseX >= g_Buttons.fightButton.left && mouseX <= (g_Buttons.fightButton.left + g_Buttons.fightButton.width) &&
				mouseY >= g_Buttons.fightButton.top && mouseY <= (g_Buttons.fightButton.top + g_Buttons.fightButton.height))
			{
				g_Move = true;

			}
			else if (mouseX >= g_Buttons.pokemonButton.left && mouseX <= (g_Buttons.pokemonButton.left + g_Buttons.pokemonButton.width) &&
				mouseY >= g_Buttons.pokemonButton.top && mouseY <= (g_Buttons.pokemonButton.top + g_Buttons.pokemonButton.height))
			{
				g_Switch = true;
			}
			else if (mouseX >= g_Buttons.itemButton.left && mouseX <= (g_Buttons.itemButton.left + g_Buttons.itemButton.width) &&
				mouseY >= g_Buttons.itemButton.top && mouseY <= (g_Buttons.itemButton.top + g_Buttons.itemButton.height))
			{
				g_Item = true;
			}
			else if (mouseX >= g_Buttons.runButton.left && mouseX <= (g_Buttons.runButton.left + g_Buttons.runButton.width) &&
				mouseY >= g_Buttons.runButton.top && mouseY <= (g_Buttons.runButton.top + g_Buttons.runButton.height))
			{
				g_Run = true;
			}
		}
	}
}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here

// MAP & TILES PART FUNCTIONS

void	InitWorld() {
	TextureFromFile("Resources/map_three_island.png", g_World.scenes[0].texture);
	g_World.scenes[0].zoom = 1.8f;

	const float width{ g_World.scenes[0].texture.width }, height{ g_World.scenes[0].texture.height },
		screenWidth{ g_World.scenes[0].texture.width / g_World.scenes[0].zoom };

	g_World.scenes[0].src = Rectf{ 0.f, 0.f, screenWidth, screenWidth };
	g_World.scenes[0].dst = Rectf{ 0.f, 0.f, g_WindowWidth, g_WindowHeight };

}

void	FreeWorld() {
	DeleteTexture(g_World.scenes[0].texture);
}

void	DrawMap() {
	const Scene curScene{ g_World.scenes[g_World.currentSceneIndex] };
	DrawTexture(curScene.texture, curScene.dst, curScene.src);
}

void	UpdateMapPos() {

}

void	UpdateCharacterDir() {

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
	sourceBackground
	{
		1.f,
		1.f,
		g_BackgroundSizeWidth,
		g_BackgroundSizeHeight,
	},
	destinationLaxMan
	{
		g_LaxManX,
		g_LaxManY,
		g_WindowWidth * 0.35125f,
		g_WindowHeight * 0.39f,
	},
	sourcePokemon
	{
		1.f,
		1.f,
		g_PokemonSize,
		g_PokemonSize,
	},
	sourceTextBlock
	{
		1.f,
		1.f,
		g_TextBlockSizeWidth,
		g_TextBlockSizeHeight,
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
	destinationGodmoongussAttack
	{
		g_GodmoongussAttackX,
		g_GodmoongussAttackY,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	},
	destinationLaxAttack
	{
		g_LaxAttackX,
		g_LaxAttackY,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	},
	destinationWait
	{
		g_WaitX,
		g_WaitY,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	},
	destinationItem
	{
		g_ItemX,
		g_ItemY,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	},
	destinationSwitch
	{
		g_SwitchX,
		g_SwitchY,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	},
	destinationRun
	{
		g_RunX,
		g_RunY,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	},
	destinationItemDone
	{
		g_ItemDoneX,
		g_ItemDoneY,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	},
	destinationNotFirstTurn
	{
		g_NotFirstTurnX,
		g_NotFirstTurnY,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	},
	destinationFaint
	{
		g_FaintX,
		g_FaintY,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	};

	utils::DrawTexture(g_BackgroundTexture, destinationBackground, sourceBackground);
	utils::DrawTexture(g_LaxManTexture, destinationLaxMan, sourcePokemon);
	utils::DrawTexture(g_GodmoongussTexture, destinationGodmoonguss, sourcePokemon);
	utils::DrawTexture(g_AttackTexture, destinationAttack, sourcePokemon);
	utils::DrawTexture(g_GodmoongussAttackTexture, destinationGodmoongussAttack, sourceTextBlock);
	utils::DrawTexture(g_LaxAttackTexture, destinationLaxAttack, sourceTextBlock);
	utils::DrawTexture(g_WaitTexture, destinationWait, sourceTextBlock);
	utils::DrawTexture(g_ItemTexture, destinationItem, sourceTextBlock);
	utils::DrawTexture(g_SwitchTexture, destinationSwitch, sourceTextBlock);
	utils::DrawTexture(g_RunTexture, destinationRun, sourceTextBlock);
	utils::DrawTexture(g_ItemDoneTexture, destinationItemDone, sourceTextBlock);
	utils::DrawTexture(g_NotFirstTurnTexture, destinationNotFirstTurn, sourceTextBlock);
	utils::DrawTexture(g_FaintTexture, destinationFaint, sourceTextBlock);
	SetColor(0.27f, 0.27f, 0.27f);
	FillRect(g_Buttons.HpBar1.left, g_Buttons.HpBar1.top, g_HPBarWidth1, g_Buttons.HpBar1.height);
	FillRect(g_Buttons.HpBar2.left, g_Buttons.HpBar2.top, g_HPBarWidth2, g_Buttons.HpBar2.height);
	SetColor(0.f, 0.f, 0.f);
	DrawRect(g_Buttons.HpBar1.left, g_Buttons.HpBar1.top, g_Buttons.HpBar1.width, g_Buttons.HpBar1.height);
	DrawRect(g_Buttons.HpBar2.left, g_Buttons.HpBar2.top, g_Buttons.HpBar2.width, g_Buttons.HpBar2.height, 2.f);
	utils::DrawTexture(g_GreenTexture, destinationBackground, sourceBackground);


}

void Attack()
{


	float speedLax{ 5.f };
	float speedGmoonguss{ 2.5f };
	float speedHPBar{ 1.f };
	static float PhaseWaitCounter{ 0.f };
	static float PhaseFaintCounter{ 0.f };
	static float speedAttack{ 0.f };
	static float HPBar1Target = g_HPBarWidth1 - (g_WindowWidth * 0.03125f);
	static AttackPhase phase = AttackPhase::phase_lax_forward;

	switch (phase)
	{
	case AttackPhase::phase_lax_forward:
		g_LaxManX += speedLax;
		g_LaxAttackX = (g_WindowWidth * 0.0062548866f);
		g_LaxAttackY = (g_WindowHeight * 0.674196351f);
		if (g_LaxManX >= g_WindowWidth * 0.13125f)
		{
			g_LaxManX = g_WindowWidth * 0.13125f;
			phase = AttackPhase::phase_lax_backward;

		}
		break;

	case AttackPhase::phase_lax_backward:
		g_LaxManX -= speedLax;
		if (g_LaxManX <= g_WindowWidth * 0.0625f)
		{
			g_LaxManX = g_WindowWidth * 0.0625f;
			phase = AttackPhase::phase_attack;

		}
		break;

	case AttackPhase::phase_attack:
		speedAttack += 1.f;
		if (speedAttack < 20.f)
		{
			g_AttackX = (g_WindowWidth * 0.59375f);
			g_AttackY = (g_WindowHeight * 0.125f);
		}
		else if (speedAttack < 40.f)
		{
			g_AttackX = (g_WindowWidth * 0.70f);
			g_AttackY = (g_WindowHeight * 0.175f);
		}
		else if (speedAttack < 60.f)
		{
			g_AttackX = (g_WindowWidth * 0.80f);
			g_AttackY = (g_WindowHeight * 0.225f);
		}
		else
		{
			speedAttack = 0.f;
			g_AttackX = (g_WindowWidth * -0.99375f);
			g_AttackY = (g_WindowHeight * -0.025f);
			g_GodmoongussX = g_WindowWidth * 0.59375f;
			phase = AttackPhase::phase_gmoonguss_forward;
		}
		break;
	case AttackPhase::phase_gmoonguss_forward:
		g_GodmoongussX += speedGmoonguss;
		if (g_GodmoongussX >= g_WindowWidth * 0.6625f)
		{
			g_GodmoongussX = g_WindowWidth * 0.6625f;
			phase = AttackPhase::phase_gmoonguss_backward;
		}
		break;
	case AttackPhase::phase_gmoonguss_backward:
		g_GodmoongussX -= speedGmoonguss;
		if (g_GodmoongussX <= g_WindowWidth * 0.59375f)
		{
			g_GodmoongussX = g_WindowWidth * 0.59375f;
			phase = AttackPhase::phase_hpbar1_down;
		}
		break;
	case AttackPhase::phase_hpbar1_down:
		g_HPBarWidth1 -= speedHPBar;
		if (g_HPBarWidth1 <= HPBar1Target)
		{
			g_HPBarWidth1 = HPBar1Target;
			HPBar1Target = HPBar1Target - (g_WindowWidth * 0.03125f);
			g_LaxAttackX = (g_WindowWidth * -0.99375f);
			g_LaxAttackY = (g_WindowHeight * -0.025f);
			g_WaitX = (g_WindowWidth * 0.0062548866f);
			g_WaitY = (g_WindowHeight * 0.674196351f);
			phase = AttackPhase::phase_wait;
		}
		break;
	case AttackPhase::phase_wait:
		PhaseWaitCounter += 1.f;

		if (PhaseWaitCounter >= 180)
		{
			PhaseWaitCounter = 0;

			phase = AttackPhase::phase_gmoongusscounter_backward;
		}
		break;
	case AttackPhase::phase_gmoongusscounter_backward:
		g_GodmoongussX -= speedGmoonguss;
		g_WaitX = (g_WindowWidth * -0.99375f);
		g_WaitY = (g_WindowHeight * -0.025f);
		g_GodmoongussAttackX = (g_WindowWidth * 0.0062548866f);
		g_GodmoongussAttackY = (g_WindowHeight * 0.674196351f);
		if (g_GodmoongussX <= g_WindowWidth * 0.525f)
		{
			g_GodmoongussX = g_WindowWidth * 0.525f;
			phase = AttackPhase::phase_gmoongusscounter_forward;

		}
		break;
	case AttackPhase::phase_gmoongusscounter_forward:
		g_GodmoongussX += speedGmoonguss;
		if (g_GodmoongussX >= g_WindowWidth * 0.59375f)
		{
			g_GodmoongussX = g_WindowWidth * 0.59375f;
			phase = AttackPhase::phase_attackcounter;
		}
		break;
	case AttackPhase::phase_attackcounter:
		speedAttack += 1.f;
		if (speedAttack < 20.f)
		{
			g_AttackX = (g_WindowWidth * 0.0625f);
			g_AttackY = (g_WindowHeight * 0.350f);
		}
		else if (speedAttack < 40.f)
		{
			g_AttackX = (g_WindowWidth * 0.1625f);
			g_AttackY = (g_WindowHeight * 0.410f);
		}
		else if (speedAttack < 60.f)
		{
			g_AttackX = (g_WindowWidth * 0.2625f);
			g_AttackY = (g_WindowHeight * 0.470f);
		}
		else
		{
			speedAttack = 0.f;
			g_AttackX = (g_WindowWidth * -0.99375f);
			g_AttackY = (g_WindowHeight * -0.025f);
			g_GodmoongussX = g_WindowWidth * 0.59375f;
			phase = AttackPhase::phase_laxcounter_backward;
		}
		break;
	case AttackPhase::phase_laxcounter_backward:
		g_LaxManX -= speedLax;
		if (g_LaxManX <= g_WindowWidth * 0.f)
		{
			g_LaxManX = g_WindowWidth * 0.f;
			phase = AttackPhase::phase_laxcounter_forward;

		}
		break;

	case AttackPhase::phase_laxcounter_forward:
		g_LaxManX += speedLax;
		if (g_LaxManX >= g_WindowWidth * 0.0625f)
		{
			g_LaxManX = g_WindowWidth * 0.0625f;
			phase = AttackPhase::phase_hpbar2_down;
		}
		break;
	case AttackPhase::phase_hpbar2_down:
		g_HPBarWidth2 -= speedHPBar;
		if (g_HPBarWidth2 <= HPBar2Target)
		{
			g_HPBarWidth2 = HPBar2Target;
			HPBar2Target = HPBar2Target - (g_WindowWidth * 0.0625f);
			g_GodmoongussAttackX = (g_WindowWidth * -0.99375f);
			g_GodmoongussAttackY = (g_WindowHeight * -0.025f);
			if (g_HPBarWidth2 <= 0.f)
			{
				g_FaintX = (g_WindowWidth * 0.0062548866f);
				g_FaintY = (g_WindowHeight * 0.674196351f);
			}
			phase = AttackPhase::phase_done;
		}
		break;
	case AttackPhase::phase_done:
		if (g_Move)
		{
			phase = AttackPhase::phase_lax_forward;
			g_Move = false;
			g_notFirstTurn = true;
		}
		break;
	}
}


void Item()
{
	static bool onlyOnce{ false };
	static float phaseWaitCounter{ 0.f };
	static float phaseDoneCounter{ 0.f };
	static float phaseFaintCounter{ 0.f };
	float speedHPBar{ 1.f };
	static float speedAttack{ 0.f };
	float speedLax{ 5.f };
	float speedGmoonguss{ 2.5f };
	static ItemPhase phase = ItemPhase::phase_hpbar2_up;
	if (g_notFirstTurn == true)
	{
		if (onlyOnce == false)
		{
			switch (phase)
			{
			case ItemPhase::phase_hpbar2_up:
				g_ItemX = (g_WindowWidth * 0.0062548866f);
				g_ItemY = (g_WindowHeight * 0.674196351f);
				g_HPBarWidth2 += speedHPBar;
				if (g_HPBarWidth2 >= g_Buttons.HpBar2.width)
				{
					g_HPBarWidth2 = g_Buttons.HpBar2.width;
					HPBar2Target = g_Buttons.HpBar2.width - (g_WindowWidth * 0.0625f);
					g_ItemX = (g_WindowWidth * -0.99375f);
					g_ItemY = (g_WindowHeight * -0.025f);
					g_WaitX = (g_WindowWidth * 0.0062548866f);
					g_WaitY = (g_WindowHeight * 0.674196351f);
					phase = ItemPhase::phase_itemwait;
				}
				break;
			case ItemPhase::phase_itemwait:
				phaseWaitCounter += 1.f;

				if (phaseWaitCounter >= 180)
				{
					phaseWaitCounter = 0;

					phase = ItemPhase::phase_itemgmoongusscounter_backward;
				}
				break;
			case ItemPhase::phase_itemgmoongusscounter_backward:
				g_GodmoongussX -= speedGmoonguss;
				g_WaitX = (g_WindowWidth * -0.99375f);
				g_WaitY = (g_WindowHeight * -0.025f);
				g_GodmoongussAttackX = (g_WindowWidth * 0.0062548866f);
				g_GodmoongussAttackY = (g_WindowHeight * 0.674196351f);
				if (g_GodmoongussX <= g_WindowWidth * 0.525f)
				{
					g_GodmoongussX = g_WindowWidth * 0.525f;
					phase = ItemPhase::phase_itemgmoongusscounter_forward;

				}
				break;
			case ItemPhase::phase_itemgmoongusscounter_forward:
				g_GodmoongussX += speedGmoonguss;
				if (g_GodmoongussX >= g_WindowWidth * 0.59375f)
				{
					g_GodmoongussX = g_WindowWidth * 0.59375f;
					phase = ItemPhase::phase_itemattackcounter;
				}
				break;
			case ItemPhase::phase_itemattackcounter:
				speedAttack += 1.f;
				if (speedAttack < 20.f)
				{
					g_AttackX = (g_WindowWidth * 0.0625f);
					g_AttackY = (g_WindowHeight * 0.350f);
				}
				else if (speedAttack < 40.f)
				{
					g_AttackX = (g_WindowWidth * 0.1625f);
					g_AttackY = (g_WindowHeight * 0.410f);
				}
				else if (speedAttack < 60.f)
				{
					g_AttackX = (g_WindowWidth * 0.2625f);
					g_AttackY = (g_WindowHeight * 0.470f);
				}
				else
				{
					speedAttack = 0.f;
					g_AttackX = (g_WindowWidth * -0.99375f);
					g_AttackY = (g_WindowHeight * -0.025f);
					g_GodmoongussX = g_WindowWidth * 0.59375f;
					phase = ItemPhase::phase_itemlaxcounter_backward;
				}
				break;
			case ItemPhase::phase_itemlaxcounter_backward:
				g_LaxManX -= speedLax;
				if (g_LaxManX <= g_WindowWidth * 0.f)
				{
					g_LaxManX = g_WindowWidth * 0.f;
					phase = ItemPhase::phase_itemlaxcounter_forward;

				}
				break;

			case ItemPhase::phase_itemlaxcounter_forward:
				g_LaxManX += speedLax;
				if (g_LaxManX >= g_WindowWidth * 0.0625f)
				{
					g_LaxManX = g_WindowWidth * 0.0625f;
					phase = ItemPhase::phase_itemhpbar2_down;
				}
				break;
			case ItemPhase::phase_itemhpbar2_down:
				g_HPBarWidth2 -= speedHPBar;
				if (g_HPBarWidth2 <= HPBar2Target)
				{
					g_HPBarWidth2 = HPBar2Target;
					HPBar2Target = HPBar2Target - (g_WindowWidth * 0.0625f);
					g_GodmoongussAttackX = (g_WindowWidth * -0.99375f);
					g_GodmoongussAttackY = (g_WindowHeight * -0.025f);
					phase = ItemPhase::phase_itemdone;
				}
				break;
			case ItemPhase::phase_itemdone:
				if (g_Item)
				{
					phase = ItemPhase::phase_hpbar2_up;
					g_Item = false;
					onlyOnce = true;
					if (g_HPBarWidth2 <= 0.f)
					{
						g_FaintX = (g_WindowWidth * 0.0062548866f);
						g_FaintY = (g_WindowHeight * 0.674196351f);
					}
				}
				break;
			}
		}
		else
		{
			g_ItemDoneX = (g_WindowWidth * 0.0062548866f);
			g_ItemDoneY = (g_WindowHeight * 0.674196351f);
			phaseDoneCounter += 1.f;
			if (phaseDoneCounter >= 180)
			{
				phaseDoneCounter = 0;
				g_ItemDoneX = (g_WindowWidth * -0.99375f);
				g_ItemDoneY = (g_WindowHeight * -0.025f);
				g_Item = false;

			}
		}
	}
	else
	{
		g_NotFirstTurnX = (g_WindowWidth * 0.0062548866f);
		g_NotFirstTurnY = (g_WindowHeight * 0.674196351f);
		phaseDoneCounter += 1.f;
		if (phaseDoneCounter >= 180)
		{
			phaseDoneCounter = 0;
			g_NotFirstTurnX = (g_WindowWidth * -0.99375f);
			g_NotFirstTurnY = (g_WindowHeight * -0.025f);
			g_Item = false;

		}
	}
}
void Switch()
{
	static float g_PhaseWaitCounter{ 0.f };
	g_SwitchX = (g_WindowWidth * 0.0062548866f);
	g_SwitchY = (g_WindowHeight * 0.674196351f);
	g_PhaseWaitCounter += 1.f;
	if (g_PhaseWaitCounter >= 180)
	{
		g_PhaseWaitCounter = 0;
		g_SwitchX = (g_WindowWidth * -0.99375f);
		g_SwitchY = (g_WindowHeight * -0.025f);
		g_Switch = false;
	}
}
void RunAway()
{
	static float g_PhaseWaitCounter{ 0.f };
	g_RunX = (g_WindowWidth * 0.0062548866f);
	g_RunY = (g_WindowHeight * 0.674196351f);
	g_PhaseWaitCounter += 1.f;
	if (g_PhaseWaitCounter >= 180)
	{
		g_PhaseWaitCounter = 0;
		g_RunX = (g_WindowWidth * -0.99375f);
		g_RunY = (g_WindowHeight * -0.025f);
		g_Run = false;
	}
}

#pragma endregion ownDefinitions