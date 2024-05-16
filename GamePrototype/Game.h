#pragma once
#include "BaseGame.h"
#include "vector"

class Player;
class Victim;
class Text;
class XP;
class Button;

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	//Constants
	static const int		MAX_VICTIMS			{ 15	};
	static constexpr float	RESPAWN_TIME_VICTIMS{  1.5f };
	static const int		NR_OF_UPGRADES		{  3    };

	// Enum Class
	enum class GameState
	{
		Start, Game, GameOver, Tutorial, Upgrade
	};

	// Private 
	float		m_RespawnTimer;
	bool		m_IsAttacking;
	int			m_Score;
	std::string m_Score_As_String;
	Point2f		m_cameraPos;
	std::vector<XP*> m_XPPtrVec;

	GameState   m_State;

	Victim*		m_VictimPtrArr[MAX_VICTIMS];
	Button*		m_UpgradeBtnPtrArr[NR_OF_UPGRADES];
	Player*		m_PlayerPtr;
	TTF_Font*	m_BigFontPtr;
	TTF_Font*	m_SmallFontPtr;
	Text*		m_GameOverTextPtr;
	Text*		m_StartTextPtr;
	Text*		m_ScoreTextPtr;
	Text*		m_ScoreNumberPtr;
	Text*		m_FinalScoreTextPtr;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void GameScreen() const;
	void StartScreen() const;
	void GameOverScreen() const;
	void UpgradeScreen() const;
	void TutorialScreen() const;
	void CreateVictim(const int index);
	void DeleteVictim(const int index);
	void Target(const int index);
	void RespawnVictim();
	void ResetGame();
	void ResolveCollision(Victim* victim1, Victim* victim2);
	void ResolveCollision(Victim* victim , Player* player);
};