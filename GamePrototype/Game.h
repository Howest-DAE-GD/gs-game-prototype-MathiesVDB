#pragma once
#include "BaseGame.h"

class Player;
class Victim;

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
	static const int MAX_VICTIMS{ 15 };

	// Enum Class
	enum class GameState
	{
		Start, Game, GameOver
	};

	// Private 
	Player* m_PlayerPtr;

	GameState m_State;

	Victim* m_VictimPtrArr[MAX_VICTIMS];

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void GameScreen() const;
	void StartScreen() const;
	void GameOverScreen() const;
	void CreateVictim(const int index);
};