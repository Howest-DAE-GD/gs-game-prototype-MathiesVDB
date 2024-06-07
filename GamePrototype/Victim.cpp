#include "pch.h"
#include "Victim.h"
#include "utils.h"
#include "BaseGame.h"
#include "iostream"
#include "Player.h"

Victim::Victim(Player* playerPtr) :
	Entity(18, 20, 25, 5, 1),
	m_Targetable{ false },
	m_PlayerPtr{playerPtr}
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

	utils::FillRect(m_Position, m_Size, m_Size);

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });

	//Show health if not 100
	if(m_Health != m_MaxHealth)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::FillRect(Point2f{ m_Position.x - 10, m_Position.y + 30 }, (m_Health / 40) * 100, 5);

		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		utils::DrawRect(Point2f{ m_Position.x - 10, m_Position.y + 30 }, 40, 5);
	}
}

int Victim::GetEntityKillScore() const
{
	return m_KillScore;
}

float Victim::GetHealth() const
{
	return m_Health;
}

Rectf Victim::GetVictimRect()
{
	Rectf VictimRect{ m_Position.x, m_Position.y, (float)m_Size, (float)m_Size };
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