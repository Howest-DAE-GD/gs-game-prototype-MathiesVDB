#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "Victim.h"
#include "Player.h"

Game::Game( const Window& window ) 
	:	BaseGame{ window },
		m_State{GameState::Start},
		m_VictimPtrArr{}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_PlayerPtr = new Player();

	//create initial 5 victims
	for (int counter{}; counter <= 5; ++counter)
	{
		CreateVictim(counter);
	}
}

void Game::Cleanup( )
{
	delete m_PlayerPtr;
	m_PlayerPtr = nullptr;

	for (int counter{}; counter < MAX_VICTIMS; ++counter)
	{
		if (m_VictimPtrArr[counter] != 0)
		{
			delete m_VictimPtrArr[counter];
			m_VictimPtrArr[counter] = nullptr;
		}
	}
}

void Game::Update( float elapsedSec )
{
	m_PlayerPtr->Update(elapsedSec);

	for (int counter{}; counter < MAX_VICTIMS; ++counter)
	{
		if (m_VictimPtrArr[counter] != 0)
		{
			m_VictimPtrArr[counter]->Move(elapsedSec);
		}
	}

	if (m_State != GameState::Game)
	{
		m_State = GameState::Game;
	}
}

void Game::Draw( ) const
{
	ClearBackground( );
	switch (m_State)
	{
	case GameState::Start:
		StartScreen();
		break;
	case GameState::Game:
		GameScreen();
		break;
	case GameState::GameOver:

		break;
	default:
		break;
	}
}

void Game::GameScreen() const
{
	m_PlayerPtr->Draw();

	for (int counter{}; counter < MAX_VICTIMS; ++counter)
	{
		if (m_VictimPtrArr[counter] != 0)
		{
			m_VictimPtrArr[counter]->Draw();
		}
	}
}

void Game::StartScreen() const
{
	
}

void Game::GameOverScreen() const
{

}

void Game::CreateVictim(const int index)
{
	if (m_VictimPtrArr[index] == 0)
	{
		m_VictimPtrArr[index] = new Victim();
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}