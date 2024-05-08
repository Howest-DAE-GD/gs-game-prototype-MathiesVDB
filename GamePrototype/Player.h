#pragma once
#include "Entity.h"

class Victim;
class XP;

class Player final : public Entity
{
public:
	explicit Player();

	virtual void Draw(const Point2f& cameraPos) const;
	virtual void Move(float elapsedSec) override;
	virtual void Action(Victim* victim);

	void Update(float elapsedSec, bool isPlaying);
	void ShowStats(const Point2f& cameraPos) const;

	void  AddHealth(const float hearthIncrease);
	float GetHealth() const;
	int	  GetMaxHealth() const;

	void  AddXP(XP* xpDrop);
	float GetXP() const;
	int	  GetMaxXP() const;
	bool  IsLevelUp() const;
	void  ToggleLevelUp();
	void  Upgrade(const int upgradeIndex);

	bool IsClose(Victim* victim) const;
	bool HasAttacked() const;
	bool CanAttack() const;

	void SetPosition(const Point2f& newPos);
	Point2f GetPlayerPos() const;
	Circlef GetPlayerHitbox() const;

private:
	//enum Classes
	enum class Direction
	{
		Up, Right, Down, Left
	};

	//Constants
	const float UPGRADE_INCREMENT{   0.1f };
	const float ATTACK_COOLDOWN	 {	 1.f  };
	const int   RADIUS_PLAYER	 {  15    };
	const int   MAX_HEALTH		 { 100    };
	const int   XP_THRESHHOLD	 { 100    };
	const int	KILL_RADIUS		 {  35    };

	//Variables
	float m_Health;
	float m_Exp;
	float m_AttackCooldown;
	int	  m_Damage;
	bool  m_CanAttack;
	bool  m_HasTarget;
	bool  m_IsLevelUp;

	Point2f m_Position{ 450, 250 };

	Direction m_Direction{ Direction::Right };
};

