#pragma once
#include <utils.h>
#include "Audio.h"
using namespace utils;

#pragma region ConstVariables

const float
	g_SpeedHPBar{ 25.f },
	g_MovementLength{ 55.f },
	g_AttackSpeed{ 200.f },
	g_HeightOfTextBlock{ g_WindowHeight * 0.3f },
	g_HalfWidth{ g_WindowWidth / 2.f };

const int g_AmmountOfMoves{ 4 };

#pragma endregion ConstVariables

#pragma region Enum&Structs


struct Texts {
	// concat with the name(s) of the concerned pokemon(s) 
	const std::string attack{ " Attacks " };
	const std::string retaliation{ " retaliates with an attack on " };
	const std::string wait{ "Waiting for " };
	const std::string item{ " healed with an item" };
	const std::string faint{ " has fainted" };

	const std::string itemDoneText{ "You don't have any items left" };
	const std::string notFirstTurnText{ "Your HP is full" };
	const std::string runText{ "You can't run from a god" };
	const std::string switchText{ "You don't have pokemon to switch to" };
};

const Texts g_Texts{};

enum class Phases
{
	phase_allypokemon_move,
	phase_attack,
	phase_enemypokemon_move,
	phase_hpbarenemy_down,
	phase_wait,
	phase_enemypokemoncounter_move,
	phase_attackcounter,
	phase_allypokemoncounter_move,
	phase_hpbarally_down,
	phase_done,
	phase_hpbarally_up
};

enum class FightingOptions
{
	fight,
	bag,
	pokemon,
	run
};

enum class MoveOptions
{
	topleft,
	topright,
	bottomleft,
	bottomright
};


struct Moves
{
	std::string
		name{};
	float
		damage{};
};

struct HPBar
{
	Point2f position{};
	float
		width{ g_WindowWidth * 0.23f },
		height{ g_WindowHeight * 0.0175f };

};

struct Pokemon
{
	std::string name{};
	Moves arrMoves[g_AmmountOfMoves]{};
	float
		speed{},
		total{ },
		actual{ total },
		animHP{ actual };
};

struct PokemonInBattle
{
	Point2f
		position{};
	bool
		attackTextureIsOn{};
	HPBar hpbar{};
};

struct Sounds
{
	Mix_Music* g_GodmungussBattleMusic{};
	Mix_Chunk* g_ArrowMove{};
	Mix_Chunk* g_Surf{};
	Mix_Chunk* g_Attack{};
	Mix_Chunk* g_DamageTaken{};
};
#pragma endregion Enum&Structs

#pragma region Variables

Sounds
	g_Noises{};

PokemonInBattle
	g_AllyPokemon
	{
		Point2f
		{
			g_WindowWidth / 10,
			g_WindowHeight - (g_HeightOfTextBlock + (g_WindowWidth * 0.25f))
		}
	},
	g_EnemyPokemon
	{
		Point2f
		{
		(g_WindowWidth / 2) + 50.f,
		g_WindowHeight - (g_HeightOfTextBlock * 1.75f) - (20 + (g_HeightOfTextBlock * 1.32f))
		}
	},
	g_BossPokemon
	{
		Point2f
		{
		(g_WindowWidth * 0.575f),
		0.f
		}
	};

HPBar 
	g_HPBarAllyPokemon
	{
		Point2f
		{
			g_WindowWidth - (g_WindowWidth * 0.269f),
			g_WindowHeight - (g_HeightOfTextBlock * 1.43f),
		}
	},
	g_HPBarEnemyPokemon{
		Point2f
		{
			g_WindowWidth * 0.1875f,
			g_WindowHeight * 0.106f,
		}
	};
Moves
	g_Surf
	{
		"Surf",
		50.f
	},
	g_HydroPump
	{
		"HydroPump",
		70.f
	},
	g_DragonRage
	{
		"DragonRage",
		40.f
	},
	g_HyperBeam
	{
		"HyperBeam",
		100.f
	};
Rectf
	g_MoveOptionsRect{
			0.f,
			g_WindowHeight - g_HeightOfTextBlock,
			g_WindowWidth * 0.66f,
			g_HeightOfTextBlock,
	},
	g_DestinationFightingOptions
	{
			g_HalfWidth,
			g_WindowHeight - g_HeightOfTextBlock,
			g_HalfWidth,
			g_HeightOfTextBlock,
	};
Pokemon
	g_Godmoonguss
	{
		"Godmoonguss",
		g_Surf,
		g_HydroPump,
		g_DragonRage,
		g_HyperBeam,
			100.f,
			300.f
	},
	g_Gyarados
	{
		"Gyarados",
		g_Surf,
		g_HydroPump,
		g_DragonRage,
		g_HyperBeam,
			50.f,
			100.f
	};
float
	g_SpeedAttack{ 0.f },
	g_PhaseWaitCounter{ 0.f },
	g_SavedPosition{ -1 },
	g_PhaseDoneCounter{ 0.f },
	g_MovementAnimAlpha{ 0.f },
	g_HPBarTarget{},
	g_AnimationTime{ 1.f / 0.6f },
	g_SavedHPDamage{ -1 },
	g_SavedHPHeal{ -1 };

bool
	g_Attack{},
	g_Item{},
	g_Run{},
	g_Switch{},
	g_notFirstTurn{},
	g_WaitTextBlock{},
	g_ItemTextureIsOn{},
	g_SwitchTextureIsOn{},
	g_RunTextureIsOn{},
	g_ItemDoneTextureIsOn{},
	g_FightingOptionsTextureIsOn{ true },
	g_NotFirstTurnTextureIsOn{},
	g_FaintTextureIsOn{},
	g_ItemOnlyOnce{},
	g_IsHeal{},
	g_PickingMoves{},
	g_SurfIsOn{},
	g_SoundDone{};


utils::Texture
	g_BackgroundTexture{},
	g_GyaradosTexture{},
	g_InfoAllyPokemonTexture{},
	g_GodmoongussTexture{},
	g_AttackTexture{},
	g_FightingOptionsTexture{},
	g_InfoEnemyPokemonTexture{},
	g_ArrowTexture{},
	g_MovesTexture{},
	g_SurfTexture{},
	g_MoveSurfTexture{};

Point2f attackSpriteSize{ g_WindowWidth * -0.99375f, g_WindowHeight * -0.025f };
Point2f arrowSpritePositionFightingOptions{ g_HalfWidth + (g_HalfWidth * 0.075f), g_WindowHeight - g_HeightOfTextBlock + g_HeightOfTextBlock * 0.25f };
Point2f arrowSpritePositionMoves{ (g_HalfWidth * 0.075f), g_WindowHeight - g_HeightOfTextBlock *0.75f };
Point2f g_BackgroundPosition{ 0.f,0.f };
Point2f g_SurfPosition{ 0.f,g_WindowHeight * 0.6f };

Phases AttackSequence{ Phases::phase_allypokemon_move };
Phases ItemSequence{ Phases::phase_hpbarally_up };
FightingOptions CurrentFightingOption{ FightingOptions::fight };
MoveOptions CurrentMove{ MoveOptions::topleft };
#pragma endregion Variables

#pragma region Functions
#pragma region Init
void	InitBattle();
void	InitText();
void	InitSprites();
void	InitMusic();
#pragma endregion Init
#pragma region End
#pragma endregion End
#pragma region InputHandling
#pragma endregion InputHandling
#pragma region Draw
void	DrawBattle();
void	DrawHPBar();
#pragma endregion Draw
#pragma region Update
void	PlayMusicBattle();
void	UpdateBattle(float elapsedSec);
void	Attack(float elapsedSec, Moves& currentMove);
void	Item(float elapsedSec);
void	Switch(float elapsedSec);
void	RunAway(float elapsedSec);
void	AttackEffect(float elapsedSec, float attackPositionX, float attackPositionY);
void	Move(float elapsedSec, PokemonInBattle& pokemon, int dir);
void	Wait(float elapsedSec);
void	Damage(Pokemon& hpBarForDamage, Moves& move);
void	Heal(Pokemon& hpBar);
void	HPBarMath(Pokemon& hpBar, float elapsedTime);
#pragma endregion Update
#pragma endregion Functions

