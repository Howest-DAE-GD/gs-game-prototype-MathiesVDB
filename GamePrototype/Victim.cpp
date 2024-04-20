#include "pch.h"
#include "Victim.h"
#include "utils.h"
#include "BaseGame.h"

Victim::Victim() :
	m_MoveTimer{}
{
	m_Position = Point2f{ float(rand() % 800), float(rand() % 500) };
}

void Victim::Draw() const
{
	utils::DrawRect(m_Position, VICTIM_SIZE, VICTIM_SIZE);
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
