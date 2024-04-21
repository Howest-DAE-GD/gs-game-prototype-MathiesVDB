#include "pch.h"
#include "Victim.h"
#include "utils.h"
#include "BaseGame.h"
#include "iostream"

Victim::Victim() :
	m_MoveTimer{},
	m_Targetable{ false }
{
	m_Position = Point2f{ float(rand() % 800), float(rand() % 500) };
}

void Victim::Draw() const
{
	if (m_Targetable)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	}
	else
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	}
	utils::DrawRect(m_Position, VICTIM_SIZE, VICTIM_SIZE);

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
}

void Victim::Move(float elapsedSec)
{
	m_MoveTimer += elapsedSec;
	if (m_MoveTimer >= IDLE_MOVE_TIMER)
	{
		int randomDirection{ rand() % 4 };
		if (randomDirection == 0)	m_Position.x -= SPEED * elapsedSec;
		if (randomDirection == 1)	m_Position.x += SPEED * elapsedSec;
		if (randomDirection == 2)	m_Position.y += SPEED * elapsedSec;
		if (randomDirection == 3)	m_Position.y -= SPEED * elapsedSec;

		m_MoveTimer = 0;
	}
}

void Victim::Action()
{
}

Rectf& Victim::GetVictimRect()
{
	Rectf VictimRect{ m_Position.x, m_Position.y, VICTIM_SIZE, VICTIM_SIZE };
	return VictimRect;
}

Point2f& Victim::GetVictimPosition()
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