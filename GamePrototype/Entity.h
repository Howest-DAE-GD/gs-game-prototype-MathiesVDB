#pragma once
class Entity
{
public:
	explicit Entity();

	virtual void Draw() const;
	virtual void Move(float elapsedSec);
	virtual void Action();

	//constants
	static const int SPEED{ 250 };

private:

};

