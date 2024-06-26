#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "Entity.h"
#include "Victim.h"
#include "Police.h"
#include "Soldier.h"
#include "Player.h"
#include "iostream"
#include "Text.h"
#include "XP.h"
#include "Button.h"
#include "Texture.h"
#include "CameraManager.h"

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
	m_TutorialPtr	= new Texture("TutorialScreen.png");
	m_MapPtr		= new Texture("Map.png");
	m_CameraPtr		= new CameraManager();

	m_BigFontPtr	= TTF_OpenFont("Jersey15-Regular.ttf", 120);
	m_SmallFontPtr	= TTF_OpenFont("Jersey15-Regular.ttf", 30);
	m_PlayerPtr		= new Player();

	m_UpgradeBtnPtrArr[0] = new Button(Point2f{ m_cameraPos.x + 100, m_cameraPos.y + 300 }, "Health Potion", Color4f{ 1.f, 0.f, 0.f, 1.f }, m_SmallFontPtr, 0);
	m_UpgradeBtnPtrArr[1] = new Button(Point2f{ m_cameraPos.x + 300, m_cameraPos.y + 300 }, "Damage Potion", Color4f{ 0.9f, 0.4f, 0.1f, 1.f }, m_SmallFontPtr, 1);
	m_UpgradeBtnPtrArr[2] = new Button(Point2f{ m_cameraPos.x + 500, m_cameraPos.y + 300 }, "Health Regen Potion", Color4f{ 0.3f, 0.3f, 1.f, 1.f }, m_SmallFontPtr, 2);

	ResetGame();

	m_GameOverTextPtr = new Text("GAME OVER",		Color4f{ 1.f, 1.f, 1.f, 1.f }, m_BigFontPtr);
	m_StartTextPtr	  = new Text("ATTACK YOUR FIRST VICTIM TO START",	Color4f{ 0.f, 0.f, 0.f, 1.f }, m_SmallFontPtr);
	m_TutorialTextPtr = new Text("Press T for controls", Color4f{ 0.f, 0.f, 0.f, 1.f }, m_SmallFontPtr);
	m_ScoreTextPtr	  = new Text("Score: ",			Color4f{ 1.f, 1.f, 1.f, 1.f }, m_SmallFontPtr);
	m_ScoreNumberPtr  = new Text(m_Score_As_String,	Color4f{ 1.f, 1.f, 1.f, 1.f }, m_SmallFontPtr);
}

void Game::Cleanup( )
{
	for (int counter{}; counter < MAX_VICTIMS; ++counter)
	{
		DeleteVictim(counter);
	}

	delete m_TutorialPtr;
	delete m_MapPtr;
	delete m_PlayerPtr;
	delete m_GameOverTextPtr;
	delete m_StartTextPtr;
	delete m_TutorialTextPtr;
	delete m_ScoreTextPtr;
	delete m_ScoreNumberPtr;

	m_TutorialPtr		= nullptr;
	m_MapPtr			= nullptr;
	m_PlayerPtr			= nullptr;
	m_GameOverTextPtr	= nullptr;
	m_TutorialTextPtr	= nullptr;
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
	m_VictimSpawnChance = 50;
	m_PoliceSpawnChance = 0;
	m_SoldierSpawnChance = 0;

	m_State = GameState::Start;
	m_Score = 0;
	m_PlayerPtr->SetPosition(Point2f{ 450, 250 });
	m_PlayerPtr->AddHealth(m_PlayerPtr->GetMaxHealth());
	m_PlayerPtr->AddXP(-m_PlayerPtr->GetXP());

	for (int counter{}; counter < MAX_VICTIMS; ++counter)
	{
		if (counter == 0)
		{
			m_VictimPtrArr[counter]->m_IsAggro = false;
		}
		DeleteVictim(counter);
	}

	for (int counter{}; counter <= 3; ++counter)
	{
		CreateVictim(counter);
	}

	auto xpIter = m_XPPtrVec.begin();
	while (xpIter != m_XPPtrVec.end())
	{
		XP* xpDrop = *xpIter;

		delete xpDrop;
		xpIter = m_XPPtrVec.erase(xpIter);
	}
}

void Game::Update( float elapsedSec )
{
	if (m_State == GameState::Game || m_State == GameState::Start)
	{
		m_CameraPtr->Update(m_PlayerPtr->GetPlayerPos(), GetViewPort());

		m_RespawnTimer += elapsedSec;

		m_PlayerPtr->Update(elapsedSec, true);

		bool hasAttacked{ false };

		for (int counter{}; counter < MAX_VICTIMS; ++counter)
		{
			if (m_VictimPtrArr[counter] != 0)
			{
				m_VictimPtrArr[counter]->Move(elapsedSec, m_PlayerPtr->GetPlayerPos());
				m_VictimPtrArr[counter]->Action(m_PlayerPtr);

				Target(counter);

				if (m_IsAttacking && m_VictimPtrArr[counter]->IsTargetable())
				{
					m_PlayerPtr->Action(m_VictimPtrArr[counter]);

					if (m_VictimPtrArr[counter]->GetHealth() <= 0)
					{
						m_Score += m_VictimPtrArr[counter]->GetEntityKillScore();
						DeleteVictim(counter);

						m_PoliceSpawnChance += 10;
						m_SoldierSpawnChance += 5;

						if (m_Score >= 50 && m_VictimSpawnChance >= 0)
						{
							m_VictimSpawnChance -= 5;
						}
					}

					hasAttacked = true;

					if (hasAttacked)
					{
						m_IsAttacking = false;
						break;
					}
				}

				for (int otherCounter{}; otherCounter < MAX_VICTIMS; ++otherCounter)
				{
					if (counter != otherCounter && m_VictimPtrArr[otherCounter] != nullptr)
					{
						ResolveCollision(m_VictimPtrArr[counter], m_VictimPtrArr[otherCounter]);
					}
				}

				// Check for collision with player
				ResolveCollision(m_VictimPtrArr[counter], m_PlayerPtr);

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
				m_UpgradeBtnPtrArr[counter]->ReleaseButton();
				break;
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
	}

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
		GameScreen();
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
		glScalef(2, 2, 1);
		glTranslatef(-m_CameraPtr->GetCameraPos().x, -m_CameraPtr->GetCameraPos().y, 0);

		m_MapPtr->Draw();
		m_PlayerPtr->Draw(m_CameraPtr->GetCameraPos());

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
	m_StartTextPtr->Draw(Point2f{ GetViewPort().width / 2 - 20, GetViewPort().height / 2 + 200 });
	m_TutorialTextPtr-> Draw(Point2f{ GetViewPort().width / 2, GetViewPort().height / 2 - 150 });
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
	m_TutorialPtr->Draw();
}

void Game::CreateVictim(const int index)
{
	int spawnChances{ m_VictimSpawnChance + m_PoliceSpawnChance + m_SoldierSpawnChance };
	int randNumber = rand() % spawnChances;
	if (m_VictimPtrArr[index] == 0)
	{
		if (randNumber <= 50)
		{
			m_VictimPtrArr[index] = new Victim();
		}
		else if (randNumber <= 150)
		{
			m_VictimPtrArr[index] = new Police();
		}
		else
		{
			m_VictimPtrArr[index] = new Soldier();
		}
	}
}

void Game::DeleteVictim(const int index)
{
	if (m_VictimPtrArr[index] != 0)
	{
		if (m_VictimPtrArr[index]->GetValue() == 1)
		{
			m_XPPtrVec.push_back(new XP(m_VictimPtrArr[index]->GetPosition(), Type::Small));
		}
		else if (m_VictimPtrArr[index]->GetValue() == 2)
		{
			m_XPPtrVec.push_back(new XP(m_VictimPtrArr[index]->GetPosition(), Type::Medium));
		}
		else
		{
			m_XPPtrVec.push_back(new XP(m_VictimPtrArr[index]->GetPosition(), Type::Large));
		}

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

void Game::ResolveCollision(Entity* victim1, Entity* victim2)
{
	Rectf rect1 = victim1->GetRect();
	Rectf rect2 = victim2->GetRect();

	if (utils::IsOverlapping(rect1, rect2))
	{
		// Resolve collision by separating the rectangles
		float overlapX = std::min(rect1.left + rect1.width, rect2.left + rect2.width) - std::max(rect1.left, rect2.left);
		float overlapY = std::min(rect1.bottom + rect1.height, rect2.bottom + rect2.height) - std::max(rect1.bottom, rect2.bottom);

		if (overlapX < overlapY)
		{
			if (rect1.left < rect2.left)
			{
				victim1->SetPosition(Point2f{ victim1->GetPosition().x - overlapX / 2, victim1->GetPosition().y });
				victim2->SetPosition(Point2f{ victim2->GetPosition().x + overlapX / 2, victim2->GetPosition().y });
			}
			else
			{
				victim1->SetPosition(Point2f{ victim1->GetPosition().x + overlapX / 2, victim1->GetPosition().y });
				victim2->SetPosition(Point2f{ victim2->GetPosition().x - overlapX / 2, victim2->GetPosition().y });
			}
		}
		else
		{
			if (rect1.bottom < rect2.bottom)
			{
				victim1->SetPosition(Point2f{ victim1->GetPosition().x, victim1->GetPosition().y - overlapY / 2 });
				victim2->SetPosition(Point2f{ victim2->GetPosition().x, victim2->GetPosition().y + overlapY / 2 });
			}
			else
			{
				victim1->SetPosition(Point2f{ victim1->GetPosition().x, victim1->GetPosition().y + overlapY / 2 });
				victim2->SetPosition(Point2f{ victim2->GetPosition().x, victim2->GetPosition().y - overlapY / 2 });
			}
		}
	}
}

void Game::ResolveCollision(Entity* victim, Player* player)
{
	Rectf victimRect = victim->GetRect();
	Circlef playerHitbox = player->GetPlayerHitbox();

	if (utils::IsOverlapping(victimRect, playerHitbox))
	{
		// Resolve collision by separating the victim and player
		float overlapX = std::min(victimRect.left + victimRect.width, playerHitbox.center.x + playerHitbox.radius) - std::max(victimRect.left, playerHitbox.center.x - playerHitbox.radius);
		float overlapY = std::min(victimRect.bottom + victimRect.height, playerHitbox.center.y + playerHitbox.radius) - std::max(victimRect.bottom, playerHitbox.center.y - playerHitbox.radius);

		if (overlapX < overlapY)
		{
			if (victimRect.left < playerHitbox.center.x)
			{
				victim->SetPosition(Point2f{ victim->GetPosition().x - overlapX / 2, victim->GetPosition().y });
				player->SetPosition(Point2f{ player->GetPlayerPos().x + overlapX / 2, player->GetPlayerPos().y });
			}
			else
			{
				victim->SetPosition(Point2f{ victim->GetPosition().x + overlapX / 2, victim->GetPosition().y });
				player->SetPosition(Point2f{ player->GetPlayerPos().x - overlapX / 2, player->GetPlayerPos().y });
			}
		}
		else
		{
			if (victimRect.bottom < playerHitbox.center.y)
			{
				victim->SetPosition(Point2f{ victim->GetPosition().x, victim->GetPosition().y - overlapY / 2 });
				player->SetPosition(Point2f{ player->GetPlayerPos().x, player->GetPlayerPos().y + overlapY / 2 });
			}
			else
			{
				victim->SetPosition(Point2f{ victim->GetPosition().x, victim->GetPosition().y + overlapY / 2 });
				player->SetPosition(Point2f{ player->GetPlayerPos().x, player->GetPlayerPos().y - overlapY / 2 });
			}
		}
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if (e.keysym.sym == SDLK_r && m_State == GameState::GameOver)
	{
		ResetGame();
	}
	if (e.keysym.sym == SDLK_t && m_State == GameState::Start)
	{
		m_State = GameState::Tutorial;
	}
	if (e.keysym.sym == SDLK_SPACE && m_PlayerPtr->CanAttack() && m_State != GameState::Upgrade)
	{
		m_IsAttacking = true;
		m_PlayerPtr->ResetAttackCooldown();
		m_State = GameState::Game;
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
	if (m_State == GameState::Tutorial || m_State == GameState::Upgrade)
	{
		m_State = GameState::Game;
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