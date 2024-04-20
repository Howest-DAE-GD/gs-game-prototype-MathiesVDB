#pragma once
#include "Entity.h"

class Victim : public Entity
{
public:
	explicit Victim();

	virtual void Draw() const override;
	virtual void Move(float elapsedSec) override;
	virtual void Action() override;
private:
	//Constants
	static const int VICTIM_SIZE{ 15 };

	//Private Members
	Point2f m_Position;
};

