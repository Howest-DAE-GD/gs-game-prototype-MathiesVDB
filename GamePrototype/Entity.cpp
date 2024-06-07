#include "pch.h"
#include "Entity.h"
#include "BaseGame.h"
#include "utils.h"
#include "Player.h"
#include "iostream"

bool Entity::m_IsAggro{ false };

Entity::Entity(const int size, const int maxHealth, const int damageRange, const int damage, const int killScore)
	:	m_AttackCooldown{},
		m_Size{size},
		m_MaxHealth{maxHealth},
		m_DamageRange{damageRange},
		m_Damage{damage},
		m_KillScore{killScore},
		m_Health{(float)maxHealth},
		m_PreviousPos{}
{

}

void Entity::Draw() const
{

}

void Entity::Move(float elapsedSec, const Point2f& target)
{
	if (m_IsAggro)
	{
		m_AttackCooldown += elapsedSec;

		Vector2f velocity{ target.x - m_Position.x, target.y - m_Position.y };
		m_Position += velocity.Normalized() * (SPEED - 50) * elapsedSec;
	}
	else
	{

	}

	m_Position = HandleBorderCollision(m_Position, m_PreviousPos);

	m_PreviousPos = m_Position;
}

Point2f Entity::HandleBorderCollision(Point2f& pos, const Point2f& prevPos)
{
	if (pos.x < 10
		|| pos.x > 989)
	{
		pos.x = prevPos.x;
	}
	if (pos.y < 10
		|| pos.y > 555)
	{
		pos.y = prevPos.y;
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
	return 0;
}

float Entity::GetHealth() const
{
	return 0.0f;
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