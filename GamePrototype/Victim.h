#pragma once
#include "Entity.h"

class Victim : public Entity
{
public:
	explicit	 Victim();

	virtual void Draw() const override;
	virtual void Move(float elapsedSec) override;
	virtual void Action() override;

	virtual int   GetEntityKillScore() const override;
	virtual float GetHealth() const override;
	virtual void  TakeDamage(const float takenDamage) override;

	Rectf		 GetVictimRect();
	Point2f		 GetVictimPosition();

	bool		 IsTargetable() const;

	void		 ToggleTargetable();
private:
	//Constants
	static const	 int	VICTIM_SIZE		{  25    };
	static const	 int	KILL_SCORE		{   1    };
	static const	 int	MAX_HEALTH		{  20    };
	static constexpr float	WANDER_TIMER	{   0.5f };

	//Private Members
	Point2f m_Position;

	float	m_MoveTimer;
	float	m_Health;
	int		m_RandomDirection;
	bool	m_Targetable;
	bool	m_IsMoving;
	bool	m_HasTakenDamage;
};

