#include "pch.h"
#include "Victim.h"
#include "utils.h"
#include "BaseGame.h"
#include "iostream"
#include "Player.h"

Victim::Victim(Player* playerPtr) :
	m_Targetable{ false },
	m_Health{MAX_HEALTH},
	m_PlayerPtr{playerPtr},
	m_HasTakenDamage{false},
	m_AttackCooldown{}
{
	m_Position = Point2f{ float(rand() % 800), float(rand() % 500) };
}

void Victim::Draw() const
{
	if (m_HasTakenDamage)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	}
	else
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	}

	utils::DrawRect(m_Position, VICTIM_SIZE, VICTIM_SIZE);

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });

	//Show health if not 100
	if(m_Health != MAX_HEALTH)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::FillRect(Point2f{ m_Position.x - 10, m_Position.y + 30 }, (m_Health / 40) * 100, 5);

		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		utils::DrawRect(Point2f{ m_Position.x - 10, m_Position.y + 30 }, 40, 5);
	}
}

void Victim::Move(float elapsedSec, const Point2f& target)
{
	m_AttackCooldown += elapsedSec;

	Vector2f velocity{ target.x - m_Position.x, target.y - m_Position.y };
	m_Position += velocity.Normalized() * (SPEED - 30) * elapsedSec;

	m_HasTakenDamage = false;
}

void Victim::Action()
{
	if (utils::IsPointInCircle(m_PlayerPtr->GetPlayerPos(), Circlef{ m_Position, DAMAGE_RANGE }) && m_AttackCooldown >= ATTACK_COOLDOWN)
	{
		m_PlayerPtr->TakeDamage(DAMAGE);
		m_AttackCooldown = 0;
	}
}

int Victim::GetEntityKillScore() const
{
	return KILL_SCORE;
}

float Victim::GetHealth() const
{
	return m_Health;
}

void Victim::TakeDamage(const float takenDamage)
{
	m_Health -= takenDamage;

	m_HasTakenDamage = true;
}

void Victim::SetPosition(const Point2f& newPos)
{
	m_Position = newPos;
}

Rectf Victim::GetVictimRect()
{
	Rectf VictimRect{ m_Position.x, m_Position.y, VICTIM_SIZE, VICTIM_SIZE };
	return VictimRect;
}

Point2f Victim::GetVictimPosition()
{
	return m_Position;
}

bool Victim::IsTargetable() const
{
	return m_Targetable;
}

void Victim::ToggleTargetable()
{
	m_Targetable = !m_Targetable;
}