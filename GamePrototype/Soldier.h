#pragma once
#include "Entity.h"

class Soldier final : public Entity
{
public:
	explicit	 Soldier();

	void Draw() const override;

	bool		 IsTargetable() const;

	void		 ToggleTargetable();
private:
};