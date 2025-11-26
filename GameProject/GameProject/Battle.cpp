#include "pch.h"
#include "Core.h"
#include "Battle.h"
#include <iostream>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

//		INIT 

void InitBattle() {

	//PlaySound(TEXT("Resources/Godmoongus8Bit2.wav"), NULL, SND_FILENAME | SND_ASYNC); // changed to local path hope its still working

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

//		END

//		INPUT HANDLING

void HandleMouseUpBattle(const SDL_MouseButtonEvent& e) {
	const float mouseX{ float(e.x) };
	const float mouseY{ float(e.y) };

	if (!(g_Attack or g_Switch or g_Item or g_Run))
	{
		if (mouseX >= g_Rects.fightButton.left && mouseX <= (g_Rects.fightButton.left + g_Rects.fightButton.width) &&
			mouseY >= g_Rects.fightButton.top && mouseY <= (g_Rects.fightButton.top + g_Rects.fightButton.height))
		{
			g_Attack = true;
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

//		DRAW

void DrawBattle()
{
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
		g_WindowWidth* 0.35125f,
		g_WindowHeight * 0.39f,
	},
	destinationGodmoonguss
	{
		g_GodmoongussX,
		g_GodmoongussY,
		g_WindowWidth* 0.35125f,
		g_WindowHeight * 0.39f,
	},
	destinationAttack
	{
		attackSpriteSize.x,
		attackSpriteSize.y,
		g_WindowWidth* 0.1f,
		g_WindowHeight * 0.1f,
	},
	destinationTextBlock
	{
		textBlockSpriteSize.x,
		textBlockSpriteSize.y,
		g_WindowWidth* 0.987f,
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
	FillRect(g_Rects.HpBarEnemy.left, g_Rects.HpBarEnemy.top, HPBarEnemyPokemon.hPBarWidth, g_Rects.HpBarEnemy.height);
	FillRect(g_Rects.HpBarAlly.left, g_Rects.HpBarAlly.top, HPBarAllyPokemon.hPBarWidth, g_Rects.HpBarAlly.height);

	SetColor(0.f, 0.f, 0.f);
	DrawRect(g_Rects.HpBarEnemy.left, g_Rects.HpBarEnemy.top, g_Rects.HpBarEnemy.width, g_Rects.HpBarEnemy.height);
	DrawRect(g_Rects.HpBarAlly.left, g_Rects.HpBarAlly.top, g_Rects.HpBarAlly.width, g_Rects.HpBarAlly.height, 2.f);
	utils::DrawTexture(g_GreenTexture, destinationBackground);
	// sometimes you use utils and sometimes not, but you don't need to use it 
	// unless you have another function with the same name and it's ambiguous
}

//		UPDATE

void	UpdateBattle(float elapsedSec) {
	if (g_Attack)
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
		if (g_Attack)
		{
			Attackphase = AttackPhase::phase_lax_forward;
			g_Attack = false;
			g_notFirstTurn = true;
		}
		break;
	}
}

void Item(float elapsedSec)
{
	static bool onlyOnce{ false }; // maybe try to get rid of these
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
					if (HPBarAllyPokemon.hPBarWidth <= 0.f)
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
		attackSpriteSize.x = attackPositionX;
		attackSpriteSize.y = attackPositionY;
	}
	else if (g_SpeedAttack < 40.f)
	{
		attackSpriteSize.x = attackPositionX + attackDistance;
		attackSpriteSize.y = attackPositionY + attackDistance;
	}
	else if (g_SpeedAttack < 60.f)
	{
		attackSpriteSize.x = attackPositionX + (attackDistance * 2);
		attackSpriteSize.y = attackPositionY + (attackDistance * 2);
	}
	else
	{
		g_SpeedAttack = 0.f;
		attackSpriteSize.x = (g_WindowWidth* -0.99375f);
		attackSpriteSize.y = (g_WindowHeight * -0.025f);
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

	HPBarEnemyPokemon.hPBarWidth -= g_SpeedHPBar * elapsedSec;
	if (HPBarEnemyPokemon.hPBarWidth <= HPBarEnemyPokemon.hPBarTarget)
	{
		HPBarEnemyPokemon.hPBarWidth = HPBarEnemyPokemon.hPBarTarget;
		HPBarEnemyPokemon.hPBarTarget = HPBarEnemyPokemon.hPBarTarget - (HPBarEnemyPokemon.hPBarHitAmmount);
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
	HPBarAllyPokemon.hPBarWidth -= g_SpeedHPBar * elapsedSec;
	if (HPBarAllyPokemon.hPBarWidth <= HPBarAllyPokemon.hPBarTarget)
	{
		HPBarAllyPokemon.hPBarWidth = HPBarAllyPokemon.hPBarTarget;
		HPBarAllyPokemon.hPBarTarget = HPBarAllyPokemon.hPBarTarget - (HPBarAllyPokemon.hPBarHitAmmount);
		if (Attackphase == AttackPhase::phase_hpbar2_down)
			Attackphase = static_cast<AttackPhase>(static_cast<int>(Attackphase) + 1);
		else if (Itemphase == ItemPhase::phase_itemhpbar2_down)
			Itemphase = static_cast<ItemPhase>(static_cast<int>(Itemphase) + 1);
	}
}

// theres a function hp1up, then hp2down, 
// it could maybe be only one function and you pass the number of damage/heal to it ?
void HPBar2Up(float elapsedSec)
{
	HPBarAllyPokemon.hPBarWidth += g_SpeedHPBar * elapsedSec;
	if (HPBarAllyPokemon.hPBarWidth >= g_Rects.HpBarAlly.width)
	{
		HPBarAllyPokemon.hPBarWidth = g_Rects.HpBarAlly.width;
		HPBarAllyPokemon.hPBarTarget = g_Rects.HpBarAlly.width - (HPBarAllyPokemon.hPBarHitAmmount);
		Itemphase = static_cast<ItemPhase>(static_cast<int>(Itemphase) + 1);
	}
}