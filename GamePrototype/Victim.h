#pragma once
#include "Entity.h"

class Victim : public Entity
{
public:
	explicit Victim();

	virtual void Draw() const override;
	virtual void Move(float elapsedSec) override;
	virtual void Attack() override;
private:

};

