#pragma once
#include "BaseCar.h"

class Player :
	public BaseCar
{
public:
	Player(void);

	virtual void SetPtStart(const CRect& rect);

protected:
};

