#include "pch.h"
#include "Police.h"
#include "utils.h"

Police::Police()
    :   Entity(18, 30, 25, 10, 2, 3)
{
	m_Position = Point2f{ float(rand() % 800), float(rand() % 500) };
}

void Police::Draw() const
{
	if (m_Targetable)
	{
		utils::SetColor(Color4f{ 1.f, 0.3f, 0.3f, 1.f });
	}
	else
	{
		utils::SetColor(Color4f{ 0.f, 0.f, 1.f, 1.f });
	}

	utils::FillRect(m_Position, m_Size, m_Size);

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });

	//Show health if not 100
	if (m_Health != m_MaxHealth)
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::FillRect(Point2f{ m_Position.x - 10, m_Position.y + 30 }, (m_Health / 40) * 100, 5);

		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		utils::DrawRect(Point2f{ m_Position.x - 10, m_Position.y + 30 }, 40, 5);
	}
}

bool Police::IsTargetable() const
{
    return m_Targetable;
}

void Police::ToggleTargetable()
{
    m_Targetable = !m_Targetable;
}
