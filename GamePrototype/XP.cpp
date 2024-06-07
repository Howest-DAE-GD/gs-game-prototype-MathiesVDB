#include "pch.h"
#include "XP.h"
#include "utils.h"

XP::XP(Point2f pos, Type xpType) 
	: m_Position{pos}
{
	m_XPType = xpType;

	switch (m_XPType)
	{
	case Type::Small:
		m_Value = 10;
		break;
	case Type::Medium:
		m_Value = 20;
		break;
	case Type::Large:
		m_Value = 50;
		break;
	default:
		m_Value = -1;
		break;
	}
}

void XP::Draw() const
{
	switch (m_XPType)
	{
	case Type::Small:
		utils::SetColor(Color4f{ 0.8f, 0.8f, 0.8f, 1.f });
		break;
	case Type::Medium:
		utils::SetColor(Color4f{ 0.f, 0.f, 1.f, 1.f });
		break;
	case Type::Large:
		utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
		break;
	}
	utils::FillEllipse(m_Position, RADIUS, RADIUS);

	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
	utils::DrawEllipse(m_Position, RADIUS, RADIUS, 1.f);

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });

}

int XP::GetValue() const
{
	return m_Value;
}

Circlef XP::GetXPHitbox()
{
	Circlef xpHitbox{ m_Position, RADIUS };
	return xpHitbox;
}
