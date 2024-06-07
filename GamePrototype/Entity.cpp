#include "pch.h"
#include "Entity.h"
#include "BaseGame.h"
#include "utils.h"
#include "Player.h"
#include "iostream"

bool Entity::m_IsAggro{ false };

Entity::Entity(const int size, const int maxHealth, const int damageRange, const int damage, const int value, const int killScore)
	:	m_AttackCooldown{},
		m_Size{size},
		m_MaxHealth{maxHealth},
		m_DamageRange{damageRange},
		m_Damage{damage},
		m_KillScore{killScore},
		m_Health{(float)maxHealth},
		m_Targetable{false},
		m_Value{value},
		m_RandX{},
		m_RandY{},
		m_IdleTarget{}
{

}

void Entity::Draw() const
{

}

void Entity::Move(float elapsedSec, const Point2f& target)
{
	Vector2f velocity{};

	if (m_IsAggro)
	{
		m_AttackCooldown += elapsedSec;

		velocity = Vector2f{ target.x - m_Position.x, target.y - m_Position.y };
	}
	else
	{
		m_IdleTarget = Point2f{ m_Position.x - m_RandX, m_Position.y - m_RandY };
		if (m_Position.x == m_IdleTarget.x && m_Position.y == m_IdleTarget.y)
		{
			m_RandX = (rand() % 400) - 200;
			m_RandY = (rand() % 400) - 200;
		}
		velocity = Vector2f{ m_IdleTarget.x - m_Position.x, m_IdleTarget.y - m_Position.y };
	}

	m_Position += velocity.Normalized() * (SPEED - 50) * elapsedSec;

	m_Position = HandleBorderCollision(m_Position);
}

Point2f Entity::HandleBorderCollision(Point2f& pos)
{
	if (pos.x < 10)
	{
		pos.x += 1;
	}
	if (pos.x > 989)
	{
		pos.x -= 1;
	}
	if (pos.y < 10)
	{
		pos.y += 1;
	}
	if(pos.y > 555)
	{
		pos.y -= 1;
	}

	return pos;
}

void Entity::Action(Player* playerPtr)
{
	if (m_IsAggro && utils::IsPointInCircle(playerPtr->GetPlayerPos(), Circlef{ m_Position, (float)m_DamageRange }) && m_AttackCooldown >= ATTACK_COOLDOWN)
	{
		playerPtr->TakeDamage(m_Damage);
		m_AttackCooldown = 0;
	}
}

int Entity::GetEntityKillScore() const
{
	return m_KillScore;
}

float Entity::GetHealth() const
{
	return m_Health;
}

int Entity::GetValue() const
{
	return m_Value;
}

Point2f Entity::GetPosition() const
{
	return m_Position;
}

Rectf Entity::GetRect() const
{
	Rectf Rect{ m_Position.x, m_Position.y, (float)m_Size, (float)m_Size };
	return Rect;
}

void Entity::TakeDamage(const float takenDamage)
{
	m_Health -= takenDamage;
	m_IsAggro = true;
}

void Entity::SetPosition(const Point2f& newPos)
{
	m_Position = newPos;
}

bool Entity::IsTargetable() const
{
	return m_Targetable;
}

void Entity::ToggleTargetable()
{
	m_Targetable = !m_Targetable;
}