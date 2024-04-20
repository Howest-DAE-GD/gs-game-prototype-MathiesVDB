#include "pch.h"
#include "Victim.h"
#include "utils.h"

Victim::Victim()
{
}

void Victim::Draw() const
{
	utils::DrawRect(m_Position, VICTIM_SIZE, VICTIM_SIZE);
}

void Victim::Move(float elapsedSec)
{
}

void Victim::Action()
{
}
