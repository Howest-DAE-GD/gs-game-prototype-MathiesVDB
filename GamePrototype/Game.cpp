#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "Victim.h"
#include "Player.h"
#include "iostream"
#include "Text.h"

Game::Game(const Window& window)
	: BaseGame{ window },
	m_State{ GameState::Start },
	m_VictimPtrArr{},
	m_IsAttacking{},
	m_RespawnTimer{},
	m_GameOverTextPtr{},
	m_Score{},
	m_Score_As_String{"0"}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_BigFontPtr	= TTF_OpenFont("Jersey15-Regular.ttf", 120);
	m_SmallFontPtr	= TTF_OpenFont("Jersey15-Regular.ttf", 30);
	m_PlayerPtr		= new Player();

	//create initial 5 victims
	for (int counter{}; counter <= 5; ++counter)
	{
		CreateVictim(counter);
	}

	m_GameOverTextPtr = new Text("GAME OVER",		Color4f{ 1.f, 1.f, 1.f, 1.f }, m_BigFontPtr);
	m_StartTextPtr	  = new Text("CLICK TO START",	Color4f{ 1.f, 1.f, 1.f, 1.f }, m_BigFontPtr);
	m_ScoreTextPtr	  = new Text("Score: ",			Color4f{ 1.f, 1.f, 1.f, 1.f }, m_SmallFontPtr);
	m_ScoreNumberPtr = new Text(m_Score_As_String,	Color4f{ 1.f, 1.f, 1.f, 1.f }, m_SmallFontPtr);
}

void Game::Cleanup( )
{
	for (int counter{}; counter < MAX_VICTIMS; ++counter)
	{
		DeleteVictim(counter);
	}

	delete m_PlayerPtr;
	delete m_GameOverTextPtr;
	delete m_StartTextPtr;
	delete m_ScoreTextPtr;
	delete m_ScoreNumberPtr;

	m_PlayerPtr			= nullptr;
	m_GameOverTextPtr	= nullptr;
	m_StartTextPtr	    = nullptr;
	m_ScoreTextPtr	    = nullptr;
	m_ScoreNumberPtr    = nullptr;

	if (m_BigFontPtr != nullptr)
	{
		TTF_CloseFont(m_BigFontPtr);
		m_BigFontPtr = nullptr;
	}

	if (m_SmallFontPtr != nullptr)
	{
		TTF_CloseFont(m_SmallFontPtr);
		m_SmallFontPtr = nullptr;
	}

}

void Game::Update( float elapsedSec )
{
	m_RespawnTimer += elapsedSec;

	CheckAttacking();
	
	m_PlayerPtr->Update(elapsedSec, m_State == GameState::Game);

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

				++m_Score;

				return;
			}
		}
	}

	if (m_RespawnTimer >= RESPAWN_TIME_VICTIMS)
	{
		RespawnVictim();

		m_RespawnTimer = 0;
	}

	if (m_PlayerPtr->GetHunger() <= 0)
	{
		m_State = GameState::GameOver;
	}

	m_Score_As_String = std::to_string(m_Score);

	delete m_ScoreNumberPtr;
	m_ScoreNumberPtr = new Text(m_Score_As_String, Color4f{ 1.f, 1.f, 1.f, 1.f }, m_SmallFontPtr);
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
		GameOverScreen();
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

	m_ScoreTextPtr->Draw  (Point2f{ 45,  GetViewPort().height - 20 });
	m_ScoreNumberPtr->Draw(Point2f{ 100, GetViewPort().height - 20 });
}

void Game::StartScreen() const
{
	m_StartTextPtr->Draw(Point2f{ GetViewPort().width / 2 - 20, GetViewPort().height / 2 + 100 });
}

void Game::GameOverScreen() const
{
	m_GameOverTextPtr->Draw(Point2f{ GetViewPort().width / 2 - 20, GetViewPort().height / 2 + 100 });
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
	m_State = GameState::Game;
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}