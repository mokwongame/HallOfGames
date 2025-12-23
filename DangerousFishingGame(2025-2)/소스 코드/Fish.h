#pragma once

#include "BaseObj.h"

class Fish :
	public BaseObj
{
public:
	Fish(void);
	virtual void SetPtStart(const CRect& rect);

	virtual int GetRad(void) const { return 25; }  

	int m_dir; // +1: 오른쪽, -1: 왼쪽
	int m_kind;    // 1 또는 2 (물고기 종류)

protected:
};

