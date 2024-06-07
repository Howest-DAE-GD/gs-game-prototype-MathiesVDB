#pragma once
#include "Entity.h"

class Player;

class Victim final: public Entity
{
public:
	explicit	 Victim(Player* playerPtr);

	void Draw() const override;

	int   GetEntityKillScore() const override;
	float GetHealth() const override;

	Rectf		 GetVictimRect();
	Point2f		 GetVictimPosition();

	bool		 IsTargetable() const;

	void		 ToggleTargetable();
private:
	//Private Members
	bool	m_Targetable;

	Player* m_PlayerPtr;
};

