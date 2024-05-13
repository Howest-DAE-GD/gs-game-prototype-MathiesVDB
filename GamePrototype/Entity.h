#pragma once
class Entity
{
public:
	explicit Entity();

	virtual void Draw() const;
	virtual void Move(float elapsedSec);
	virtual void Move(float elapsedSec,const Point2f& target);
	virtual void Action();

	virtual int	  GetEntityKillScore() const;
	virtual float GetHealth() const;
	virtual void TakeDamage(const float takenDamage);

	//constants
	static const int SPEED{ 250 };

private:

};

