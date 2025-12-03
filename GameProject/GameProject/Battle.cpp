#include "pch.h"
#include "Core.h"
#include "Battle.h"
#include <iostream>
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

//		INIT 

void InitBattle() {

	PlaySound(TEXT("Resources/Godmoongus8Bit2.wav"), NULL, SND_FILENAME | SND_ASYNC); // changed to local path hope its still working

	TextureFromFile("Resources/Background.png", g_BackgroundTexture);
	TextureFromFile("Resources/LaxMan.png", g_LaxManTexture);
	TextureFromFile("Resources/Green2.png", g_GreenTexture);
	TextureFromFile("Resources/Godmoonguss.png", g_GodmoongussTexture);
	TextureFromFile("Resources/Attack.png", g_AttackTexture);
	TextureFromFile("Resources/LaxAttack.png", g_LaxAttackTexture);
	TextureFromFile("Resources/GodmoongussAttack.png", g_GodmoongussAttackTexture);
	TextureFromFile("Resources/Wait.png", g_WaitTexture);
	TextureFromFile("Resources/Item.png", g_ItemTexture);
	TextureFromFile("Resources/Switch.png", g_SwitchTexture);
	TextureFromFile("Resources/Run.png", g_RunTexture);
	TextureFromFile("Resources/ItemsDone.png", g_ItemDoneTexture);
	TextureFromFile("Resources/NotFirstTurn.png", g_NotFirstTurnTexture);
	TextureFromFile("Resources/Faint.png", g_FaintTexture);

	std::cout << "press every other button before fight \n" << "Music and sprites by Jasper Bouchet" << std::endl;
}

//		END

//		INPUT HANDLING

void HandleMouseUpBattle(const SDL_MouseButtonEvent& e) {
	const float mouseX{ float(e.x) };
	const float mouseY{ float(e.y) };

	if (!(g_Attack || g_Switch || g_Item || g_Run))
	{
		if (mouseX >= fightButton.left && mouseX <= (fightButton.left + fightButton.width) &&
			mouseY >= fightButton.top && mouseY <= (fightButton.top + fightButton.height))
		{
			g_Attack = true;
			Damage(HPBarEnemyPokemon);
			Damage(HPBarAllyPokemon);
			std::cout << "it works" << std::endl;
		}
		else if (mouseX >= pokemonButton.left && mouseX <= (pokemonButton.left + pokemonButton.width) &&
			mouseY >= pokemonButton.top && mouseY <= (pokemonButton.top + pokemonButton.height))
		{
			g_Switch = true;
			std::cout << "it works too" << std::endl;
		}
		else if (mouseX >= itemButton.left && mouseX <= (itemButton.left + itemButton.width) &&
			mouseY >= itemButton.top && mouseY <= (itemButton.top + itemButton.height))
		{
			g_Item = true;
			Heal(HPBarAllyPokemon);
			std::cout << "it works aswell" << std::endl;
		}
		else if (mouseX >= runButton.left && mouseX <= (runButton.left + runButton.width) &&
			mouseY >= runButton.top && mouseY <= (runButton.top + runButton.height))
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
		0.f,
		0.f,
		g_WindowWidth,
		g_WindowHeight,
	},
	destinationLaxMan
	{
		AllyPokemon.position.x,
		AllyPokemon.position.y,
		g_WindowWidth * 0.35125f,
		g_WindowHeight * 0.39f,
	},
	destinationGodmoonguss
	{
		EnemyPokemon.position.x,
		EnemyPokemon.position.y,
		g_WindowWidth * 0.35125f,
		g_WindowHeight * 0.39f,
	},
	destinationAttack
	{
		attackSpriteSize.x,
		attackSpriteSize.y,
		g_WindowWidth * 0.1f,
		g_WindowHeight * 0.1f,
	},
	destinationTextBlock
	{
		textBlockSpriteSize.x,
		textBlockSpriteSize.y,
		g_WindowWidth * 0.987f,
		g_WindowHeight * 0.3197f,
	};

	DrawTexture(g_BackgroundTexture, destinationBackground);
	DrawTexture(g_LaxManTexture, destinationLaxMan);
	DrawTexture(g_GodmoongussTexture, destinationGodmoonguss);
	DrawTexture(g_AttackTexture, destinationAttack);

	if (EnemyPokemon.attackTextureIsOn == true)
	{
		DrawTexture(g_GodmoongussAttackTexture, destinationTextBlock);
	}
	if (AllyPokemon.attackTextureIsOn == true)
	{
		DrawTexture(g_LaxAttackTexture, destinationTextBlock);
	}
	if (g_WaitTextBlock == true)
	{
		DrawTexture(g_WaitTexture, destinationTextBlock);
	}
	if (g_ItemTextureIsOn == true)
	{
		DrawTexture(g_ItemTexture, destinationTextBlock);
	}
	if (g_SwitchTextureIsOn == true)
	{
		DrawTexture(g_SwitchTexture, destinationTextBlock);
	}
	if (g_RunTextureIsOn == true)
	{
		DrawTexture(g_RunTexture, destinationTextBlock);
	}
	if (g_ItemDoneTextureIsOn == true)
	{
		DrawTexture(g_ItemDoneTexture, destinationTextBlock);
	}
	if (g_NotFirstTurnTextureIsOn == true)
	{
		DrawTexture(g_NotFirstTurnTexture, destinationTextBlock);
	}
	if (g_FaintTextureIsOn == true)
	{
		DrawTexture(g_FaintTexture, destinationTextBlock);
	}
	SetColor(0.27f, 0.27f, 0.27f);
	float hello{ HpBarEnemy.left + HPBarEnemyPokemon.hPBarWidth };
	FillRect(HpBarEnemy.left, HpBarEnemy.top, HPBarEnemyPokemon.hPBarWidth, HpBarEnemy.height);
	FillRect(HpBarAlly.left, HpBarAlly.top, HPBarAllyPokemon.hPBarWidth, HpBarAlly.height);

	SetColor(0.f, 0.f, 0.f);
	DrawRect(HpBarEnemy.left, HpBarEnemy.top, HpBarEnemy.width, HpBarEnemy.height);
	DrawRect(HpBarAlly.left, HpBarAlly.top, HpBarAlly.width, HpBarAlly.height, 2.f);
	DrawTexture(g_GreenTexture, destinationBackground);
}

//		UPDATE

void	UpdateBattle(float elapsedSec) {
	if (g_Attack)
	{
		Attack(elapsedSec);
	}
	if (g_Item)
	{
		Item(elapsedSec);
	}
	if (g_Switch)
	{
		Switch(elapsedSec);
	}
	if (g_Run)
	{
		RunAway(elapsedSec);
	}
}
void Attack(float elapsedSec)
{
	switch (AttackSequence)
	{
	case Phases::phase_allypokemon_move:
		Move(elapsedSec, AllyPokemon,1);
		AllyPokemon.attackTextureIsOn = true;
		break;
	case Phases::phase_attack:
		AttackEffect(elapsedSec, EnemyPokemon.position.x, EnemyPokemon.position.y);
		break;
	case Phases::phase_enemypokemon_move:
		Move(elapsedSec, EnemyPokemon,1);
		break;
	case Phases::phase_hpbarenemy_down:
		HPBarEnemyDown(elapsedSec);
		break;
	case Phases::phase_wait:
		AllyPokemon.attackTextureIsOn = false;
		Wait(elapsedSec);
		break;
	case Phases::phase_enemypokemoncounter_move:
		Move(elapsedSec, EnemyPokemon,-1);
		EnemyPokemon.attackTextureIsOn = true;
		break;
	case Phases::phase_attackcounter:
		AttackEffect(elapsedSec, AllyPokemon.position.x, AllyPokemon.position.y);
		break;
	case Phases::phase_allypokemoncounter_move:
		Move(elapsedSec, AllyPokemon,-1);
		break;
	case Phases::phase_hpbarally_down:
		HPBarAllyDown(elapsedSec);
		break;
	case Phases::phase_done:
		EnemyPokemon.attackTextureIsOn = false;
		if (g_Attack)
		{
			AttackSequence = Phases::phase_allypokemon_move;
			g_Attack = false;
			g_notFirstTurn = true;
		}
		break;
	}
}
void Item(float elapsedSec)
{
	if (g_notFirstTurn == true)
	{
		if (g_ItemOnlyOnce == false)
		{
			switch (ItemSequence)
			{
			case Phases::phase_hpbarally_up:
	
				HPBarAllyUp(elapsedSec);
				g_ItemTextureIsOn = true;
				break;
			case Phases::phase_wait:
				g_ItemTextureIsOn = false;
				Wait(elapsedSec);
				break;
			case Phases::phase_enemypokemoncounter_move:
				Move(elapsedSec, EnemyPokemon,-1);
				EnemyPokemon.attackTextureIsOn = true;
				break;
			case Phases::phase_attackcounter:
				AttackEffect(elapsedSec, AllyPokemon.position.x, AllyPokemon.position.y);
				break;
			case Phases::phase_allypokemoncounter_move:
				Move(elapsedSec, AllyPokemon,-1);
				break;
			case Phases::phase_hpbarally_down:
				HPBarAllyDown(elapsedSec);
				break;
			case Phases::phase_done:
				EnemyPokemon.attackTextureIsOn = false;
				if (g_Item)
				{
					g_Item = false;
					g_ItemOnlyOnce = true;
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
			g_PhaseDoneCounter += 1.f;
			if (g_PhaseDoneCounter >= 180)
			{
				g_ItemDoneTextureIsOn = false;
				g_PhaseDoneCounter = 0;
				g_Item = false;

			}
		}
	}
	else
	{
		g_NotFirstTurnTextureIsOn = true;
		g_PhaseDoneCounter += 1.f;
		if (g_PhaseDoneCounter >= 180)
		{
			g_PhaseDoneCounter = 0;
			g_NotFirstTurnTextureIsOn = false;
			g_Item = false;

		}
	}
}
void Switch(float elapsedSec)
{
	const float SwitchIncrementation{ 50.f };

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
	g_RunTextureIsOn = true;
	g_PhaseWaitCounter += RunIncrementation * elapsedSec;
	if (g_PhaseWaitCounter >= 100)
	{
		g_PhaseWaitCounter = 0;
		g_RunTextureIsOn = false;
		g_Run = false;
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
		attackSpriteSize.x = (g_WindowWidth * -0.99375f);
		attackSpriteSize.y = (g_WindowHeight * -0.025f);
		if (AttackSequence == Phases::phase_attack || AttackSequence == Phases::phase_attackcounter)
			AttackSequence = static_cast<Phases>(static_cast<int>(AttackSequence) + 1);
		else if (ItemSequence == Phases::phase_attackcounter)
			ItemSequence = static_cast<Phases>(static_cast<int>(ItemSequence) + 1);
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
		if (AttackSequence == Phases::phase_wait)
			AttackSequence = static_cast<Phases>(static_cast<int>(AttackSequence) + 1);
		else if (ItemSequence == Phases::phase_wait)
			ItemSequence = static_cast<Phases>(static_cast<int>(ItemSequence) + 1);
		g_WaitTextBlock = false;
	}
}

void Move(float elapsedSec, PokemonInBattle& pokemon, int dir)
{
	if (g_SavedPosition < 0)
	{
		g_SavedPosition = pokemon.position.x;
		g_MovementAnimAlpha = 0;
	}
	if (g_MovementAnimAlpha >= 1)
	{
		if (AttackSequence == Phases::phase_enemypokemoncounter_move ||
			AttackSequence == Phases::phase_enemypokemon_move ||
			AttackSequence == Phases::phase_allypokemoncounter_move ||
			AttackSequence == Phases::phase_allypokemon_move)
		{
			AttackSequence = static_cast<Phases>(static_cast<int>(AttackSequence) + 1);
		}
		else if (ItemSequence == Phases::phase_enemypokemoncounter_move ||
			ItemSequence == Phases::phase_allypokemoncounter_move
			)
		{
			ItemSequence = static_cast<Phases>(static_cast<int>(ItemSequence) + 1);
		}
		pokemon.position.x = g_SavedPosition;
		g_MovementAnimAlpha = 0;
		g_SavedPosition = -1;

		return;
	}

	g_MovementAnimAlpha += elapsedSec* g_AnimationTime;
	bool isMovingBackwards{ g_MovementAnimAlpha > 0.5 };

	float target = g_SavedPosition + (g_MovementLength * dir);

	float alpha = g_MovementAnimAlpha * 2;
	if (isMovingBackwards)
	{
		alpha = 1.f - (g_MovementAnimAlpha - 0.5f)*2;
	}

	float currentX = utils::Lerp(g_SavedPosition, target, alpha);
	pokemon.position.x = currentX;
}


// theres a function hp1up, then hp2down, 
// it could maybe be only one function and you pass the number of damage/heal to it ?
void HPBarAllyUp(float elapsedSec)
{
	HPBarAllyPokemon.hPBarWidth += g_SpeedHPBar * elapsedSec;
	float target{ ((HPBarAllyPokemon.hpBarCurrent / HPBarEnemyPokemon.hpBarTotal) * HpBarAlly.width) };
	std::cout << "HP bar: " << HPBarEnemyPokemon.hPBarWidth << " | Target: " << target << std::endl;
	if (HPBarAllyPokemon.hPBarWidth >= target)
	{
		HPBarAllyPokemon.hPBarWidth = target;
		ItemSequence = Phases::phase_wait;
	}
}
void HPBarAllyDown(float elapsedSec)
{
	HPBarAllyPokemon.hPBarWidth -= g_SpeedHPBar * elapsedSec;
	float target{ ((HPBarAllyPokemon.hpBarCurrent / HPBarEnemyPokemon.hpBarTotal) * HpBarAlly.width) };
	std::cout << "HP bar: " << HPBarEnemyPokemon.hPBarWidth << " | Target: " << target << std::endl;
	if (HPBarAllyPokemon.hPBarWidth <= target)
	{
		HPBarAllyPokemon.hPBarWidth = target;
		if (AttackSequence == Phases::phase_hpbarally_down)
			AttackSequence = static_cast<Phases>(static_cast<int>(AttackSequence) + 1);
		else if (ItemSequence == Phases::phase_hpbarally_down)
			ItemSequence = static_cast<Phases>(static_cast<int>(ItemSequence) + 1);
	}
}
void HPBarEnemyDown(float elapsedSec)
{
	float target{ ((HPBarEnemyPokemon.hpBarCurrent / HPBarEnemyPokemon.hpBarTotal) * HpBarEnemy.width) };
	HPBarEnemyPokemon.hPBarWidth -= g_SpeedHPBar * elapsedSec;
	if (HPBarEnemyPokemon.hPBarWidth <= target)
	{
		HPBarEnemyPokemon.hPBarWidth = target;
		AttackSequence = static_cast<Phases>(static_cast<int>(AttackSequence) + 1);
	}
}

void Damage(HPBar& hpBarForDamage)
{
	bool isDamage{ true };
	std::cout << "HP bar: " << HPBarEnemyPokemon.hPBarWidth << " | Target: " << HPBarEnemyPokemon.hpBarCurrent << std::endl;
		HPBarMath(hpBarForDamage.hpBarHitAmmount,hpBarForDamage, isDamage);
}
void Heal(HPBar& hpBarForHealing)
{
	bool isDamage{ false };
	HPBarMath(hpBarForHealing.hpBarHitAmmount, hpBarForHealing, isDamage);
}
void HPBarMath( float amountChange, HPBar& hpBar, bool isDamage)
{
	if(isDamage)
	hpBar.hpBarCurrent -= amountChange;
	else
	{
		hpBar.hpBarCurrent += (100 - hpBar.hpBarCurrent);
		isDamage = true;
	}
}