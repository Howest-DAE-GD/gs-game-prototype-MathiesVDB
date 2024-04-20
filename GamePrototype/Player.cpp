#include "pch.h"
#include "Player.h"
#include "utils.h"

Player::Player() :
	m_Hunger{ float(MAX_HUNGER) }
{
	// nothing to do
}

void Player::Draw() const
{
	utils::DrawEllipse(m_PlayerPos, RADIUS_PLAYER, RADIUS_PLAYER, 3.f);

	ShowHunger();
}

void Player::Update(float elapsedSec)
{
	Move(elapsedSec);

	float hungerChange{ HUNGER_DECREASE * elapsedSec };
	m_Hunger -= hungerChange;
}

void Player::Move(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	// read the key values

	const bool isLeft	{ bool(pStates[SDL_SCANCODE_LEFT])  };
	const bool isRight	{ bool(pStates[SDL_SCANCODE_RIGHT]) };
	const bool isUp		{ bool(pStates[SDL_SCANCODE_UP])    };
	const bool isDown	{ bool(pStates[SDL_SCANCODE_DOWN])  };

	// update position
	if (isLeft)		m_PlayerPos.x -= SPEED * elapsedSec;
	if (isRight)	m_PlayerPos.x += SPEED * elapsedSec;
	if (isUp)		m_PlayerPos.y += SPEED * elapsedSec;
	if (isDown)		m_PlayerPos.y -= SPEED * elapsedSec;
}

void Player::Action()
{
}

void Player::ShowHunger() const
{
	Point2f hungerBarLocation{ 300, 50 };

	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	utils::FillRect(hungerBarLocation, m_Hunger * 3, 40);

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });

	utils::DrawRect(hungerBarLocation, 300, 40, 3.f);
}

void Player::AddHunger(const float hungerIncrease)
{
	m_Hunger += hungerIncrease;
}

float Player::GetHunger() const
{
	return m_Hunger;
}
