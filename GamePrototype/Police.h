#pragma once
#include "Entity.h"

class Police final : public Entity 
{
public:
	explicit	 Police();

	void Draw() const override;

	bool		 IsTargetable() const;

	void		 ToggleTargetable();
private:
};