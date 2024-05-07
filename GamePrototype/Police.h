#pragma once
#include "Entity.h"

class Police final : public Entity 
{
public:
	explicit Police();

	virtual void Draw() const;
	virtual void Move(float elapsedSec);
	virtual void Action();

private:
	
};

