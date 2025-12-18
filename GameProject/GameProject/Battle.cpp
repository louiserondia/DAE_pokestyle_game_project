#include "pch.h"
#include "Core.h"
#include "Battle.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>

void TurnOffBattle();
void DrawTextFromString(const std::string& str, const Point2f& pos, int fontSize = 100, bool isBlack = 1);

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
	LoadSoundEffect(g_Noises.g_Toxic, "../Resources/Toxic.wav");
	LoadSoundEffect(g_Noises.g_RazorLeaf, "../Resources/RazorLeaf.wav");
	LoadSoundEffect(g_Noises.g_Surf, "../Resources/SurfSound.wav");
	LoadSoundEffect(g_Noises.g_DragonRage, "../Resources/DragonRage.wav");
	LoadSoundEffect(g_Noises.g_HyperBeam, "../Resources/HyperBeam.wav");
	LoadSoundEffect(g_Noises.g_DamageTaken, "../Resources/DamageSoundEffect.wav");
}
void PlayMusicBattle() {
	PlayMusic(g_Noises.g_GodmungussBattleMusic);
}
void InitSprites()
{

	TextureFromFile("Resources/BackgroundGrass.png", g_BackgroundGrassTexture);
	TextureFromFile("Resources/BackgroundCave.png", g_BackgroundCaveTexture);
	TextureFromFile("Resources/FightingOptions.png", g_FightingOptionsTexture);
	TextureFromFile("Resources/Gyarados.png", g_GyaradosTexture);
	TextureFromFile("Resources/Sandslash.png", g_SandslashTexture);
	TextureFromFile("Resources/Victreebel.png", g_VictreebelTexture);
	TextureFromFile("Resources/InfoAllyPokemon.png", g_InfoAllyPokemonTexture);
	TextureFromFile("Resources/InfoEnemyPokemon.png", g_InfoEnemyPokemonTexture);
	TextureFromFile("Resources/GodmoongussColor.png", g_GodmoongussTexture);
	TextureFromFile("Resources/ArrowforOptions.png", g_ArrowTexture);
	TextureFromFile("Resources/MovesOptions.png", g_MovesTexture);
	TextureFromFile("Resources/Surf.png", g_SurfTexture);
	TextureFromFile("Resources/HydroPump.png", g_HydroPumpTexture);
	TextureFromFile("Resources/DragonRage.png", g_DragonRageTexture);
	TextureFromFile("Resources/RazorLeaf.png", g_RazorLeafTexture);
	TextureFromFile("Resources/HyperBeam.png", g_HyperBeamTexture);
	TextureFromFile("Resources/Paras.png", g_ParasTexture);
	TextureFromFile("Resources/Toxic.png", g_ToxicTexture);
	TextureFromFile("Resources/Parasect.png", g_ParasectTexture);
}
void InitText()
{
	std::cout << "Music and Godmoonguss sprites by Jasper Bouchet" << std::endl;
}
void Reset(int pokemonId)
{
	g_Gyarados.actual = g_Gyarados.total;
	arrWildBushPokemon[g_PickedPokemon].actual = arrWildBushPokemon[g_PickedPokemon].total;
	g_Gyarados.animHP = g_Gyarados.total;
	arrWildBushPokemon[g_PickedPokemon].animHP = arrWildBushPokemon[g_PickedPokemon].total;
	g_PickedPokemon = pokemonId;
	if (g_PickedPokemon == 0)
	{
		g_EnemyPokemon.position.top = 50.f;
	}
	else
	{
		g_EnemyPokemon.position.top = 0.f;
	}
	if (g_NotFirstTurn == true)
	{
		g_NotFirstTurn = false;
	}
	g_ItemOnlyOnce = false;
	ItemSequence = Phases::phase_hpbarally_up;
	
}
#pragma endregion Init

#pragma region End
void FreeBattle()
{
	DeleteTexture(g_BackgroundGrassTexture);
	DeleteTexture(g_BackgroundCaveTexture);
	DeleteTexture(g_SurfTexture);
	DeleteTexture(g_HydroPumpTexture);
	DeleteTexture(g_DragonRageTexture);
	DeleteTexture(g_RazorLeafTexture);
	DeleteTexture(g_HyperBeamTexture);
	DeleteTexture(g_GyaradosTexture);
	DeleteTexture(g_InfoAllyPokemonTexture);
	DeleteTexture(g_SandslashTexture);
	DeleteTexture(g_MovesTexture);
	DeleteTexture(g_ArrowTexture);
	DeleteTexture(g_VictreebelTexture);
	DeleteTexture(g_GodmoongussTexture);
	DeleteTexture(g_FightingOptionsTexture);
	DeleteTexture(g_InfoEnemyPokemonTexture);
	Mix_FreeMusic(g_Noises.g_GodmungussBattleMusic);
	Mix_FreeChunk(g_Noises.g_ArrowMove);
	Mix_FreeChunk(g_Noises.g_DamageTaken);
	Mix_FreeChunk(g_Noises.g_Surf);
	Mix_FreeChunk(g_Noises.g_HyperBeam);
	Mix_FreeChunk(g_Noises.g_DragonRage);
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
					if(g_PickedPokemon==4)
					{
						g_Run = true;
					}
					else
					{
						TurnOffBattle();
					}
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
					if (!g_DidHyperBeamLastTurn)
					{
						g_Attack = true;
						g_HyperBeamIsOn = true;
					}
					else
					{
						g_HyperBeamTextIsOn = true;
					}
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
	};
	const int fontsize{ 60 };
	if (g_PickedPokemon == 0 || g_PickedPokemon == 1)
	{
		DrawTexture(g_BackgroundGrassTexture, destinationBackground);
	}
	else
	{
		DrawTexture(g_BackgroundCaveTexture, destinationBackground);
	}
	DrawTexture(g_GyaradosTexture, g_AllyPokemon.position);
	DrawTexture(g_InfoAllyPokemonTexture, destinationgInfoAllyPokemonTexture);
	DrawTexture(g_InfoEnemyPokemonTexture, destinationgInfoEnemyPokemonTexture);

	DrawTextFromString(g_Gyarados.name, Point2f{ destinationgInfoAllyPokemonTexture.left + 75.f, destinationgInfoAllyPokemonTexture.top + 5.f}, fontsize);
	DrawTextFromString(arrWildBushPokemon[g_PickedPokemon].name, Point2f{ destinationgInfoEnemyPokemonTexture.left + 30.f, destinationgInfoEnemyPokemonTexture.top + 5.f }, fontsize);

	if (g_IsBossFight)
	{
		DrawTexture(g_GodmoongussTexture, g_EnemyPokemon.position);
	}
	if (g_PickedPokemon == 0)
	{
		DrawTexture(*arrWildBushPokemon[g_PickedPokemon].pokemonSprite, g_EnemyPokemon.position);
	}
	if (g_PickedPokemon == 1)
	{
		DrawTexture(*arrWildBushPokemon[g_PickedPokemon].pokemonSprite, g_EnemyPokemon.position);
	}
	if (g_PickedPokemon == 2)
	{
		DrawTexture(*arrWildBushPokemon[g_PickedPokemon].pokemonSprite, g_EnemyPokemon.position);
	}
	if (g_PickedPokemon == 3)
	{
		DrawTexture(*arrWildBushPokemon[g_PickedPokemon].pokemonSprite, g_EnemyPokemon.position);
	}
	if (g_PickedPokemon == 4)
	{
		DrawTexture(*arrWildBushPokemon[g_PickedPokemon].pokemonSprite, g_EnemyPokemon.position);
	}
	if (g_FightingOptionsTextureIsOn)
	{
		DrawTexture(g_FightingOptionsTexture, g_DestinationFightingOptions);
		DrawTexture(g_ArrowTexture, destinationArrowFightingOptionsTexture);
	}
	if (g_PickingMoves)
	{
		DrawTexture(g_MovesTexture, destinationMoves);
		DrawTexture(g_ArrowTexture, destinationArrowMovesTexture);
	}
	DrawTexts();
	DrawHPBar();
	DrawAttackEffects();

}

void DrawHPBar()
{
	float alphaEnemy{ arrWildBushPokemon[g_PickedPokemon].animHP / arrWildBushPokemon[g_PickedPokemon].total };
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
void DrawTexts()
{
	const int fontsize{ 60 };
	if (g_PickingMoves)
	{
		const float offsetHor{ g_HalfWidth * 0.65f }, offsetVert{ g_HeightOfTextBlock / 3.f };
		const Point2f origin{ (g_HalfWidth * 0.075f) + g_DestinationFightingOptions.width * 0.05f * 1.5f,
			g_WindowHeight - g_HeightOfTextBlock * 0.80f };

		DrawTextFromString(g_Gyarados.arrMoves[0].name, origin, fontsize);
		DrawTextFromString(g_Gyarados.arrMoves[1].name, Point2f{ (origin.x + g_HalfWidth * 0.65f),origin.y }, fontsize);
		DrawTextFromString(g_Gyarados.arrMoves[2].name, Point2f{ origin.x, (origin.y + g_HeightOfTextBlock / 3.f) }, fontsize);
		DrawTextFromString(g_Gyarados.arrMoves[3].name, Point2f{ (origin.x + g_HalfWidth * 0.65f), (origin.y + g_HeightOfTextBlock / 3.f) }, fontsize);
	}
	const Point2f textPos{ Point2f{ 65.f, g_WindowHeight - g_HeightOfTextBlock * 0.70f } };
	const int fontSize{ 80 };

	if (g_EnemyPokemon.attackTextureIsOn == true)
	{
		DrawTextFromString(arrWildBushPokemon[g_PickedPokemon].name + g_Texts.retaliation + g_Gyarados.name, Point2f{ 65.f, g_WindowHeight - g_HeightOfTextBlock * 0.60f }, 60, false);
	}
	if (g_AllyPokemon.attackTextureIsOn == true)
	{
		DrawTextFromString(g_Gyarados.name + g_Texts.attack + arrWildBushPokemon[g_PickedPokemon].name, textPos, fontSize, false);
	}
	if (g_WaitTextBlock == true)
	{
		DrawTextFromString(g_Texts.wait + arrWildBushPokemon[g_PickedPokemon].name, textPos, fontSize, false);
	}
	if (g_ItemTextureIsOn == true) // only for us ? 
	{
		DrawTextFromString(g_Gyarados.name + g_Texts.item, textPos, fontSize, false);
	}
	if (g_SwitchTextureIsOn == true)
	{
		DrawTextFromString(g_Texts.switchText, textPos, fontSize, false);
	}
	if (g_RunTextureIsOn == true)
	{
		DrawTextFromString(g_Texts.runText, textPos, fontSize, false);
	}
	if (g_ItemDoneTextureIsOn == true)
	{
		DrawTextFromString(g_Texts.itemDoneText, textPos, fontSize, false);
	}
	if (g_NotFirstTurnTextureIsOn == true)
	{
		DrawTextFromString(g_Texts.notFirstTurnText, textPos, fontSize, false);
	}
	if (g_FaintTextureIsOn == true)
	{
		DrawTextFromString(g_Gyarados.name + g_Texts.faint, textPos, fontSize, false);
	}
	if (g_FaintTextureEnemyIsOn == true)
	{
		DrawTextFromString(arrWildBushPokemon[g_PickedPokemon].name + g_Texts.faint, textPos, fontSize, false);
	}
	if (g_HyperBeamTextIsOn == true)
	{
		DrawTextFromString(g_Texts.hyperbeam, Point2f{ 65.f, g_WindowHeight - g_HeightOfTextBlock * 0.60f }, 60,false);
	}
}
void DrawAttackEffects()
{
	Rectf
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
		g_HydroPumpTexture.height / 4
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
		g_DragonRageTexture.height / 5
	},
	destinationDragonRage
	{
		g_DragonRageDestinationPosition.x,
		g_DragonRageDestinationPosition.y,
		g_EnemyPokemon.position.width,
		g_EnemyPokemon.position.height,
	},
	sourceHyperBeam
	{
		g_HyperBeamSourcePosition.x,
		g_HyperBeamSourcePosition.y,
		g_HyperBeamTexture.width,
		g_HyperBeamTexture.height / 6
	},
	destinationHyperBeam
	{
		g_HyperBeamDestinationPosition.x,
		g_HyperBeamDestinationPosition.y,
		g_EnemyPokemon.position.width,
		g_EnemyPokemon.position.height,
	},
	sourceSlash
	{
		g_SlashSourcePosition.x,
		g_SlashSourcePosition.y,
		g_DragonRageTexture.width,
		g_DragonRageTexture.height / 5
	},
	destinationSlash
	{
		g_SlashDestinationPosition.x,
		g_SlashDestinationPosition.y,
		g_AllyPokemon.position.width,
		g_AllyPokemon.position.height,
	},
	sourceRazorLeaf
	{
		g_RazorLeafSourcePosition.x,
		g_RazorLeafSourcePosition.y,
		g_RazorLeafTexture.width,
		g_RazorLeafTexture.height / 5
	},
	destinationRazorLeaf
	{
		g_RazorLeafDestinationPosition.x,
		g_RazorLeafDestinationPosition.y,
		g_AllyPokemon.position.width,
		g_AllyPokemon.position.height,
	},
	sourceToxic
	{
		g_ToxicSourcePosition.x,
		g_ToxicSourcePosition.y,
		g_ToxicTexture.width,
		g_ToxicTexture.height / 5
	},
	destinationToxic
	{
		g_ToxicDestinationPosition.x,
		g_ToxicDestinationPosition.y,
		g_AllyPokemon.position.width,
		g_AllyPokemon.position.height,
	};
	if (AttackSequence == Phases::phase_attack)
	{
		if (g_SurfIsOn)
		{
			DrawTexture(g_SurfTexture, destinationSurf);
		}
		if (g_HydroPumpIsOn)
		{
			g_HydroPumpSourcePosition.y = g_CurrentHydroPumpIndex * g_HydroPumpTexture.height / 4;
			DrawTexture(g_HydroPumpTexture, destinationHydroPump, sourceHydroPump);
		}
		if (g_DragonRageIsOn)
		{
			g_DragonRageSourcePosition.y = g_CurrentDragonRageIndex * g_DragonRageTexture.height / 5;
			DrawTexture(g_DragonRageTexture, destinationDragonRage, sourceDragonRage);
		}
		if (g_HyperBeamIsOn)
		{
			g_HyperBeamSourcePosition.y = g_CurrentHyperBeamIndex * g_HyperBeamTexture.height / 6;
			DrawTexture(g_HyperBeamTexture, destinationHyperBeam, sourceHyperBeam);
		}
	}
	if (g_SlashIsOn)
	{
		g_SlashSourcePosition.y = g_CurrentSlashIndex * g_DragonRageTexture.height / 5;
		DrawTexture(g_DragonRageTexture, destinationSlash, sourceSlash);
	}
	if (g_RazorLeafIsOn)
	{
		g_RazorLeafSourcePosition.y = g_CurrentRazorLeafIndex * g_RazorLeafTexture.height / 5;
		DrawTexture(g_RazorLeafTexture, destinationRazorLeaf, sourceRazorLeaf);
	}
	if (g_ToxicIsOn)
	{
		g_ToxicSourcePosition.y = g_CurrentToxicIndex * g_ToxicTexture.height / 5;
		DrawTexture(g_ToxicTexture, destinationToxic, sourceToxic);
	}
}
#pragma endregion Draw

#pragma region Update
void UpdateBattle(float elapsedSec) {
	if (g_Attack)
	{
		if (CurrentMove == MoveOptions::topleft)
		{
			Attack(elapsedSec, g_Surf);
		}
		else if (CurrentMove == MoveOptions::topright)
		{
			Attack(elapsedSec, g_DragonRage);
		}
		else if (CurrentMove == MoveOptions::bottomleft)
		{
			Attack(elapsedSec, g_HydroPump);
		}
		else if (CurrentMove == MoveOptions::bottomright)
		{
			Attack(elapsedSec, g_HyperBeam);
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


	if(g_HyperBeamTextIsOn)
	{
		HyperBeamText(elapsedSec);
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
		AttackEffect(elapsedSec, g_EnemyPokemon.position.left, g_EnemyPokemon.position.top, g_EnemyPokemon.position.width, g_EnemyPokemon.position.height);
		break;
	case Phases::phase_enemypokemon_move:
		Move(elapsedSec, g_EnemyPokemon, 1);
		break;
	case Phases::phase_hpbarenemy_down:
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_DamageTaken);
			g_SoundDone = true;
		}
		Damage(arrWildBushPokemon[g_PickedPokemon], currentMove);
		HPBarMath(arrWildBushPokemon[g_PickedPokemon], elapsedSec);
		break;
	case Phases::phase_wait:
		g_AllyPokemon.attackTextureIsOn = false;
		Wait(elapsedSec);
		break;
	case Phases::phase_enemypokemoncounter_move:
		Move(elapsedSec, g_EnemyPokemon, -1);
		g_EnemyPokemon.attackTextureIsOn = true;
		break;
	case Phases::phase_attackcounter:
		AttackEffectEnemy(elapsedSec);
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
		Damage(g_Gyarados, arrWildBushPokemon[g_PickedPokemon].arrMoves[rand()%4]);
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
			if (arrWildBushPokemon[g_PickedPokemon].actual <= 0.f)
			{
				arrWildBushPokemon[g_PickedPokemon].actual = 0.f;
				g_FightingOptionsTextureIsOn = false;
				g_FaintTextureEnemyIsOn = true;
				TurnOffBattle();
			}
			AttackSequence = Phases::phase_allypokemon_move;
			g_NotFirstTurn = true;
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
	if (g_NotFirstTurn == true)
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
				Move(elapsedSec, g_EnemyPokemon, -1);
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
				Damage(g_Gyarados, arrWildBushPokemon[g_PickedPokemon].arrMoves[rand() % 4]);
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
void HyperBeamText(float elapsedSec)
{
	if (g_DidHyperBeamLastTurn == true)
	{
		g_FightingOptionsTextureIsOn = false;
		g_PickingMoves = false;
		const float HyperBeamIncrementation{ 50.f };
		g_PhaseWaitCounter += HyperBeamIncrementation*elapsedSec;
		if (g_PhaseWaitCounter >= 100)
		{
			g_PhaseWaitCounter = 0;
			g_HyperBeamTextIsOn = false;
			g_FightingOptionsTextureIsOn = true;
			g_PickingMoves = true;
		}
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
void AttackEffect(float elapsedSec, float attackPositionX, float attackPositionY, float attackWidth, float attackHeight)
{

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
				g_SurfIsOn = false;
				g_SoundDone = false;
				g_DidHyperBeamLastTurn = false;
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
			g_CurrentHydroPumpIndex = HydroPumpAnimatonTime % 4;
			if (HydroPumpincrementation >= 1.f)
			{
				HydroPumpincrementation = 0.f;
				g_HydroPumpSourcePosition.y = 0.f;
				g_HydroPumpIsOn = false;
				g_SoundDone = false;
				g_DidHyperBeamLastTurn = false;
			}
		}
		else if (g_DragonRageIsOn)
		{
			static float DragonRageincrementation{ 0.f };
			DragonRageincrementation += elapsedSec;
			const int DragonRageAnimatonTime{ static_cast<int>(DragonRageincrementation * frameRate) };
			if (!g_SoundDone)
			{
				PlaySoundEffect(g_Noises.g_DragonRage);
				g_SoundDone = true;
			}
			g_CurrentDragonRageIndex = DragonRageAnimatonTime % 5;
			if (DragonRageincrementation >= 0.5f)
			{
				DragonRageincrementation = 0.f;
				g_DragonRageSourcePosition.y = 0.f;
				g_DragonRageIsOn = false;
				g_SoundDone = false;
				g_DidHyperBeamLastTurn = false;
			}

		}
		else if (g_HyperBeamIsOn)
		{
			static float HyperBeamincrementation{ 0.f };
			HyperBeamincrementation += elapsedSec;
			const int HyperBeamAnimatonTime{ static_cast<int>(HyperBeamincrementation * frameRate) };
			if (!g_SoundDone)
			{
				PlaySoundEffect(g_Noises.g_HyperBeam);
				g_SoundDone = true;
			}
			g_CurrentHyperBeamIndex = HyperBeamAnimatonTime % 6;
			if (HyperBeamincrementation >= 0.5f)
			{
				HyperBeamincrementation = 0.f;
				g_HyperBeamSourcePosition.y = 0.f;
				g_HyperBeamIsOn = false;
				g_SoundDone = false;
				g_DidHyperBeamLastTurn = true;

			}
		}
		else
		{
			g_SpeedAttack = 0.f;
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
void AttackEffectEnemy(float elapsedSec)
{
	static bool attackIsntGoing{ false };
	float frameRate{ 10.f };
	if (g_PickedPokemon == 0 || g_PickedPokemon == 2 || g_PickedPokemon == 3)
	{
		static float Slashincrementation{ 0.f };
		Slashincrementation += elapsedSec;
		const int SlashAnimatonTime{ static_cast<int>(Slashincrementation * frameRate) };
			g_SlashIsOn = true;
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_RazorLeaf);
			g_SoundDone = true;
		}
		g_CurrentSlashIndex = SlashAnimatonTime % 5;
		if (Slashincrementation >= 0.5f)
		{
			Slashincrementation = 0.f;
			g_SlashSourcePosition.y = 0.f;
			attackIsntGoing = true;
			g_SlashIsOn = false;
			g_SoundDone = false;
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
	else if ( g_PickedPokemon == 1)
	{
		static float RazorLeafincrementation{ 0.f };
		RazorLeafincrementation += elapsedSec;
		const int RazorLeafAnimatonTime{ static_cast<int>(RazorLeafincrementation * frameRate) };
		g_RazorLeafIsOn = true;
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_DragonRage);
			g_SoundDone = true;
		}
		g_CurrentRazorLeafIndex = RazorLeafAnimatonTime % 5;
		if (RazorLeafincrementation >= 0.5f)
		{
			RazorLeafincrementation = 0.f;
			g_RazorLeafSourcePosition.y = 0.f;
			attackIsntGoing = true;
			g_RazorLeafIsOn = false;
			g_SoundDone = false;
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
	else
	{
		static float Toxicincrementation{ 0.f };
		Toxicincrementation += elapsedSec;
		const int ToxicAnimatonTime{ static_cast<int>(Toxicincrementation * frameRate) };
		g_ToxicIsOn = true;
		if (!g_SoundDone)
		{
			PlaySoundEffect(g_Noises.g_Toxic);
			g_SoundDone = true;
		}
		g_CurrentToxicIndex = ToxicAnimatonTime % 5;
		if (Toxicincrementation >= 0.5f)
		{
			Toxicincrementation = 0.f;
			g_ToxicSourcePosition.y = 0.f;
			attackIsntGoing = true;
			g_ToxicIsOn = false;
			g_SoundDone = false;
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
void HPBarMath(Pokemon& hpBar, float elapsedTime)
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