#pragma once
#include <utils.h>
using namespace utils;

//		--- CONST VARIABLES ---

//		--- ENUM & STRUCTS ---

enum class AttackPhase
{
	phase_lax_forward,
	phase_lax_backward,
	phase_attack,
	phase_gmoonguss_forward,
	phase_gmoonguss_backward,
	phase_hpbar1_down,
	phase_wait,
	phase_gmoongusscounter_backward,
	phase_gmoongusscounter_forward,
	phase_attackcounter,
	phase_laxcounter_backward,
	phase_laxcounter_forward,
	phase_hpbar2_down,
	phase_done
};

enum class ItemPhase
{
	phase_hpbar2_up,
	phase_itemwait,
	phase_itemgmoongusscounter_backward, // maybe there's a way like you said to avoid this repetition ?
	phase_itemgmoongusscounter_forward,
	phase_itemattackcounter,
	phase_itemlaxcounter_backward,
	phase_itemlaxcounter_forward,
	phase_itemhpbar2_down,
	phase_itemdone
};

struct HPBar {
	float
		hPBarHitAmmount{},
		hPBarTarget{},
		hPBarWidth{g_WindowWidth * 0.3125f };
};

struct Pokemon
{

};


//		--- VARIABLES ---


float
g_LaxManX{ g_WindowWidth * 0.0625f },
g_LaxManY{ g_WindowHeight * 0.255f },
g_GodmoongussX{ g_WindowWidth * 0.59375f },
g_GodmoongussY{ g_WindowHeight * 0.025f },
g_SpeedLax{ 200.f },
g_SpeedAttack{ 0.f },
g_SpeedGmoonguss{ 200.f },
g_SpeedHPBar{ 50.f },
g_PhaseWaitCounter{ 0.f },
g_MovementLength{ 55.f }; // aren't some of these supposed to be const ? if yes write in -- CONST VAR -- above

bool
g_Attack{},
g_Item{},
g_Run{},
g_Switch{},
g_notFirstTurn{},
g_LaxAttackTextureIsOn{},
g_WaitTextBlock{},
g_GMoongussAttackTextureIsOn{}, // this could be in the struct
g_ItemTextureIsOn{},
g_SwitchTextureIsOn{},
g_RunTextureIsOn{},
g_ItemDoneTextureIsOn{},
g_NotFirstTurnTextureIsOn{},
g_FaintTextureIsOn{};


utils::Texture
g_BackgroundTexture{},
g_LaxManTexture{},
g_GreenTexture{},
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
g_FaintTexture{}; // you need a function to init and one to delete them

// normally you don't need these source variables because the struct Texture has width ad height when you load an image in it (texture.width)
Point2f attackSpriteSize{ g_WindowWidth * -0.99375f, g_WindowHeight * -0.025f };
Point2f	textBlockSpriteSize{ g_WindowWidth * 0.0062548866f, g_WindowHeight * 0.674196351f };
Point2f g_BackgroundPosition{ 0.f,0.f };


// and * 0.0062548866f -> also you can define it's position directly on the screen but you could also define it compared to another thing
//for ex maybe it's just 10px under the ennemy position

// all those could be Point2f, but also maybe it'd easier to have a struct for the character and the ennemy 
// and you create an instance of that struct of each of them since they have common variables and behaviour


HPBar HPBarEnemyPokemon{
	g_WindowWidth * 0.03125f,
	(g_WindowWidth * 0.3125f) - (g_WindowWidth * 0.03125f) // isn't this just 0 ? also use the other's variables if they have a link (like if enemy bar == ally witdh/ 2)
};
HPBar HPBarAllyPokemon{
	g_WindowWidth * 0.0625f,
	(g_WindowWidth * 0.3125f) - (g_WindowWidth * 0.0625f)
};

AttackPhase Attackphase = AttackPhase::phase_lax_forward;
ItemPhase Itemphase = ItemPhase::phase_hpbar2_up; // init with {}


struct Rects // you could also try to define their position from another variable
{
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
	},
	HpBarEnemy
	{
		g_WindowWidth * 0.1875f,
		g_WindowHeight * 0.14f,
		HPBarEnemyPokemon.hPBarWidth,
		g_WindowHeight * 0.0217125f,
	},
	HpBarAlly
	{
		g_WindowWidth * 0.58625f,
		g_WindowHeight * 0.57625f,
		HPBarAllyPokemon.hPBarWidth,
		g_WindowHeight * 0.0217125f,
	};
};

//actually try to fin a better way to deal with this because as you said it's a struct acting like a variable, i can't put it above with the other structs bc of initialization timing issues
//you could create the rectf like this without a struct 
// also instead of always windowWidth * value, you can sometimes use windowwidth - offset (as in 10px or so)
Rects g_Rects; // -> init !! 

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
void	LaxForward(float elapsedSec); // maybe try to have a MoveForward where you send lax or gmoongus as argument
void	LaxBackward(float elapsedSec);
void	AttackEffect(float elapsedSec, float attackPositionX, float attackPositionY);
void	GMoongussForward(float elapsedSec);
void	GMoongussBackward(float elapsedSec);
void	HPBar1Down(float elapsedSec);
void	Wait(float elapsedSec);
void	HPBar2Down(float elapsedSec);
void	HPBar2Up(float elapsedSec);

//		UTILS



