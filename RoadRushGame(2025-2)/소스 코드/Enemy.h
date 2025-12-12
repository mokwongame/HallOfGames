#pragma once
#include "BaseCar.h"

class Enemy :
	public BaseCar
{
public:
	Enemy(void);

	virtual void SetPtStart(const CRect& rect);

protected:
};

