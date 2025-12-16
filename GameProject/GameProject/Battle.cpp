#include "pch.h"
#include "Core.h"
#include "Battle.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>

void TurnOffBattle();

#pragma region Init
void InitBattle()
{
	InitMusic();
	InitSprites();
	InitText();
}
void InitMusic()
{
	LoadMusic(g_Noises.g_GodmungussBattleMusic, "../Resources/Godmoongus8Bit2.ogg");
	LoadSoundEffect(g_Noises.g_ArrowMove, "../Resources/ArrowMoveSondEffect.wav");
	LoadSoundEffect(g_Noises.g_Surf, "../Resources/SurfSound.wav");
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
	TextureFromFile("Resources/Gyarados.png", g_GyaradosTexture);
	TextureFromFile("Resources/InfoAllyPokemon.png", g_InfoAllyPokemonTexture);
	TextureFromFile("Resources/InfoEnemyPokemon.png", g_InfoEnemyPokemonTexture);
	TextureFromFile("Resources/GodmoongussColor.png", g_GodmoongussTexture);
	TextureFromFile("Resources/ArrowforOptions.png", g_ArrowTexture);
	TextureFromFile("Resources/Attack.png", g_AttackTexture);
	TextureFromFile("Resources/MovesOptions.png", g_MovesTexture);
	TextureFromFile("Resources/Surf.png", g_SurfTexture);
	TextureFromFile("Resources/HydroPump.png", g_HydroPumpTexture);
	TextureFromFile("Resources/DragonRage.png", g_DragonRageTexture);
}
void InitText()
{
	//g_GyaradosAttackText = "Gyarados Attacks Godmunguss";
	//	g_GodmoongussAttackText = "Godmunguss Retaliates with an attack on Gyarados";
	//	g_WaitText = "Waiting for Godmunguss";
	//	g_ItemText = "Gyarados healed with an item";
	//	g_ItemDoneText = "You don't have any items left";
	//	g_NotFirstTurnText = "Your HP is full";
	//	g_RunText = "You can't run from a god";
	//	g_SwitchText = "You don't have pokemon to switch to";
	//	g_FaintText = "Gyarados has fainted";
	//	g_GyaradosNameText = "Gyarados";
	//	g_GodmoongussNameText = "Godmoonguss";
	//	g_SurfText = "Surf";
	//	g_HydroPumpText = "Hydro Pump";
	//	g_DragonRageText = "Dragon Rage";
	//	g_HyperBeamText = "Hyper Beam";



	TextureFromString("Gyarados Attacks Godmunguss", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_GyaradosAttackText);
	TextureFromString("Godmunguss Retaliates with an attack on Gyarados", "Resources/pokemon_fire_red.ttf", 60, Color4f{ 1.f,1.f,1.f,1.f }, g_GodmoongussAttackText);
	TextureFromString("Waiting for Godmunguss", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_WaitText);
	TextureFromString("Gyarados healed with an item", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_ItemText);
	TextureFromString("You don't have any items left", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_ItemDoneText);
	TextureFromString("Your HP is full", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_NotFirstTurnText);
	TextureFromString("You can't run from a god", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_RunText);
	TextureFromString("You don't have pokemon to switch to", "Resources/pokemon_fire_red.ttf", 80, Color4f{ 1.f,1.f,1.f,1.f }, g_SwitchText);
	TextureFromString("Gyarados has fainted", "Resources/pokemon_fire_red.ttf", 100, Color4f{ 1.f,1.f,1.f,1.f }, g_FaintText);
	TextureFromString("Gyarados", "Resources/pokemon_fire_red.ttf", 80, Color4f{ 0.f,0.f,0.f,1.f }, g_GyaradosNameText);
	TextureFromString("Godmoonguss", "Resources/pokemon_fire_red.ttf", 80, Color4f{ 0.f,0.f,0.f,1.f }, g_GodmoongussNameText);
	TextureFromString("Surf", "Resources/pokemon_fire_red.ttf", 60, Color4f{ 0.f,0.f,0.f,1.f }, g_SurfText);
	TextureFromString("Hydro Pump", "Resources/pokemon_fire_red.ttf", 60, Color4f{ 0.f,0.f,0.f,1.f }, g_HydroPumpText);
	TextureFromString("Dragon Rage", "Resources/pokemon_fire_red.ttf", 60, Color4f{ 0.f,0.f,0.f,1.f }, g_DragonRageText);
	TextureFromString("Hyper Beam", "Resources/pokemon_fire_red.ttf", 60, Color4f{ 0.f,0.f,0.f,1.f }, g_HyperBeamText);

	std::cout << "Music and Godmoonguss sprites by Jasper Bouchet" << std::endl;
}
#pragma endregion Init

#pragma region End
void FreeBattle()
{
	DeleteTexture(g_BackgroundTexture);
	DeleteTexture(g_GyaradosTexture);
	DeleteTexture(g_InfoAllyPokemonTexture);
	DeleteTexture(g_GodmoongussTexture);
	DeleteTexture(g_AttackTexture);
	DeleteTexture(g_GodmoongussAttackText);
	DeleteTexture(g_GyaradosAttackText);
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
	if ((!(g_Attack || g_Switch || g_Item || g_Run)))
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
					arrowSpritePositionMoves.x += g_HalfWidth * 0.65f;
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
					arrowSpritePositionMoves.x -= g_HalfWidth * 0.65f;
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
					g_SurfIsOn = true;
				}
				else if (CurrentMove == MoveOptions::topright)
				{
					g_Attack = true;
					g_HydroPumpIsOn = true;
				}
				else if (CurrentMove == MoveOptions::bottomleft)
				{
					g_Attack = true;
					g_DragonRageIsOn = true;
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
	Rectf
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
		g_DestinationFightingOptions.width * 0.05f,
		g_DestinationFightingOptions.height * 0.2f,
	},
	destinationArrowMovesTexture
	{
		arrowSpritePositionMoves.x,
		arrowSpritePositionMoves.y,
		g_DestinationFightingOptions.width * 0.05f,
		g_DestinationFightingOptions.height * 0.2f,
	},
	destinationMoves
	{
		0.f,
		g_WindowHeight - g_HeightOfTextBlock,
		g_WindowWidth,
		g_HeightOfTextBlock
	},
		destinationAttack
	{
		attackSpriteSize.x,
		attackSpriteSize.y,
		g_WindowWidth * 0.1f,
		g_WindowHeight * 0.1f,
	},
	destinationSurf
	{
		g_SurfPosition.x,
		g_SurfPosition.y,
		g_WindowWidth * 0.4f,
		g_WindowHeight - (g_WindowHeight * 0.6f),
	},
	sourceHydroPump
	{
		g_HydroPumpSourcePosition.x,
		g_HydroPumpSourcePosition.y,
		g_HydroPumpTexture.width,
		g_HydroPumpTexture.height/4
	},
	destinationHydroPump
	{
		g_HydroPumpDestinationPosition.x,
		g_HydroPumpDestinationPosition.y,
		g_EnemyPokemon.position.width,
		g_EnemyPokemon.position.height,
	},
	sourceDragonRage
	{
		g_DragonRageSourcePosition.x,
		g_DragonRageSourcePosition.y,
		g_DragonRageTexture.width,
		g_DragonRageTexture.height / 4
	},
		destinationDragonRage
	{
		g_DragonRageDestinationPosition.x,
		g_DragonRageDestinationPosition.y,
		g_EnemyPokemon.position.width,
		g_EnemyPokemon.position.height,
	};
	DrawTexture(g_BackgroundTexture, destinationBackground);
	DrawTexture(g_GyaradosTexture, g_AllyPokemon.position);
	DrawTexture(g_GodmoongussTexture, g_BossPokemon.position);
	DrawTexture(g_AttackTexture, destinationAttack);
	DrawTexture(g_InfoAllyPokemonTexture, destinationgInfoAllyPokemonTexture);
	DrawTexture(g_InfoEnemyPokemonTexture, destinationgInfoEnemyPokemonTexture);
	DrawTexture(g_GyaradosNameText, Point2f{ destinationgInfoAllyPokemonTexture.left+ 65.f, destinationgInfoAllyPokemonTexture.top-5.f});
	DrawTexture(g_GodmoongussNameText, Point2f{ destinationgInfoEnemyPokemonTexture.left + 20.f, destinationgInfoEnemyPokemonTexture.top - 5.f});
	if (g_FightingOptionsTextureIsOn)
	{
		DrawTexture(g_FightingOptionsTexture, g_DestinationFightingOptions);
		DrawTexture(g_ArrowTexture, destinationArrowFightingOptionsTexture);
	}
	if (g_PickingMoves)
	{
		DrawTexture(g_MovesTexture, destinationMoves);
		DrawTexture(g_ArrowTexture, destinationArrowMovesTexture);
		DrawTexture(g_SurfText, g_Move1Position);
		DrawTexture(g_HydroPumpText, Point2f{ (g_Move1Position.x + g_HalfWidth * 0.65f),g_Move1Position.y });
		DrawTexture(g_DragonRageText, Point2f{ g_Move1Position.x,(g_Move1Position.y + g_HeightOfTextBlock / 3.f) });
		DrawTexture(g_HyperBeamText, Point2f{ (g_Move1Position.x + g_HalfWidth * 0.65f),(g_Move1Position.y + g_HeightOfTextBlock / 3.f) });
	}
	if (g_EnemyPokemon.attackTextureIsOn == true)
	{
		DrawTexture(g_GodmoongussAttackText, Point2f{
				g_WindowWidth / 2 - g_GodmoongussAttackText.width / 2,
				g_WindowHeight - (g_HeightOfTextBlock * 0.6f)
			});
	}
	if (g_AllyPokemon.attackTextureIsOn == true)
	{
		DrawTexture(g_GyaradosAttackText, Point2f{
				g_WindowWidth / 2 - g_GyaradosAttackText.width / 2,
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
	if (g_SurfIsOn)
	{
		DrawTexture(g_SurfTexture, destinationSurf);
	}
	if (g_HydroPumpIsOn)
	{
		g_HydroPumpSourcePosition.y = g_CurrentHydroPumpIndex* g_HydroPumpTexture.height / 4;
		DrawTexture(g_HydroPumpTexture, destinationHydroPump,sourceHydroPump);
	}
	if (g_DragonRageIsOn)
	{
		g_DragonRageSourcePosition.y = g_CurrentDragonRageIndex * g_DragonRageTexture.height / 5;
		DrawTexture(g_DragonRageTexture, destinationDragonRage, sourceDragonRage);
	}
	DrawHPBar();

}

void DrawHPBar()
{
	float alphaEnemy{ g_Godmoonguss.animHP / g_Godmoonguss.total };
	float alphaAlly{ g_Gyarados.animHP / g_Gyarados.total };
	if ((g_HPBarEnemyPokemon.width * alphaEnemy) > (g_HPBarEnemyPokemon.width / 2))
	{
		SetColor(0.44f, 0.97f, 0.66f);
		FillRect(g_HPBarEnemyPokemon.position.x, g_HPBarEnemyPokemon.position.y, (g_HPBarEnemyPokemon.width * alphaEnemy), g_HPBarEnemyPokemon.height);
	}
	else if ((g_HPBarEnemyPokemon.width * alphaEnemy) <= (g_HPBarEnemyPokemon.width / 2) &&
		(g_HPBarEnemyPokemon.width * alphaEnemy) > (g_HPBarEnemyPokemon.width / 4))
	{
		SetColor(0.97f, 0.87f, 0.2f);
		FillRect(g_HPBarEnemyPokemon.position.x, g_HPBarEnemyPokemon.position.y, (g_HPBarEnemyPokemon.width * alphaEnemy), g_HPBarEnemyPokemon.height);
	}
	else if ((g_HPBarEnemyPokemon.width * alphaEnemy) <= (g_HPBarEnemyPokemon.width / 4))
	{
		SetColor(0.97f, 0.34f, 0.2f);
		FillRect(g_HPBarEnemyPokemon.position.x, g_HPBarEnemyPokemon.position.y, (g_HPBarEnemyPokemon.width * alphaEnemy), g_HPBarEnemyPokemon.height);
	}
	if ((g_HPBarAllyPokemon.width * alphaAlly) > (g_HPBarAllyPokemon.width / 2))
	{
		SetColor(0.44f, 0.97f, 0.66f);
		FillRect(g_HPBarAllyPokemon.position.x, g_HPBarAllyPokemon.position.y, (g_HPBarAllyPokemon.width * alphaAlly), g_HPBarAllyPokemon.height);
	}
	else if ((g_HPBarAllyPokemon.width * alphaAlly) <= (g_HPBarAllyPokemon.width / 2) &&
		(g_HPBarAllyPokemon.width * alphaAlly) > (g_HPBarAllyPokemon.width / 4))
	{
		SetColor(0.97f, 0.87f, 0.2f);
		FillRect(g_HPBarAllyPokemon.position.x, g_HPBarAllyPokemon.position.y, (g_HPBarAllyPokemon.width * alphaAlly), g_HPBarAllyPokemon.height);
	}
	else if ((g_HPBarAllyPokemon.width * alphaAlly) <= (g_HPBarAllyPokemon.width / 4))
	{
		SetColor(0.97f, 0.34f, 0.2f);
		FillRect(g_HPBarAllyPokemon.position.x, g_HPBarAllyPokemon.position.y, (g_HPBarAllyPokemon.width * alphaAlly), g_HPBarAllyPokemon.height);
	}
}
#pragma endregion Draw

#pragma region Update
void UpdateBattle(float elapsedSec) {
	if (g_Attack)
	{
		if (CurrentMove == MoveOptions::topleft)
		{
			Attack(elapsedSec,g_Surf);
		}
		else if (CurrentMove == MoveOptions::topright)
		{
			Attack(elapsedSec,g_DragonRage);
		}
		else if (CurrentMove == MoveOptions::bottomleft)
		{
			Attack(elapsedSec,g_HydroPump);
		}
		else if (CurrentMove == MoveOptions::bottomright)
		{
			Attack(elapsedSec,g_HyperBeam);
		}
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
void Attack(float elapsedSec, Moves& currentMove)
{

	switch (AttackSequence)
	{
	case Phases::phase_allypokemon_move:
		Move(elapsedSec, g_AllyPokemon, 1);
		g_AllyPokemon.attackTextureIsOn = true;
		g_FightingOptionsTextureIsOn = false;
		g_PickingMoves = false;
		break;
	case Phases::phase_attack:
		AttackEffect(elapsedSec, g_BossPokemon.position.left, g_BossPokemon.position.top, g_BossPokemon.position.width, g_BossPokemon.position.height);
		break;
	case Phases::phase_enemypokemon_move:
		Move(elapsedSec, g_BossPokemon, 1);
		break;
	case Phases::phase_hpbarenemy_down:
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_DamageTaken);
			g_SoundDone = true;
		}
		Damage(g_Godmoonguss, currentMove);
		HPBarMath(g_Godmoonguss, elapsedSec);
		break;
	case Phases::phase_wait:
		g_AllyPokemon.attackTextureIsOn = false;
		Wait(elapsedSec);
		break;
	case Phases::phase_enemypokemoncounter_move:
		Move(elapsedSec, g_BossPokemon, -1);
		g_EnemyPokemon.attackTextureIsOn = true;
		break;
	case Phases::phase_attackcounter:
		AttackEffect(elapsedSec, g_AllyPokemon.position.left, g_AllyPokemon.position.top, g_AllyPokemon.position.width, g_AllyPokemon.position.height);
		break;
	case Phases::phase_allypokemoncounter_move:
		Move(elapsedSec, g_AllyPokemon, -1);
		break;
	case Phases::phase_hpbarally_down:
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_DamageTaken);
			g_SoundDone = true;
		}
		Damage(g_Gyarados, g_DragonRage);
		HPBarMath(g_Gyarados, elapsedSec);
		break;
	case Phases::phase_done:
		g_EnemyPokemon.attackTextureIsOn = false;
		if (g_Attack)
		{
			if (g_Gyarados.actual <= 0.f)
			{
				g_Gyarados.actual = 0.f;
				g_FightingOptionsTextureIsOn = false;
				g_FaintTextureIsOn = true;
				TurnOffBattle();
			}
			AttackSequence = Phases::phase_allypokemon_move;
			g_notFirstTurn = true;
			if (g_Gyarados.actual != 0.f)
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
				Heal(g_Gyarados);
				HPBarMath(g_Gyarados, elapsedSec);
				g_ItemTextureIsOn = true;
				g_FightingOptionsTextureIsOn = false;
				break;
			case Phases::phase_wait:
				g_ItemTextureIsOn = false;
				Wait(elapsedSec);
				break;
			case Phases::phase_enemypokemoncounter_move:
				g_EnemyPokemon.attackTextureIsOn = true;
				Move(elapsedSec, g_BossPokemon, -1);
				break;
			case Phases::phase_attackcounter:
				AttackEffect(elapsedSec, g_AllyPokemon.position.left, g_AllyPokemon.position.top, g_AllyPokemon.position.width, g_AllyPokemon.position.height);
				break;
			case Phases::phase_allypokemoncounter_move:
				Move(elapsedSec, g_AllyPokemon, -1);
				break;
			case Phases::phase_hpbarally_down:
				if (!g_SoundDone)
				{
					PlaySoundEffect(g_Noises.g_DamageTaken);
					g_SoundDone = true;
				}
				Damage(g_Gyarados, g_DragonRage);
				HPBarMath(g_Gyarados, elapsedSec);
				break;
			case Phases::phase_done:
				g_EnemyPokemon.attackTextureIsOn = false;
				if (g_Item)
				{
					g_ItemOnlyOnce = true;
					g_FightingOptionsTextureIsOn = true;
					if (g_Gyarados.actual <= 0.f)
					{
						g_Gyarados.actual = 0.f;
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
void AttackEffect(float elapsedSec, float attackPositionX, float attackPositionY, float floatattackWidth, float floatattackheight)
{
	static bool attackIsntGoing{ false };
	float frameRate{ 10.f };
	if (g_SurfIsOn)
	{
		float SurfMovement{ 200.f };
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_Surf);
			g_SoundDone = true;
		}
		g_SurfPosition.x += (SurfMovement * 2) * elapsedSec;
			g_SurfPosition.y -= SurfMovement * elapsedSec;
			if (g_SurfPosition.x >= g_WindowWidth)
			{
				g_SurfPosition.x = 0.f;
				g_SurfPosition.y = g_WindowHeight * 0.6f;
				attackIsntGoing = true;
				g_SurfIsOn = false;
				g_SoundDone = false;
			}
	}
	else if (g_HydroPumpIsOn)
	{
		static float HydroPumpincrementation{ 0.f };
		HydroPumpincrementation += elapsedSec;
		const int HydroPumpAnimatonTime{ static_cast<int>(HydroPumpincrementation * frameRate) };
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_Surf);
			g_SoundDone = true;
		}
		g_HydroPumpDestinationPosition.x = (attackPositionX + (floatattackWidth / 2.f)) - (g_EnemyPokemon.position.width / 1.9f);
		g_HydroPumpDestinationPosition.y = attackPositionY + (floatattackWidth / 2.f) - (g_EnemyPokemon.position.height / 1.9f);
			g_CurrentHydroPumpIndex = HydroPumpAnimatonTime % 4;
		if (HydroPumpincrementation >= 1.f)
		{
			HydroPumpincrementation = 0.f;
			g_HydroPumpSourcePosition.y = 0.f;
			attackIsntGoing = true;
			g_HydroPumpIsOn = false;
			g_SoundDone = false;
		}
		
	}
	else if (g_DragonRageIsOn)
	{
		static float DragonRageincrementation{ 0.f };
		DragonRageincrementation += elapsedSec;
		const int DragonRageAnimatonTime{ static_cast<int>(DragonRageincrementation * frameRate) };
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_Surf);
			g_SoundDone = true;
		}
		g_DragonRageDestinationPosition.x = (attackPositionX + (g_BossPokemon.position.width / 2.f)) - (g_EnemyPokemon.position.width / 1.9f);
		g_DragonRageDestinationPosition.y = attackPositionY + (g_BossPokemon.position.height / 2.f) - (g_EnemyPokemon.position.height / 1.9f);
		g_CurrentDragonRageIndex = DragonRageAnimatonTime;
		if (DragonRageincrementation >= 1.f)
		{
			DragonRageincrementation = 0.f;
			g_DragonRageSourcePosition.y = 0.f;
			attackIsntGoing = true;
			g_DragonRageIsOn = false;
			g_SoundDone = false;
		}

	}
	else if (attackIsntGoing = true)
	{
		g_SpeedAttack = 0.f;
		if (g_Attack)
		{
			if (AttackSequence == Phases::phase_attack || AttackSequence == Phases::phase_attackcounter)
				AttackSequence = static_cast<Phases>(static_cast<int>(AttackSequence) + 1);
			attackIsntGoing = false;
		}
		else if (g_Item)
		{
			if (ItemSequence == Phases::phase_attackcounter)
				ItemSequence = static_cast<Phases>(static_cast<int>(ItemSequence) + 1);
			attackIsntGoing = false;
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
		g_SavedPosition = pokemon.position.left;
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
		pokemon.position.left = g_SavedPosition;
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
	pokemon.position.left = currentX;
}
void Damage(Pokemon& hpBarForDamage, Moves& move)
{
	if (g_SavedHPDamage < 0) {
		g_SavedHPDamage = move.damage;
		hpBarForDamage.actual -= g_SavedHPDamage;
		if (hpBarForDamage.actual < 0) hpBarForDamage.actual = 0;
	}
}
void Heal(Pokemon& hpBarForHealing)
{
	g_IsHeal = true;
	if (g_SavedHPHeal < 0) {
		g_SavedHPHeal = hpBarForHealing.total;
		hpBarForHealing.actual += (g_SavedHPHeal - hpBarForHealing.actual);
	}
}
void HPBarMath(Pokemon& hpBar,float elapsedTime)
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