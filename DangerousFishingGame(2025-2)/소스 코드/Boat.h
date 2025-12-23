#pragma once

#include "BaseObj.h"

class Boat :
	public BaseObj
{
public:
	Boat(void);
	virtual void SetPtStart(const CRect& rect);

	CPoint GetLineAnchor(void) const;

protected:
};

