#pragma once
#include "Entity.h"

class Player;

class Victim final: public Entity
{
public:
	explicit	 Victim();

	void Draw() const override;
private:
	
};

