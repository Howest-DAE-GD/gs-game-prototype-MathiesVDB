#include "pch.h"
#include "Player.h"
#include "Victim.h"
#include "XP.h"
#include "utils.h"
#include "iostream"

Player::Player() :
	Entity(10, 100, 35, 10),
	m_Exp{},
	m_HasTarget{},
	m_TimeSinceLastFight{},
	m_CanAttack{true},
	m_IsLevelUp{false},
	m_HealthRegen{0.1f}
{
	// nothing to do
}

void Player::Draw(const Point2f& cameraPos) const
{
	utils::FillEllipse(m_Position, m_Size, m_Size);

	ShowStats(cameraPos);
}

void Player::Update(float elapsedSec, bool isPlaying)
{
	m_AttackCooldown += elapsedSec;
	m_TimeSinceLastFight += elapsedSec;

	if (m_AttackCooldown >= ATTACK_COOLDOWN)
	{
		m_CanAttack = true;
	}
	else
	{
		m_CanAttack = false;
	}

	if (m_TimeSinceLastFight >= OUT_OF_COMBAT_REGEN_TIME)
	{
		AddHealth(m_HealthRegen);
	}

	if (isPlaying)
	{
		Move(elapsedSec);
	}
}

void Player::Move(float elapsedSec, const Point2f& target)
{
	Vector2f velocity{};
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	// read the key values

	const bool isLeft	{ bool(pStates[SDL_SCANCODE_LEFT ]) };
	const bool isRight	{ bool(pStates[SDL_SCANCODE_RIGHT]) };
	const bool isUp		{ bool(pStates[SDL_SCANCODE_UP   ]) };
	const bool isDown	{ bool(pStates[SDL_SCANCODE_DOWN ]) };

	// update position
	if (isLeft)		velocity.x -= SPEED;
	if (isRight)	velocity.x += SPEED;
	if (isUp)		velocity.y += SPEED;
	if (isDown)		velocity.y -= SPEED;

	m_Position += velocity.Normalized() * SPEED * elapsedSec;

	HandleBorderCollision(m_Position, m_PreviousPos);

	m_PreviousPos = m_Position;
}

void Player::Action(Victim* victim)
{	
	victim->TakeDamage(m_Damage);
	m_TimeSinceLastFight = 0;
}

void Player::ShowStats(const Point2f& cameraPos) const
{
	const Point2f xpBarLocation	   { cameraPos.x		, cameraPos.y	   };
	const Point2f healthBarLocation{ cameraPos.x + 348	, cameraPos.y + 7  };
	
	//Draw health bar
	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	utils::FillRect(healthBarLocation, m_Health / m_MaxHealth * 100, 10);

	//Draw outline health bar
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawRect(healthBarLocation, 100, 10, 1.5f);

	//Draw XP bar
	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::FillRect(xpBarLocation, m_Exp * 4.5f, 5);
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
	m_TimeSinceLastFight = 0;
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
		m_MaxHealth += 5;
		std::cout << m_Health << std::endl;
		break;
	case 1:
		m_Damage += 2;
		std::cout << m_Damage << std::endl;
		break;
	case 2:
		m_HealthRegen *= UPGRADE_INCREMENT;
		break;
	}
}

bool Player::IsClose(Victim* victim) const
{
	Circlef playerKillRadius{ m_Position, (float)m_DamageRange };

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

Point2f Player::GetPlayerPos() const
{
	return m_Position;
}

Circlef Player::GetPlayerHitbox() const
{
	Circlef playerHitbox{ m_Position, (float)m_Size };
	return playerHitbox;
}
