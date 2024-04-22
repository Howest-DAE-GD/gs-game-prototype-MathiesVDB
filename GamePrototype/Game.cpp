#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "Victim.h"
#include "Player.h"
#include "iostream"
#include "Text.h"

Game::Game( const Window& window ) 
	:	BaseGame{ window },
		m_State{GameState::Start},
		m_VictimPtrArr{},
		m_IsAttacking{},
		m_RespawnTimer{}
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
		DeleteVictim(counter);
	}
}

void Game::Update( float elapsedSec )
{
	m_RespawnTimer += elapsedSec;

	CheckAttacking();
	
	m_PlayerPtr->Update(elapsedSec);

	for (int counter{}; counter < MAX_VICTIMS; ++counter)
	{
		if (m_VictimPtrArr[counter] != 0)
		{
			m_VictimPtrArr[counter]->Move(elapsedSec);

			Target(counter);

			if (m_IsAttacking && m_VictimPtrArr[counter]->IsTargetable())
			{
				m_PlayerPtr->Action(m_VictimPtrArr[counter]);

				DeleteVictim(counter);
			}
		}
	}

	if (m_RespawnTimer >= RESPAWN_TIME_VICTIMS)
	{
		RespawnVictim();

		m_RespawnTimer = 0;
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

void Game::DeleteVictim(const int index)
{
	if (m_VictimPtrArr[index] != 0)
	{
		delete m_VictimPtrArr[index];
		m_VictimPtrArr[index] = nullptr;
	}
}

void Game::Target(const int index)
{
	if (m_PlayerPtr->IsClose(m_VictimPtrArr[index]))
	{
		if (!m_VictimPtrArr[index]->IsTargetable())
		{
			m_VictimPtrArr[index]->ToggleTargetable();
		}
	}
	else
	{
		if (m_VictimPtrArr[index]->IsTargetable())
		{
			m_VictimPtrArr[index]->ToggleTargetable();
		}
	}
}

void Game::CheckAttacking()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	bool isAttack{ bool(pStates[SDL_SCANCODE_SPACE]) };

	m_IsAttacking = isAttack;
}

void Game::RespawnVictim()
{
	for (int counter{}; counter < MAX_VICTIMS; ++counter)
	{
		if (m_VictimPtrArr[counter] == 0)
		{
			CreateVictim(counter);
			return;
		}
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