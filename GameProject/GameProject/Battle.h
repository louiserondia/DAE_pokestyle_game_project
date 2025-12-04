#pragma once
#include <utils.h>
using namespace utils;

//		--- CONST VARIABLES ---
const float
g_SpeedHPBar{ 50.f },
g_MovementLength{ 55.f },
g_AttackSpeed{ 200.f },
g_HeightOfTextBlock{ g_WindowHeight * 0.3f },
g_HalfWidth{ g_WindowWidth / 2.f };

const int
g_AmmountOfMoves{ 6 };
//		--- ENUM & STRUCTS ---

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



struct Moves
{
	float
		damage{};
};
struct Pokemon
{
	float hp{100.f};
	Moves arrMoves[g_AmmountOfMoves]{};
};
struct HPBar
{
	Point2f position
	{
		0.f,
		0.f,
	};
	float
		width{ g_WindowWidth * 0.23f }, //full width of the bar
		height{ g_WindowHeight * 0.0175f }, //full height of the bar
		total{ 100.f },
		actual{ total },
		animHP{ actual };

};
struct PokemonInBattle
{
	Point2f
		position{};
	bool
		attackTextureIsOn{};
};


//		--- VARIABLES ---
PokemonInBattle
AllyPokemon
{
	Point2f
	{
	g_WindowWidth /10,
	g_WindowHeight - (g_HeightOfTextBlock * 2)
	}
},
EnemyPokemon
{
	Point2f
	{
	(g_WindowWidth / 2) + 50.f,
	g_WindowHeight - (g_HeightOfTextBlock * 1.75f) - (20 + (g_HeightOfTextBlock * 1.32f))
	}
};
HPBar HPBarAllyPokemon
{
	Point2f
	{
		g_WindowWidth - (g_WindowWidth * 0.269f),
		g_WindowHeight - (g_HeightOfTextBlock * 1.43f),
	}
},
HPBarEnemyPokemon{
	Point2f
	{
		g_WindowWidth * 0.1875f,
		g_WindowHeight * 0.106f,
	}
};
Moves Tackle
{
	10.f
};
Moves StrongTackle
{
	20.f
};
Rectf fightButton{
		g_WindowWidth * 0.494125f,
		g_WindowHeight * 0.77125f,
		g_WindowWidth * 0.26f,
		g_WindowHeight * 0.06625f,
},
pokemonButton
{
	g_WindowWidth * 0.789625f,
	fightButton.top,
	g_WindowWidth * 0.1211875f,
	fightButton.height,
},
itemButton
{
	fightButton.left,
	g_WindowHeight * 0.88125f,
	fightButton.width,
	g_WindowHeight * 0.0608125f,
},
runButton
{
	pokemonButton.left,
	itemButton.top,
	g_WindowWidth * 0.15625f,
	itemButton.height,
};
float
g_SpeedAttack{ 0.f },
g_PhaseWaitCounter{ 0.f },
g_SavedPosition{ -1 },
g_PhaseDoneCounter{ 0.f },
g_MovementAnimAlpha{ 0.f },
g_HPBarTarget{},
g_AnimationTime{ 1.f / 0.6f };

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
g_NotFirstTurnTextureIsOn{},
g_FaintTextureIsOn{},
g_ItemOnlyOnce{};


utils::Texture
g_BackgroundTexture{},
g_LaxManTexture{},
g_InfoAllyPokemonTexture{},
g_GodmoongussTexture{},
g_AttackTexture{},
g_GodmoongussAttackTexture{},
g_LaxAttackTexture{},
g_WaitTexture{},
g_ItemTexture{},
g_SwitchTexture{},
g_RunTexture{},
g_ItemDoneTexture{},
g_NotFirstTurnTexture{},
g_FaintTexture{},
g_FightingOptionsTexture{},
g_InfoEnemyPokemonTexture{};

Point2f attackSpriteSize{ g_WindowWidth * -0.99375f, g_WindowHeight * -0.025f };
Point2f	textBlockSpriteSize{ g_WindowWidth * 0.0062548866f, g_WindowHeight * 0.674196351f };
Point2f g_BackgroundPosition{ 0.f,0.f };


// and * 0.0062548866f -> also you can define it's position directly on the screen but you could also define it compared to another thing
//for ex maybe it's just 10px under the ennemy position

Phases AttackSequence{ Phases::phase_allypokemon_move };
Phases ItemSequence{ Phases::phase_hpbarally_up };


// you could also try to define their position from another variable

// also instead of always windowWidth * value, you can sometimes use windowwidth - offset (as in 10px or so)


//		--- FUNCTIONS ---

//		INIT

void	InitBattle();


//		END


//		INPUT HANDLING

void	HandleMouseUpBattle(const SDL_MouseButtonEvent& e);

//		DRAW

void	DrawBattle();

//		UPDATE

void	UpdateBattle(float elapsedSec);
void	Attack(float elapsedSec);
void	Item(float elapsedSec);
void	Switch(float elapsedSec);
void	RunAway(float elapsedSec);
void	AttackEffect(float elapsedSec, float attackPositionX, float attackPositionY);
void	Move(float elapsedSec, PokemonInBattle& pokemon, int dir);
void	Wait(float elapsedSec);
void	HPBarAllyUp(float elapsedSec);
void	Damage(HPBar& hpBarForDamage, Moves& move);
void	Heal(HPBar& hpBar);
void	HPBarMath( HPBar& hpBar, float elapsedTime);

//		UTILS



