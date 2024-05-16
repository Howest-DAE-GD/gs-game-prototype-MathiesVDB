#include "pch.h"
#include "Player.h"
#include "Victim.h"
#include "XP.h"
#include "utils.h"
#include "iostream"

Player::Player() :
	m_MaxHealth{ 100 },
	m_Health{ 10 },
	m_Exp{},
	m_HasTarget{},
	m_Damage{10},
	m_AttackCooldown{},
	m_RegenCooldown{},
	m_CanAttack{true},
	m_IsLevelUp{false},
	m_HealthRegen{0.1f}
{
	// nothing to do
}

void Player::Draw(const Point2f& cameraPos) const
{
	utils::DrawEllipse(m_Position, RADIUS_PLAYER, RADIUS_PLAYER, 3.f);

	//utils::DrawEllipse(m_Position, KILL_RADIUS, KILL_RADIUS, 2.f);

	ShowStats(cameraPos);
}

void Player::Update(float elapsedSec, bool isPlaying)
{
	m_AttackCooldown += elapsedSec;
	m_RegenCooldown  += elapsedSec;

	if (m_AttackCooldown >= ATTACK_COOLDOWN)
	{
		m_CanAttack = true;
	}
	else
	{
		m_CanAttack = false;
	}

	if (m_RegenCooldown >= REGEN_COOLDOWN)
	{
		AddHealth(m_HealthRegen);
		m_RegenCooldown = 0;
	}

	if (isPlaying)
	{
		Move(elapsedSec);
	}
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
	victim->TakeDamage(m_Damage);
}

void Player::ShowStats(const Point2f& cameraPos) const
{
	const Point2f xpBarLocation	   { cameraPos.x		, cameraPos.y	   };
	const Point2f healthBarLocation{ cameraPos.x + 690	, cameraPos.y + 20 };
	
	//Draw health bar
	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	utils::FillRect(healthBarLocation, m_Health * 2, 15);

	//Draw outline health bar
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawRect(healthBarLocation, 200, 15, 3.f);

	//Draw XP bar
	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::FillRect(xpBarLocation, m_Exp * 9, 10);
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
}

void Player::AddHealth(const float healthIncrease)
{
	m_Health += healthIncrease;

	if (m_Health >= m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

float Player::GetHealth() const
{
	return m_Health;
}

int Player::GetMaxHealth() const
{
	return m_MaxHealth;
}

void Player::TakeDamage(const float takenDamage)
{
	m_Health -= takenDamage;
}

void Player::AddXP(int xpIncrease)
{
	m_Exp += xpIncrease;

	if (m_Exp >= XP_THRESHHOLD)
	{
		ToggleLevelUp();
	}
}

float Player::GetXP() const
{
	return m_Exp;
}

int Player::GetMaxXP() const
{
	return XP_THRESHHOLD;
}

bool Player::IsLevelUp() const
{
	return m_IsLevelUp;
}

void Player::ToggleLevelUp()
{
	m_IsLevelUp = !m_IsLevelUp;
}

void Player::Upgrade(const int upgradeIndex) // index: 0 = Attack speed upgrade, 1 = Damage upgrade, 2 = Health upgrade
{
	switch(upgradeIndex)
	{
	case 0:
		m_MaxHealth *= UPGRADE_INCREMENT;
		std::cout << m_Health << std::endl;
		break;
	case 1:
		++m_Damage;
		std::cout << m_Damage << std::endl;
		break;
	case 2:
		m_HealthRegen *= UPGRADE_INCREMENT;
		break;
	}
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

bool Player::CanAttack() const
{
	return m_CanAttack;
}

void Player::ResetAttackCooldown()
{
	m_AttackCooldown = 0;
}

void Player::SetPosition(const Point2f& newPos)
{
	m_Position = newPos;
}

Point2f Player::GetPlayerPos() const
{
	return m_Position;
}

Circlef Player::GetPlayerHitbox() const
{
	Circlef playerHitbox{ m_Position, (float)RADIUS_PLAYER };
	return playerHitbox;
}
