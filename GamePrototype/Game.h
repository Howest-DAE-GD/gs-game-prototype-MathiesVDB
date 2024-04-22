#pragma once
#include "BaseGame.h"

class Player;
class Victim;
class Text;

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
	static const int		MAX_VICTIMS{ 15 };
	static constexpr float	RESPAWN_TIME_VICTIMS{ 3.f };

	// Enum Class
	enum class GameState
	{
		Start, Game, GameOver
	};

	// Private 
	float m_RespawnTimer;
	bool m_IsAttacking;

	GameState m_State;

	Victim* m_VictimPtrArr[MAX_VICTIMS];
	Player* m_PlayerPtr;

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void GameScreen() const;
	void StartScreen() const;
	void GameOverScreen() const;
	void CreateVictim(const int index);
	void DeleteVictim(const int index);
	void Target(const int index);
	void CheckAttacking();
	void RespawnVictim();
};