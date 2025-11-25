#pragma once
#include <utils.h>
#include <map>
using namespace utils;

#pragma region gameInformation
// TODO: Set your name and group in the title here
std::string g_WindowTitle{ "Pokestyle Game Project - Rahimov, Javid & Rondia, Louise - 1DAE12" };

// Change the window dimensions here
float g_WindowWidth{ 768 };
float g_WindowHeight{ 768 };
#pragma endregion gameInformation

#pragma region ownDeclarations
// Declare your own global variables here

// --- /!\ GLOBAL /!\ ---


bool	g_IsBattleOn{};		// press B to switch from battle to map view and mechanics


// --- /!\ MAP & TILES PART /!\ ---

const int g_NrScenes{ 1 };

enum class CharacterAnimState {
	IdleDown,
	IdleUp,
	IdleLeft,
	IdleRight,
	WalkDown,
	WalkUp,
	WalkLeft,
	WalkRight,
};

struct AnimFrame {
	int row{};
	int col{};
	int nrFrames{};
};

struct Character {
	Rectf		dst{};
	Rectf		src{ 0.f, 0.f, 16.f, 24.f };
	Point2f		dir{};
	float		vx{};
	float		vy{};
	AnimFrame	curAnimFrame;
	Point2f		frameDimensions{ 16.f, 24.f };
	int			frameStartIndex{};
	int			frameIndex{};
	Texture		texture{};
};

// make function to check if not enough to the side of map, then character moves and not map (for shops, black borders)
struct Scene {
	Texture	texture{};
	Rectf	dst{};
	float	startOffset{};
	float	screenWidth{};
	float	screenHeight{};
};

struct Camera {
	Point2f pos{};
	float	zoom{};
};

struct World {
	Scene	scenes[g_NrScenes]{};
	int		currentSceneIndex{};
};

const float	g_TileSize{ 32.f };

World		g_World{};
Character	g_Character{};
Camera		g_Camera{};

std::map<std::string, AnimFrame> g_AnimFrames{};
const int	g_CharacterNrFrames{ 12 }; // maybe should be nrCol and rows
const float g_MoveSpeed{ 100.f };
float		g_FrameTime{};


void	InitWorld();
void	InitCamera();
void	InitCharacter();
void	InitAnimFrames();
void	DrawMap();
void	DrawCharacter();
void	UpdateMapPos(float elapsedSec);
void	UpdateCameraPos(float elapsedSec);
void	UpdateCharacterDirSpeed(const Uint8* pStates, float elapsedSec);
void	UpdateCharacterFrame(const Uint8* pStates, float elapsedSec);
void	FreeWorld();

bool	IsBetweenTiles();
bool	IsPosInCenterX(float pos);
bool	IsPosInCenterY(float pos);


Texture g_Level1Texture{},
g_MCTexture{};

const int
g_Map1SourceSizeWidth{ 640 }, // normally you don't need these source variables because the struct Texture has width ad height when you load an image in it (texture.width)
g_Map1SourceSizeHeight{ 576 },
g_Map1DestinationSizeWidth{ 3840 },
g_Map1DestinationSizeHeight{ 3456 },
g_MCSourceSizeWidth{ 15 },
g_MCSourceSizeHeight{ 16 },
g_MCDestinationSize{ 96 },
g_ArraySizeMap1{ (g_Map1SourceSizeWidth / 8) * (g_Map1SourceSizeHeight / 8) };

Point2f arrTiles[g_ArraySizeMap1]{};

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
g_FaintTexture{}; // you need a function to init and one to delete them



Point2f
attackSpriteSize
{
	g_WindowWidth * -0.99375f,
	g_WindowHeight * -0.025f
},
textBlockSpriteSize
{
	g_WindowWidth * 0.0062548866f,
	g_WindowHeight * 0.674196351f
};
// and * 0.0062548866f -> also you can define it's position directly on the screen but you could also define it compared to another thing
//for ex maybe it's just 10px under the ennemy position

//<<<<<<< HEAD
//=======

// all those could be Point2f, but also maybe it'd easier to have a struct for the character and the ennemy 
// and you create an instance of that struct of each of them since they have common variables and behaviour
//>>>>>>> 11cc9d77b11657326443a0f7dee026b9d921aa13

struct HPBar
{
	float
		hPBarHitAmmount,
		hPBarTarget,
		hPBarWidth = (g_WindowWidth * 0.3125f);
};

HPBar
HPBarEnemyPokemon
{
	g_WindowWidth * 0.03125f,
	(g_WindowWidth * 0.3125f) - (g_WindowWidth * 0.03125f)
},
HPBarAllyPokemon
{
	g_WindowWidth * 0.0625f,
	(g_WindowWidth * 0.3125f) - (g_WindowWidth * 0.0625f)
};

struct Pokemon
{

};
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
g_MovementLength{ 55.f };

bool
g_Attack{ false },
g_Item{ false },
g_Run{ false },
g_Switch{ false },
g_notFirstTurn{ false },
g_LaxAttackTextureIsOn{ false },
g_WaitTextBlock{ false },
g_GMoongussAttackTextureIsOn{ false }, // this could be in the struct
g_ItemTextureIsOn{ false },
g_SwitchTextureIsOn{ false },
g_RunTextureIsOn{ false },
g_ItemDoneTextureIsOn{ false },
g_NotFirstTurnTextureIsOn{ false },
g_FaintTextureIsOn{ false };
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
AttackPhase Attackphase = AttackPhase::phase_lax_forward;
ItemPhase Itemphase = ItemPhase::phase_hpbar2_up;
struct Rects // you could also try to define their position from another variable
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
Rects g_Rects;

Point2f g_BackgroundPosition{ 1.f,1.f };
// Declare your own functions here
void DrawEverything();
void Attack(float elapsedSec);
void Item(float elapsedSec);
void Switch(float elapsedSec);
void RunAway(float elapsedSec);
void LaxForward(float elapsedSec); // maybe try to have a MoveForward where you send lax or gmoongus as argument
void LaxBackward(float elapsedSec);
void AttackEffect(float elapsedSec, float attackPositionX, float attackPositionY);
void GMoongussForward(float elapsedSec);
void GMoongussBackward(float elapsedSec);
void HPBar1Down(float elapsedSec);
void Wait(float elapsedSec);
void HPBar2Down(float elapsedSec);
void HPBar2Up(float elapsedSec);

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
