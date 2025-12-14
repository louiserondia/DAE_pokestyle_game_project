#include "pch.h"
#include "Core.h"
#include "Battle.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>

#pragma region Init
void InitBattle()
{
	InitMusic();
	InitSprites();
	InitText();
}
void InitMusic()
{
	//LoadMusic(g_Noises.g_GodmungussBattleMusic, "../Resources/Godmoongus8Bit2.ogg");
	LoadMusic(g_Noises.g_GodmungussBattleMusic, "../Resources/Godmoongus8Bit2.ogg");
	LoadSoundEffect(g_Noises.g_ArrowMove, "../Resources/ArrowMoveSondEffect.wav");
	LoadSoundEffect(g_Noises.g_Attack, "../Resources/AttackSoundEfffect.wav");
	LoadSoundEffect(g_Noises.g_DamageTaken, "../Resources/DamageSoundEffect.wav");
}

void PlayMusicBattle() {
	PlayMusic(g_Noises.g_GodmungussBattleMusic);
}

void InitSprites()
{
	TextureFromFile("Resources/BackgroundCave.png", g_BackgroundTexture);
	TextureFromFile("Resources/FightingOptions.png", g_FightingOptionsTexture);
	TextureFromFile("Resources/LaxMan.png", g_LaxManTexture);
	TextureFromFile("Resources/InfoAllyPokemon.png", g_InfoAllyPokemonTexture);
	TextureFromFile("Resources/InfoEnemyPokemon.png", g_InfoEnemyPokemonTexture);
	TextureFromFile("Resources/GodmoongussColor.png", g_GodmoongussTexture);
	TextureFromFile("Resources/ArrowforOptions.png", g_ArrowTexture);
	TextureFromFile("Resources/Attack.png", g_AttackTexture);
	TextureFromFile("Resources/MovesOptions.png", g_MovesTexture);
}
void InitText()
{

	TextureFromString("LaxMan Attacks Godmunguss", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_LaxAttackText);
	TextureFromString("Godmunguss Retaliates with an attack on Laxman", "Resources/pokemon_fire_red.ttf", 60, Color4f{ 1.f,1.f,1.f,1.f }, g_GodmoongussAttackText);
	TextureFromString("Waiting for Godmunguss", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_WaitText);
	TextureFromString("LaxMan healed with an item", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_ItemText);
	TextureFromString("You don't have any items left", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_ItemDoneText);
	TextureFromString("Your HP is full", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_NotFirstTurnText);
	TextureFromString("You can't run from a god", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_RunText);
	TextureFromString("You don't have pokemon to switch to", "Resources/pokemon_fire_red.ttf", 80, Color4f{ 1.f,1.f,1.f,1.f }, g_SwitchText);
	TextureFromString("Laxman has fainted", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_FaintText);

	std::cout << "Music and sprites by Jasper Bouchet" << std::endl;
}
#pragma endregion Init

#pragma region End
void FreeBattle()
{
	DeleteTexture(g_BackgroundTexture);
	DeleteTexture(g_LaxManTexture);
	DeleteTexture(g_InfoAllyPokemonTexture);
	DeleteTexture(g_GodmoongussTexture);
	DeleteTexture(g_AttackTexture);
	DeleteTexture(g_GodmoongussAttackText);
	DeleteTexture(g_LaxAttackText);
	DeleteTexture(g_WaitText);
	DeleteTexture(g_ItemText);
	DeleteTexture(g_SwitchText);
	DeleteTexture(g_RunText);
	DeleteTexture(g_ItemDoneText);
	DeleteTexture(g_NotFirstTurnText);
	DeleteTexture(g_FaintText);
	DeleteTexture(g_FightingOptionsTexture);
	DeleteTexture(g_InfoEnemyPokemonTexture);
	Mix_FreeMusic(g_Noises.g_GodmungussBattleMusic);
	Mix_FreeChunk(g_Noises.g_ArrowMove);
	Mix_FreeChunk(g_Noises.g_Attack);
	Mix_FreeChunk(g_Noises.g_DamageTaken);
}
#pragma endregion End

#pragma region InputHandling
void HandleKeyUpBattle(SDL_Keycode key)
{
	if (!(g_Attack || g_Switch || g_Item || g_Run))
	{
		if (!g_PickingMoves)
		{
			switch (key)
			{
				case::SDLK_RIGHT:
					if (CurrentFightingOption == FightingOptions::fight || CurrentFightingOption == FightingOptions::pokemon)
					{
						PlaySoundEffect(g_Noises.g_ArrowMove);
						arrowSpritePositionFightingOptions.x += g_HalfWidth * 0.45f;
						if (CurrentFightingOption == FightingOptions::fight)
						{
							CurrentFightingOption = FightingOptions::bag;
						}
						else if (CurrentFightingOption == FightingOptions::pokemon)
						{
							CurrentFightingOption = FightingOptions::run;
						}
					}
					break;
				case::SDLK_LEFT:
					if (CurrentFightingOption == FightingOptions::run || CurrentFightingOption == FightingOptions::bag)
					{
						PlaySoundEffect(g_Noises.g_ArrowMove);
						arrowSpritePositionFightingOptions.x -= g_HalfWidth * 0.45f;
						if (CurrentFightingOption == FightingOptions::run)
						{
							CurrentFightingOption = FightingOptions::pokemon;
						}
						else if (CurrentFightingOption == FightingOptions::bag)
						{
							CurrentFightingOption = FightingOptions::fight;
						}
					}
					break;
				case::SDLK_DOWN:
					if (CurrentFightingOption == FightingOptions::fight || CurrentFightingOption == FightingOptions::bag)
					{
						PlaySoundEffect(g_Noises.g_ArrowMove);
						arrowSpritePositionFightingOptions.y += g_HeightOfTextBlock / 3.f;
						if (CurrentFightingOption == FightingOptions::fight)
						{
							CurrentFightingOption = FightingOptions::pokemon;
						}
						else if (CurrentFightingOption == FightingOptions::bag)
						{
							CurrentFightingOption = FightingOptions::run;
						}
					}
					break;
				case::SDLK_UP:
					if (CurrentFightingOption == FightingOptions::pokemon || CurrentFightingOption == FightingOptions::run)
					{
						PlaySoundEffect(g_Noises.g_ArrowMove);
						arrowSpritePositionFightingOptions.y -= g_HeightOfTextBlock / 3.f;
						if (CurrentFightingOption == FightingOptions::pokemon)
						{
							CurrentFightingOption = FightingOptions::fight;
						}
						else if (CurrentFightingOption == FightingOptions::run)
						{
							CurrentFightingOption = FightingOptions::bag;
						}
					}
					break;
				case::SDLK_SPACE:
					PlaySoundEffect(g_Noises.g_ArrowMove);
					if (CurrentFightingOption == FightingOptions::fight)
					{
						g_PickingMoves = true;
					}
					else if (CurrentFightingOption == FightingOptions::bag)
					{
						g_Item = true;
					}
					else if (CurrentFightingOption == FightingOptions::pokemon)
					{
						g_Switch = true;
					}
					else if (CurrentFightingOption == FightingOptions::run)
					{
						g_Run = true;
					}
					break;
			}
		}
		else
		{
			switch (key)
			{
			case::SDLK_RIGHT:
				if (CurrentMove == MoveOptions::topleft || CurrentMove == MoveOptions::bottomleft)
				{
					PlaySoundEffect(g_Noises.g_ArrowMove);
					arrowSpritePositionMoves.x += g_HalfWidth * 0.45f;
					if (CurrentMove == MoveOptions::topleft)
					{
						CurrentMove = MoveOptions::topright;
					}
					else if (CurrentMove == MoveOptions::bottomleft)
					{
						CurrentMove = MoveOptions::bottomright;
					}
				}
				break;
			case::SDLK_LEFT:
				if (CurrentMove == MoveOptions::bottomright || CurrentMove == MoveOptions::topright)
				{
					PlaySoundEffect(g_Noises.g_ArrowMove);
					arrowSpritePositionMoves.x -= g_HalfWidth * 0.45f;
					if (CurrentMove == MoveOptions::bottomright)
					{
						CurrentMove = MoveOptions::bottomleft;
					}
					else if (CurrentMove == MoveOptions::topright)
					{
						CurrentMove = MoveOptions::topleft;
					}
				}
				break;
			case::SDLK_DOWN:
				if (CurrentMove == MoveOptions::topleft || CurrentMove == MoveOptions::topright)
				{
					PlaySoundEffect(g_Noises.g_ArrowMove);
					arrowSpritePositionMoves.y += g_HeightOfTextBlock / 3.f;
					if (CurrentMove == MoveOptions::topleft)
					{
						CurrentMove = MoveOptions::bottomleft;
					}
					else if (CurrentMove == MoveOptions::topright)
					{
						CurrentMove = MoveOptions::bottomright;
					}
				}
				break;
			case::SDLK_UP:
				if (CurrentMove == MoveOptions::bottomleft || CurrentMove == MoveOptions::bottomright)
				{
					PlaySoundEffect(g_Noises.g_ArrowMove);
					arrowSpritePositionMoves.y -= g_HeightOfTextBlock / 3.f;
					if (CurrentMove == MoveOptions::bottomleft)
					{
						CurrentMove = MoveOptions::topleft;
					}
					else if (CurrentMove == MoveOptions::bottomright)
					{
						CurrentMove = MoveOptions::topright;
					}
				}
				break;
			case::SDLK_SPACE:
				PlaySoundEffect(g_Noises.g_ArrowMove);
				if (CurrentMove == MoveOptions::topleft)
				{
					g_Attack = true;
				}
				else if (CurrentMove == MoveOptions::topright)
				{
					g_Attack = true;
				}
				else if (CurrentMove == MoveOptions::bottomleft)
				{
					g_Attack = true;
				}
				else if (CurrentMove == MoveOptions::bottomright)
				{
					g_Attack = true;
				}
				break;
			}
		}
	}
}
#pragma endregion InputHandling

#pragma region Draw
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
	destinationgInfoAllyPokemonTexture
	{
		g_HalfWidth,
		g_WindowHeight - (g_HeightOfTextBlock * 1.75f),
		g_HalfWidth,
		g_HeightOfTextBlock * 0.7f,
	},
	destinationgInfoEnemyPokemonTexture
	{
		0.f,
		0.f,
		g_WindowWidth * 0.48f,
		g_HeightOfTextBlock * 0.6f,
	},
	destinationArrowFightingOptionsTexture
	{
		arrowSpritePositionFightingOptions.x,
		arrowSpritePositionFightingOptions.y,
		DestinationFightingOptions.width * 0.05f,
		DestinationFightingOptions.height * 0.2f,
	},
	destinationArrowMovesTexture
	{
		arrowSpritePositionMoves.x,
		arrowSpritePositionMoves.y,
		DestinationFightingOptions.width * 0.05f,
		DestinationFightingOptions.height * 0.2f,
	},
	destinationMoves
	{
		0.f,
		g_WindowHeight - g_HeightOfTextBlock,
		g_WindowWidth,
		g_HeightOfTextBlock
	},
	destinationAllyPokemon
	{
		AllyPokemon.position.x,
		AllyPokemon.position.y,
		g_WindowWidth * 0.3f,
		g_HeightOfTextBlock,
	},
	destinationEnemyPokemon
	{
		BossPokemon.position.x,
		BossPokemon.position.y,
		g_WindowWidth * 0.32f,
		g_WindowWidth * 0.32f,
	},
	destinationAttack
	{
		attackSpriteSize.x,
		attackSpriteSize.y,
		g_WindowWidth * 0.1f,
		g_WindowHeight * 0.1f,
	};
	DrawTexture(g_BackgroundTexture, destinationBackground);
	DrawTexture(g_LaxManTexture, destinationAllyPokemon);
	DrawTexture(g_GodmoongussTexture, destinationEnemyPokemon);
	DrawTexture(g_AttackTexture, destinationAttack);
	DrawTexture(g_InfoAllyPokemonTexture, destinationgInfoAllyPokemonTexture);
	DrawTexture(g_InfoEnemyPokemonTexture, destinationgInfoEnemyPokemonTexture);

	if (g_FightingOptionsTextureIsOn)
	{
		DrawTexture(g_FightingOptionsTexture, DestinationFightingOptions);
		DrawTexture(g_ArrowTexture, destinationArrowFightingOptionsTexture);
	}
	if (g_PickingMoves)
	{
		DrawTexture(g_MovesTexture, destinationMoves);
		DrawTexture(g_ArrowTexture, destinationArrowMovesTexture);
	}
	if (EnemyPokemon.attackTextureIsOn == true)
	{
		DrawTexture(g_GodmoongussAttackText, Point2f{
				g_WindowWidth / 2 - g_GodmoongussAttackText.width / 2,
				g_WindowHeight - (g_HeightOfTextBlock * 0.6f)
			});
	}
	if (AllyPokemon.attackTextureIsOn == true)
	{
		DrawTexture(g_LaxAttackText, Point2f{
				g_WindowWidth / 2 - g_LaxAttackText.width / 2,
				g_WindowHeight - (g_HeightOfTextBlock * 0.7f)
			});
	}
	if (g_WaitTextBlock == true)
	{
		DrawTexture(g_WaitText, Point2f{
				g_WindowWidth / 2 - g_WaitText.width / 2,
				g_WindowHeight - (g_HeightOfTextBlock * 0.7f)
			});
	}
	if (g_ItemTextureIsOn == true)
	{
		DrawTexture(g_ItemText, Point2f{
				g_WindowWidth / 2 - g_ItemText.width / 2,
				g_WindowHeight - (g_HeightOfTextBlock * 0.7f)
			});
	}
	if (g_SwitchTextureIsOn == true)
	{
		DrawTexture(g_SwitchText, Point2f{
				g_WindowWidth / 2 - g_SwitchText.width / 2,
				g_WindowHeight - (g_HeightOfTextBlock * 0.7f)
			});
	}
	if (g_RunTextureIsOn == true)
	{
		DrawTexture(g_RunText, Point2f{
				g_WindowWidth / 2 - g_RunText.width / 2,
				g_WindowHeight - (g_HeightOfTextBlock * 0.7f)
			});
	}
	if (g_ItemDoneTextureIsOn == true)
	{
		DrawTexture(g_ItemDoneText, Point2f{
				g_WindowWidth / 2 - g_ItemDoneText.width / 2,
				g_WindowHeight - (g_HeightOfTextBlock * 0.7f)
			});
	}
	if (g_NotFirstTurnTextureIsOn == true)
	{
		DrawTexture(g_NotFirstTurnText, Point2f{
				g_WindowWidth / 2 - g_NotFirstTurnText.width / 2,
				g_WindowHeight - (g_HeightOfTextBlock * 0.7f)
			});
	}
	if (g_FaintTextureIsOn == true)
	{
		DrawTexture(g_FaintText, Point2f{
				g_WindowWidth / 2 - g_FaintText.width / 2,
				g_WindowHeight - (g_HeightOfTextBlock * 0.7f)
			});
	}
	float alphaEnemy{ HPBarEnemyPokemon.animHP / HPBarEnemyPokemon.total };
	float alphaAlly{ HPBarAllyPokemon.animHP / HPBarAllyPokemon.total };
	if ((HPBarEnemyPokemon.width * alphaEnemy) > (HPBarEnemyPokemon.width / 2))
	{
		SetColor(0.44f, 0.97f, 0.66f);
		FillRect(HPBarEnemyPokemon.position.x, HPBarEnemyPokemon.position.y, (HPBarEnemyPokemon.width * alphaEnemy), HPBarEnemyPokemon.height);
	}
	else if ((HPBarEnemyPokemon.width * alphaEnemy) <= (HPBarEnemyPokemon.width / 2) &&
		(HPBarEnemyPokemon.width * alphaEnemy) > (HPBarEnemyPokemon.width / 4))
	{
		SetColor(0.97f, 0.87f, 0.2f);
		FillRect(HPBarEnemyPokemon.position.x, HPBarEnemyPokemon.position.y, (HPBarEnemyPokemon.width * alphaEnemy), HPBarEnemyPokemon.height);
	}
	else if ((HPBarEnemyPokemon.width * alphaEnemy) <= (HPBarEnemyPokemon.width / 4))
	{
		SetColor(0.97f, 0.34f, 0.2f);
		FillRect(HPBarEnemyPokemon.position.x, HPBarEnemyPokemon.position.y, (HPBarEnemyPokemon.width * alphaEnemy), HPBarEnemyPokemon.height);
	}
	if ((HPBarAllyPokemon.width * alphaAlly) > (HPBarAllyPokemon.width / 2))
	{
		SetColor(0.44f, 0.97f, 0.66f);
		FillRect(HPBarAllyPokemon.position.x, HPBarAllyPokemon.position.y, (HPBarAllyPokemon.width * alphaAlly), HPBarAllyPokemon.height);
	}
	else if ((HPBarAllyPokemon.width * alphaAlly) <= (HPBarAllyPokemon.width / 2) &&
		(HPBarAllyPokemon.width * alphaAlly) > (HPBarAllyPokemon.width / 4))
	{
		SetColor(0.97f, 0.87f, 0.2f);
		FillRect(HPBarAllyPokemon.position.x, HPBarAllyPokemon.position.y, (HPBarAllyPokemon.width * alphaAlly), HPBarAllyPokemon.height);
	}
	else if ((HPBarAllyPokemon.width * alphaAlly) <= (HPBarAllyPokemon.width / 4))
	{
		SetColor(0.97f, 0.34f, 0.2f);
		FillRect(HPBarAllyPokemon.position.x, HPBarAllyPokemon.position.y, (HPBarAllyPokemon.width * alphaAlly), HPBarAllyPokemon.height);
	}



}
#pragma endregion Draw

#pragma region Update
void UpdateBattle(float elapsedSec) {
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
		Move(elapsedSec, AllyPokemon, 1);
		AllyPokemon.attackTextureIsOn = true;
		g_FightingOptionsTextureIsOn = false;
		g_PickingMoves = false;
		break;
	case Phases::phase_attack:
		AttackEffect(elapsedSec, BossPokemon.position.x, BossPokemon.position.y);
		break;
	case Phases::phase_enemypokemon_move:
		Move(elapsedSec, BossPokemon, 1);
		break;
	case Phases::phase_hpbarenemy_down:
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_DamageTaken);
			g_SoundDone = true;
		}
		Damage(HPBarEnemyPokemon, Tackle);
		HPBarMath(HPBarEnemyPokemon,elapsedSec);
		break;
	case Phases::phase_wait:
		AllyPokemon.attackTextureIsOn = false;
		Wait(elapsedSec);
		break;
	case Phases::phase_enemypokemoncounter_move:
		Move(elapsedSec, BossPokemon, -1);
		EnemyPokemon.attackTextureIsOn = true;
		break;
	case Phases::phase_attackcounter:
		AttackEffect(elapsedSec, AllyPokemon.position.x, AllyPokemon.position.y);
		break;
	case Phases::phase_allypokemoncounter_move:
		Move(elapsedSec, AllyPokemon, -1);
		break;
	case Phases::phase_hpbarally_down:
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_DamageTaken);
			g_SoundDone = true;
		}
		Damage(HPBarAllyPokemon, StrongTackle);
		HPBarMath(HPBarAllyPokemon, elapsedSec);
		break;
	case Phases::phase_done:
		EnemyPokemon.attackTextureIsOn = false;
		if (g_Attack)
		{
			if (HPBarAllyPokemon.actual <= 0.f)
			{
				HPBarAllyPokemon.actual = 0.f;
				g_FightingOptionsTextureIsOn = false;
				g_FaintTextureIsOn = true;
			}
			AttackSequence = Phases::phase_allypokemon_move;
			g_notFirstTurn = true;
			if (HPBarAllyPokemon.actual != 0.f)
			{
				g_FightingOptionsTextureIsOn = true;
			}
			g_Attack = false;
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
				Heal(HPBarAllyPokemon);
				HPBarMath(HPBarAllyPokemon, elapsedSec);
				g_ItemTextureIsOn = true;
				g_FightingOptionsTextureIsOn = false;
				break;
			case Phases::phase_wait:
				g_ItemTextureIsOn = false;
				Wait(elapsedSec);
				break;
			case Phases::phase_enemypokemoncounter_move:
				EnemyPokemon.attackTextureIsOn = true;
				Move(elapsedSec, BossPokemon, -1);
				break;
			case Phases::phase_attackcounter:
				AttackEffect(elapsedSec, AllyPokemon.position.x, AllyPokemon.position.y);
				break;
			case Phases::phase_allypokemoncounter_move:
				Move(elapsedSec, AllyPokemon, -1);
				break;
			case Phases::phase_hpbarally_down:
				if (!g_SoundDone)
				{
					PlaySoundEffect(g_Noises.g_DamageTaken);
					g_SoundDone = true;
				}
				Damage(HPBarAllyPokemon, StrongTackle);
				HPBarMath(HPBarAllyPokemon, elapsedSec);
				break;
			case Phases::phase_done:
				EnemyPokemon.attackTextureIsOn = false;
				if (g_Item)
				{
					g_ItemOnlyOnce = true;
					g_FightingOptionsTextureIsOn = true;
					if (HPBarAllyPokemon.width <= 0.f)
					{
						g_FightingOptionsTextureIsOn = false;
						g_FaintTextureIsOn = true;
					}
					g_Item = false;
				}
				break;
			}
		}
		else
		{
			g_FightingOptionsTextureIsOn = false;
			g_ItemDoneTextureIsOn = true;
			float waitIncrementation{ 50.f };
			g_PhaseDoneCounter += waitIncrementation * elapsedSec;
			if (g_PhaseDoneCounter >= 180)
			{
				g_ItemDoneTextureIsOn = false;
				g_PhaseDoneCounter = 0;
				g_Item = false;
				g_FightingOptionsTextureIsOn = true;

			}
		}
	}
	else
	{
		g_FightingOptionsTextureIsOn = false;
		g_NotFirstTurnTextureIsOn = true;
		float waitIncrementation{ 50.f };
		g_PhaseDoneCounter += waitIncrementation * elapsedSec;
		if (g_PhaseDoneCounter >= 180)
		{
			g_PhaseDoneCounter = 0;
			g_NotFirstTurnTextureIsOn = false;
			g_Item = false;
			g_FightingOptionsTextureIsOn = true;

		}
	}
}
void Switch(float elapsedSec)
{
	const float SwitchIncrementation{ 50.f };
	g_FightingOptionsTextureIsOn = false;
	g_SwitchTextureIsOn = true;
	g_PhaseWaitCounter += SwitchIncrementation * elapsedSec;
	if (g_PhaseWaitCounter >= 100)
	{
		g_PhaseWaitCounter = 0;
		g_SwitchTextureIsOn = false;
		g_Switch = false;
		g_FightingOptionsTextureIsOn = true;
	}
}
void RunAway(float elapsedSec)
{
	const float RunIncrementation{ 50.f };
	g_FightingOptionsTextureIsOn = false;
	g_RunTextureIsOn = true;
	g_PhaseWaitCounter += RunIncrementation * elapsedSec;
	if (g_PhaseWaitCounter >= 100)
	{
		g_PhaseWaitCounter = 0;
		g_RunTextureIsOn = false;
		g_Run = false;
		g_FightingOptionsTextureIsOn = true;
	}
}
void AttackEffect(float elapsedSec, float attackPositionX, float attackPositionY)
{
	float attackIncrimintation{ 60.f };
	float attackDistance{ (g_WindowWidth * 0.32f)/3.f };
	g_SpeedAttack += attackIncrimintation * elapsedSec;
	if (g_SpeedAttack < 30.f)
	{
		PlaySoundEffect(g_Noises.g_Attack);
		attackSpriteSize.x = attackPositionX;
		attackSpriteSize.y = attackPositionY;
	}
	else if (g_SpeedAttack < 50.f)
	{
		PlaySoundEffect(g_Noises.g_Attack);
		attackSpriteSize.x = attackPositionX + attackDistance;
		attackSpriteSize.y = attackPositionY + attackDistance;
	}
	else if (g_SpeedAttack < 70.f)
	{
		PlaySoundEffect(g_Noises.g_Attack);
		attackSpriteSize.x = attackPositionX + (attackDistance * 2);
		attackSpriteSize.y = attackPositionY + (attackDistance * 2);
	}
	else
	{
		g_SpeedAttack = 0.f;
		attackSpriteSize.x = (g_WindowWidth * -0.99375f);
		attackSpriteSize.y = (g_WindowHeight * -0.025f);
		if (g_Attack)
		{
			if (AttackSequence == Phases::phase_attack || AttackSequence == Phases::phase_attackcounter)
				AttackSequence = static_cast<Phases>(static_cast<int>(AttackSequence) + 1);
		}
		else if (g_Item)
		{
			if (ItemSequence == Phases::phase_attackcounter)
				ItemSequence = static_cast<Phases>(static_cast<int>(ItemSequence) + 1);
		}
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
		if (g_Attack)
		{
			if (AttackSequence == Phases::phase_wait)
				AttackSequence = static_cast<Phases>(static_cast<int>(AttackSequence) + 1);
		}
		else if (g_Item)
		{
			if (ItemSequence == Phases::phase_wait)
				ItemSequence = static_cast<Phases>(static_cast<int>(ItemSequence) + 1);
		}
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
		if (g_Attack)
		{
			if (AttackSequence == Phases::phase_enemypokemoncounter_move ||
				AttackSequence == Phases::phase_enemypokemon_move ||
				AttackSequence == Phases::phase_allypokemoncounter_move ||
				AttackSequence == Phases::phase_allypokemon_move)
			{
				AttackSequence = static_cast<Phases>(static_cast<int>(AttackSequence) + 1);
			}
		}
		else if (g_Item)
		{
			if (ItemSequence == Phases::phase_enemypokemoncounter_move ||
				ItemSequence == Phases::phase_allypokemoncounter_move
				)
			{
				ItemSequence = static_cast<Phases>(static_cast<int>(ItemSequence) + 1);
			}
		}
		pokemon.position.x = g_SavedPosition;
		g_MovementAnimAlpha = 0;
		g_SavedPosition = -1;

		return;
	}

	g_MovementAnimAlpha += elapsedSec * g_AnimationTime;
	bool isMovingBackwards{ g_MovementAnimAlpha > 0.5 };

	float target = g_SavedPosition + (g_MovementLength * dir);

	float alpha = g_MovementAnimAlpha * 2;
	if (isMovingBackwards)
	{
		alpha = 1.f - (g_MovementAnimAlpha - 0.5f) * 2;
	}

	float currentX = utils::Lerp(g_SavedPosition, target, alpha);
	pokemon.position.x = currentX;
}
void Damage(HPBar& hpBarForDamage, Moves& move)
{
	if (g_SavedHPDamage < 0) {
		g_SavedHPDamage = move.damage;
		hpBarForDamage.actual -= g_SavedHPDamage;
	}
}
void Heal(HPBar& hpBarForHealing)
{
	g_IsHeal = true;
	if (g_SavedHPHeal < 0) {
		g_SavedHPHeal = hpBarForHealing.total;
		hpBarForHealing.actual += (g_SavedHPHeal - hpBarForHealing.actual);
	}
}
void HPBarMath(HPBar& hpBar,float elapsedTime)
{
	if (!g_IsHeal)
	{
		if (hpBar.animHP <= hpBar.actual)
		{
			hpBar.animHP = hpBar.actual;
			if (AttackSequence == Phases::phase_hpbarally_down ||
				AttackSequence == Phases::phase_hpbarenemy_down)
			{
				AttackSequence = static_cast<Phases>(static_cast<int>(AttackSequence) + 1);
				g_SavedHPDamage = -1;
				g_SoundDone = false;
			}
			else if (ItemSequence == Phases::phase_hpbarally_down)
			{
				ItemSequence = static_cast<Phases>(static_cast<int>(ItemSequence) + 1);
				g_SavedHPDamage = -1;
				g_SoundDone = false;
			}
			return;
		}
		hpBar.animHP -= elapsedTime * g_SpeedHPBar;
	}
	else
	{
		if (hpBar.animHP >= hpBar.actual)
		{
			hpBar.animHP = hpBar.actual;
			ItemSequence = Phases::phase_wait;
			g_IsHeal = false;
			g_SavedHPHeal = -1;
			return;
		}
		hpBar.animHP += elapsedTime * g_SpeedHPBar;
	}
}
#pragma endregion Update