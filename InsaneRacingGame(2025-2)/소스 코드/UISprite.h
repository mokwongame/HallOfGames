#pragma once
#include "BaseCar.h"

class UISprite :
	public BaseCar
{
public:
	UISprite(void);

	// 화면 중앙에 UI 스프라이트 배치
	void SetCenter(const CPoint& pt)
	{
		SetPtCtr(pt);
	}

	// 마우스 포인트가 UI 스프라이트 영역 안에 있는지 검사
	bool HitTestPoint(const CPoint& pt) const
	{
		CRect rc = MakeRect();
		return rc.PtInRect(pt) != FALSE;
	}


protected:

};