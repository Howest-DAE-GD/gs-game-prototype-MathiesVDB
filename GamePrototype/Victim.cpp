#include "pch.h"
#include "Victim.h"
#include "utils.h"
#include "BaseGame.h"
#include "iostream"

Victim::Victim() :
	m_MoveTimer	{},
	m_Targetable{ false },
	m_IsMoving	{ false },
	m_RandomDirection{},
	m_Health{MAX_HEALTH}
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

void Victim::Move(float elapsedSec)
{
	m_MoveTimer += elapsedSec;
	if (m_MoveTimer >= WANDER_TIMER)
	{
		if (m_IsMoving)
		{
			m_MoveTimer = -1.f;
		}
		else
		{
			m_MoveTimer = (rand() % 10) / 10;
		}
		m_IsMoving = !m_IsMoving;
		m_RandomDirection = rand() % 4;
	}
	if(m_IsMoving)
	{
		if (m_RandomDirection == 0)	m_Position.x -= (SPEED - 100) * elapsedSec;
		if (m_RandomDirection == 1)	m_Position.x += (SPEED - 100) * elapsedSec;
		if (m_RandomDirection == 2)	m_Position.y += (SPEED - 100) * elapsedSec;
		if (m_RandomDirection == 3)	m_Position.y -= (SPEED - 100) * elapsedSec;
	}

	m_HasTakenDamage = false;
}

void Victim::Action()
{

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