#pragma once

class Player;

class Entity
{
public:
	Entity(const int size, const int maxHealth, const int damageRange, const int damage, const int value = 0, const int killScore = 0);

	virtual void Draw() const;
	virtual void Move(float elapsedSec, const Point2f& target = Point2f{0,0});
	virtual void Action(Player* playerPtr);

	int	  GetEntityKillScore() const;
	float GetHealth() const;
	int	  GetValue() const;
	Rectf GetRect() const;
	Point2f GetPosition() const;

	virtual void  TakeDamage(const float takenDamage);
	void  SetPosition(const Point2f& newPos);

	Point2f HandleBorderCollision(Point2f& pos);

	bool		 IsTargetable() const;

	void		 ToggleTargetable();

	//constants
	static bool m_IsAggro;
	static const int SPEED{ 190 };
	static constexpr float	ATTACK_COOLDOWN{ 1.f };

protected:
	float	m_AttackCooldown;
	float	m_Health;
	int		m_Value;
	int		m_Size;
	int		m_MaxHealth;
	int		m_DamageRange;
	int		m_Damage;
	int		m_KillScore;
	int		m_RandX;
	int		m_RandY;
	bool	m_Targetable;

	Point2f m_Position;
	Point2f m_IdleTarget;
};

