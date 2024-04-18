#pragma once
class Victim
{
public:
	explicit Victim();

	virtual void Draw() const;
	virtual void Move(float elapsedSec);
	virtual void Attack();
private:

};

