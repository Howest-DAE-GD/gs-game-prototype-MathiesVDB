#include "pch.h"
#include "Player.h"
#include "Victim.h"
#include "utils.h"
#include "iostream"

Player::Player() :
	m_Hunger{ float(MAX_HUNGER) }
{
	// nothing to do
}

void Player::Draw() const
{
	utils::DrawEllipse(m_Position, RADIUS_PLAYER, RADIUS_PLAYER, 3.f);

	utils::DrawEllipse(m_Position, KILL_RADIUS, KILL_RADIUS, 2.f);

	ShowHunger();
}

void Player::Update(float elapsedSec)
{
	Move(elapsedSec);

	float hungerChange{ HUNGER_DECREASE * elapsedSec };
	m_Hunger -= hungerChange;

	//std::cout << m_IsAttacking << std::endl;
}

void Player::Move(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	// read the key values

	const bool isLeft	{ bool(pStates[SDL_SCANCODE_LEFT ]) };
	const bool isRight	{ bool(pStates[SDL_SCANCODE_RIGHT]) };
	const bool isUp		{ bool(pStates[SDL_SCANCODE_UP   ]) };
	const bool isDown	{ bool(pStates[SDL_SCANCODE_DOWN ]) };

	// update position
	if (isLeft)		m_Position.x -= SPEED * elapsedSec;
	if (isRight)	m_Position.x += SPEED * elapsedSec;
	if (isUp)		m_Position.y += SPEED * elapsedSec;
	if (isDown)		m_Position.y -= SPEED * elapsedSec;
}

void Player::Action(Victim* victim)
{
		m_Position = victim->GetVictimPosition();

		AddHunger(20.f);
}

void Player::ShowHunger() const
{
	const Point2f hungerBarLocation{ 250, 10 };

	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	utils::FillRect(hungerBarLocation, m_Hunger * 4, 40);

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });

	utils::DrawRect(hungerBarLocation, 400, 40, 3.f);
}

void Player::AddHunger(const float hungerIncrease)
{
	m_Hunger += hungerIncrease;

	if (m_Hunger >= MAX_HUNGER)
	{
		m_Hunger = MAX_HUNGER;
	}
}

float Player::GetHunger() const
{
	return m_Hunger;
}

bool Player::IsClose(Victim* victim) const
{
	Circlef playerKillRadius{ m_Position, (float)KILL_RADIUS };

	if (utils::IsOverlapping(victim->GetVictimRect(), playerKillRadius))
	{
		return true;
	}
	return false;
}

bool Player::HasAttacked() const
{
	return false;
}
