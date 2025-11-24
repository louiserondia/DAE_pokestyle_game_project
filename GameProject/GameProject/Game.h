#pragma once
#include <utils.h>
using namespace utils;

#pragma region gameInformation
// TODO: Set your name and group in the title here
std::string g_WindowTitle{ "Pokestyle Game Project - Rahimov, Javid & Rondia, Louise - 1DAE12" };

// Change the window dimensions here
float g_WindowWidth{ 800 };
float g_WindowHeight{ 800 };
#pragma endregion gameInformation



#pragma region ownDeclarations
// Declare your own global variables here

// --- /!\ GLOBAL /!\ ---


bool	g_IsBattleOn{};		// press B to switch from battle to map view and mechanics


// --- /!\ MAP & TILES PART /!\ ---

const int g_NrScenes{ 1 };

struct Character {
	Point2f pos{};
	Point2f	dir{};
	float vx{};
	float vy{};
};

struct Scene {
	Texture	texture{};
	Point2f	pos{};
	float	zoom{};
	Rectf	src{};
	Rectf	dst{};
};

struct World {
	Scene	scenes[g_NrScenes]{};
	int		currentSceneIndex{};
};

World	g_World{};

const float	g_TileSize{ 32.f };

void	InitWorld();
void	DrawMap();
void	UpdateMapPos();
void	UpdateCharacterDir();
void	FreeWorld();


Texture g_Level1Texture{},
g_MCTexture{};

const int
g_Map1SourceSizeWidth{ 640 },
g_Map1SourceSizeHeight{ 576 },
g_Map1DestinationSizeWidth{ 3840 },
g_Map1DestinationSizeHeight{ 3456 },
g_MCSourceSizeWidth{ 15 },
g_MCSourceSizeHeight{ 16 },
g_MCDestinationSize{ 96 },
g_ArraySizeMap1{ (g_Map1SourceSizeWidth / 8) * (g_Map1SourceSizeHeight / 8) };

Point2f ArrTiles[g_ArraySizeMap1]{};

// Declare your own functions here

void InitializeTiles();
void DrawMap1();
void DrawMC();


// --- /!\ BATTLE PART /!\ ---

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
g_FaintTexture{};

float
g_AttackX{ g_WindowWidth * -0.99375f },
g_AttackY{ g_WindowHeight * -0.025f },
g_GodmoongussAttackX{ g_WindowWidth * -0.99375f },
g_GodmoongussAttackY{ g_WindowHeight * -0.025f },
g_LaxAttackX{ g_WindowWidth * -0.99375f },
g_LaxAttackY{ g_WindowHeight * -0.025f },
g_WaitX{ g_WindowWidth * -0.99375f },
g_WaitY{ g_WindowHeight * -0.025f },
g_ItemX{ g_WindowWidth * -0.99375f },
g_ItemY{ g_WindowHeight * -0.025f },
g_SwitchX{ g_WindowWidth * -0.99375f },
g_SwitchY{ g_WindowHeight * -0.025f },
g_RunX{ g_WindowWidth * -0.99375f },
g_RunY{ g_WindowHeight * -0.025f },
g_ItemDoneX{ g_WindowWidth * -0.99375f },
g_ItemDoneY{ g_WindowHeight * -0.025f },
g_NotFirstTurnX{ g_WindowWidth * -0.99375f },
g_NotFirstTurnY{ g_WindowHeight * -0.025f },
g_FaintX{ g_WindowWidth * -0.99375f },
g_FaintY{ g_WindowHeight * -0.025f };

const int
g_BackgroundSizeWidth{ 1279 },
g_BackgroundSizeHeight{ 1151 },
g_PokemonSize{ 1176 },
g_TextBlockSizeWidth{ 1280 },
g_TextBlockSizeHeight{ 306 };

float
g_LaxManX{ g_WindowWidth * 0.0625f },
g_LaxManY{ g_WindowHeight * 0.255f },
g_GodmoongussX{ g_WindowWidth * 0.59375f },
g_GodmoongussY{ g_WindowHeight * 0.025f },
g_HPBarWidth1{ g_WindowWidth * 0.3125f },
g_HPBarWidth2{ g_WindowWidth * 0.3125f };

bool
g_Move{ false },
g_Item{ false },
g_Run{ false },
g_Switch{ false },
g_notFirstTurn{ false };
static float HPBar2Target = g_HPBarWidth2 - (g_WindowWidth * 0.0625f);
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
	phase_itemgmoongusscounter_backward,
	phase_itemgmoongusscounter_forward,
	phase_itemattackcounter,
	phase_itemlaxcounter_backward,
	phase_itemlaxcounter_forward,
	phase_itemhpbar2_down,
	phase_itemdone
};

struct Buttons
{
	Rectf
		fightButton
	{
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
	HpBar1
	{
		g_WindowWidth * 0.1875f,
		g_WindowHeight * 0.14f,
		g_HPBarWidth1,
		g_WindowHeight * 0.0217125f,
	},
	HpBar2
	{
		g_WindowWidth * 0.58625f,
		g_WindowHeight * 0.57625f,
		g_HPBarWidth2,
		g_WindowHeight * 0.0217125f,
	};
};
Buttons g_Buttons;

Point2f g_BackgroundPosition{ 1.f,1.f };
// Declare your own functions here
void DrawEverything();
void Attack();
void Item();
void Switch();
void RunAway();
#pragma endregion ownDeclarations

#pragma region gameFunctions											
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling
