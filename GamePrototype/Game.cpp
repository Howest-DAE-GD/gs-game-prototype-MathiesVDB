#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "Victim.h"
#include "Player.h"
#include "iostream"
#include "Text.h"
#include "XP.h"
#include "Button.h"

Game::Game(const Window& window)
	: BaseGame{ window },
	m_State{ GameState::Start },
	m_VictimPtrArr{},
	m_IsAttacking{},
	m_RespawnTimer{},
	m_GameOverTextPtr{},
	m_Score{},
	m_Score_As_String{"0"},
	m_FinalScoreTextPtr{},
	m_XPPtrVec{}
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

	m_UpgradeBtnPtrArr[0] = new Button(Point2f{ m_cameraPos.x + 100, m_cameraPos.y + 300 }, "Health Potion", Color4f{ 1.f, 0.f, 0.f, 1.f }, m_SmallFontPtr, 0);
	m_UpgradeBtnPtrArr[1] = new Button(Point2f{ m_cameraPos.x + 300, m_cameraPos.y + 300 }, "Damage Potion", Color4f{ 0.9f, 0.4f, 0.1f, 1.f }, m_SmallFontPtr, 1);
	m_UpgradeBtnPtrArr[2] = new Button(Point2f{ m_cameraPos.x + 500, m_cameraPos.y + 300 }, "Health Regen Potion", Color4f{ 0.3f, 0.3f, 1.f, 1.f }, m_SmallFontPtr, 2);

	ResetGame();

	m_GameOverTextPtr = new Text("GAME OVER",		Color4f{ 1.f, 1.f, 1.f, 1.f }, m_BigFontPtr);
	m_StartTextPtr	  = new Text("CLICK TO START",	Color4f{ 1.f, 1.f, 1.f, 1.f }, m_BigFontPtr);
	m_ScoreTextPtr	  = new Text("Score: ",			Color4f{ 1.f, 1.f, 1.f, 1.f }, m_SmallFontPtr);
	m_ScoreNumberPtr  = new Text(m_Score_As_String,	Color4f{ 1.f, 1.f, 1.f, 1.f }, m_SmallFontPtr);
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

	auto xpIter = m_XPPtrVec.begin();
	while (xpIter != m_XPPtrVec.end())
	{
		XP* xpDrop = *xpIter;

		delete xpDrop;
		xpIter = m_XPPtrVec.erase(xpIter);
	}

	for (int counter{}; counter < NR_OF_UPGRADES; ++counter)
	{
		delete m_UpgradeBtnPtrArr[counter];
		m_UpgradeBtnPtrArr[counter] = nullptr;
	}
}

void Game::ResetGame()
{
	m_State = GameState::Start;
	m_Score = 0;
	m_PlayerPtr->SetPosition(Point2f{ 450, 250 });
	//m_PlayerPtr->AddHealth(m_PlayerPtr->GetMaxHealth());

	for (int counter{}; counter < MAX_VICTIMS; ++counter)
	{
		DeleteVictim(counter);
	}

	for (int counter{}; counter <= 5; ++counter)
	{
		CreateVictim(counter);
	}

	for (XP* xpDrop : m_XPPtrVec)
	{
		delete xpDrop;
		xpDrop = nullptr;
	}
}

void Game::Update( float elapsedSec )
{
	m_cameraPos = Point2f{
		m_PlayerPtr->GetPlayerPos().x - GetViewPort().width / 2,
		m_PlayerPtr->GetPlayerPos().y - GetViewPort().height / 2 };

	m_RespawnTimer += elapsedSec;
	
	m_PlayerPtr->Update(elapsedSec, m_State == GameState::Game);

	bool hasAttacked{ false };

	for (int counter{}; counter < MAX_VICTIMS; ++counter)
	{
		if (m_VictimPtrArr[counter] != 0)
		{
			m_VictimPtrArr[counter]->Move(elapsedSec);

			Target(counter);

			if (m_IsAttacking && m_VictimPtrArr[counter]->IsTargetable())
			{
				m_PlayerPtr->Action(m_VictimPtrArr[counter]);

				if (m_VictimPtrArr[counter]->GetHealth() <= 0)
				{
					m_Score += m_VictimPtrArr[counter]->GetEntityKillScore();
					DeleteVictim(counter);
				}
				
				hasAttacked = true;

				if (hasAttacked)
				{
					m_IsAttacking = false;
					break;
				}
			}
		}
	}

	auto xpIter = m_XPPtrVec.begin();
	while (xpIter != m_XPPtrVec.end())
	{
		XP* xpDrop = *xpIter;

		if (utils::IsOverlapping(xpDrop->GetXPHitbox(), m_PlayerPtr->GetPlayerHitbox()))
		{
			m_PlayerPtr->AddXP(xpDrop->GetValue());

			delete xpDrop;
			xpIter = m_XPPtrVec.erase(xpIter);
		}
		else
		{
			++xpIter;
		}
	}

	if (m_PlayerPtr->IsLevelUp())
	{
		m_State = GameState::Upgrade;

		m_PlayerPtr->AddXP(-m_PlayerPtr->GetXP());
		m_PlayerPtr->ToggleLevelUp();
	}

	int counter{};
	for (Button* Btn : m_UpgradeBtnPtrArr)
	{
		if (m_UpgradeBtnPtrArr[counter]->IsPressed())
		{
			m_PlayerPtr->Upgrade(m_UpgradeBtnPtrArr[counter]->GetValue());
		}
		++counter;
	}

	if (m_RespawnTimer >= RESPAWN_TIME_VICTIMS)
	{
		RespawnVictim();

		m_RespawnTimer = 0;
	}

	if (m_PlayerPtr->GetHealth() <= 0)
	{
		m_State = GameState::GameOver;
	}

	m_Score_As_String = std::to_string(m_Score);

	delete m_ScoreNumberPtr;
	m_ScoreNumberPtr = new Text(m_Score_As_String, Color4f{ 1.f, 1.f, 1.f, 1.f }, m_SmallFontPtr);

	if (m_State == GameState::GameOver && m_FinalScoreTextPtr == 0)
	{
		m_FinalScoreTextPtr = new Text("Final Score: " + m_Score_As_String, Color4f{ 1.f, 1.f, 1.f, 1.f }, m_SmallFontPtr);
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
	case GameState::Tutorial:
		TutorialScreen();
		break;
	case GameState::Game:
		GameScreen();
   		break;
	case GameState::Upgrade:
		UpgradeScreen();
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
	glPushMatrix();
	{
		glTranslatef(-m_cameraPos.x, -m_cameraPos.y, 0);

		m_PlayerPtr->Draw(m_cameraPos);

		for (int counter{}; counter < MAX_VICTIMS; ++counter)
		{
			if (m_VictimPtrArr[counter] != 0)
			{
				m_VictimPtrArr[counter]->Draw();
			}
		}

		for (XP* xpDrop : m_XPPtrVec)
		{
			xpDrop->Draw();
		}
	}
	glPopMatrix();

	m_ScoreTextPtr->Draw  (Point2f{ 45,  GetViewPort().height - 20 });
	m_ScoreNumberPtr->Draw(Point2f{ 100, GetViewPort().height - 20 });
}

void Game::StartScreen() const
{
	m_StartTextPtr->Draw(Point2f{ GetViewPort().width / 2 - 20, GetViewPort().height / 2 + 100 });
}

void Game::GameOverScreen() const
{
	m_GameOverTextPtr->Draw(Point2f{ GetViewPort().width / 2 - 20, GetViewPort().height / 2 + 120 });
	m_FinalScoreTextPtr->Draw(Point2f{ GetViewPort().width / 2, GetViewPort().height / 2 + 40 });
}

void Game::UpgradeScreen() const
{
	for (int counter{}; counter < NR_OF_UPGRADES; ++counter)
	{
		m_UpgradeBtnPtrArr[counter]->Draw();
	}
}

void Game::TutorialScreen() const
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
		m_XPPtrVec.push_back(new XP(m_VictimPtrArr[index]->GetVictimPosition(), Type::Small));

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
	if (e.keysym.sym == SDLK_r && m_State == GameState::GameOver)
	{
		ResetGame();
	}
	if (e.keysym.sym == SDLK_SPACE && m_PlayerPtr->CanAttack())
	{
		m_IsAttacking = true;
		m_PlayerPtr->ResetAttackCooldown();
	}
	else
	{
		m_IsAttacking = false;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	for (int counter{}; counter < NR_OF_UPGRADES; ++counter)
	{
		m_UpgradeBtnPtrArr[counter]->ProcessMouseMotionEvent(e);
	}
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	m_State = GameState::Game;

	for (int counter{}; counter < NR_OF_UPGRADES; ++counter)
	{
		m_UpgradeBtnPtrArr[counter]->ProcessMouseDownEvent(e);
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	for (int counter{}; counter < NR_OF_UPGRADES; ++counter)
	{
		m_UpgradeBtnPtrArr[counter]->ProcessMouseUpEvent(e);
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}