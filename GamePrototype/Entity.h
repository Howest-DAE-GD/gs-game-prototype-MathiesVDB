#pragma once
class Entity
{
public:
	explicit Entity();

	virtual void Draw() const;
	virtual void Move(float elapsedSec);
	virtual void Attack();
private:

};

