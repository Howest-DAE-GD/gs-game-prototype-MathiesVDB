#pragma once
#include "Entity.h"

class Player;

class Victim final: public Entity
{
public:
	explicit	 Victim(Player* playerPtr);

	void Draw() const override;
	void Move(float elapsedSec, const Point2f& target) override;
	void Action() override;

	int   GetEntityKillScore() const override;
	float GetHealth() const override;
	void  TakeDamage(const float takenDamage) override;
	void  SetPosition(const Point2f& newPos) override;

	Rectf		 GetVictimRect();
	Point2f		 GetVictimPosition();

	bool		 IsTargetable() const;

	void		 ToggleTargetable();
private:
	//Constants
	static const	 int	VICTIM_SIZE		{ 25   };
	static const	 int	KILL_SCORE		{  1   };
	static const	 int	MAX_HEALTH		{ 20   };
	static const	 int	DAMAGE_RANGE	{ 25   };
	static const     int	DAMAGE          {  5   };
	static constexpr float	ATTACK_COOLDOWN {  1.f };

	//Private Members
	Point2f m_Position;

	float	m_Health;
	float	m_AttackCooldown;
	bool	m_Targetable;
	bool	m_HasTakenDamage;

	Player* m_PlayerPtr;
};

