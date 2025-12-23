#pragma once

#include "BaseObj.h"

class Rope :
	public BaseObj
{
public:
	Rope(void);
	virtual void SetPtStart(const CPoint& pt);
	// 충돌 반경
	virtual int GetRad(void) const { return 20; }
	CPoint GetHookTip(void) const;

protected:
};

