#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	explicit Player();

	virtual void Draw() const;
	virtual void Move(float elapsedSec);
	virtual void Attack();
private:

};

