#pragma once
#include <afxwin.h> // MFC의 CRect 사용
#include "SnakeGameFun.h"

class Fruit
{
public:
	Fruit(void)
	{}

	~Fruit()
	{}

	bool getEnable(void) const
	{
		return m_bEnable;
	}

	void setEnable(bool bEnable)
	{
		m_bEnable = bEnable;
	}

	void setRandPt(void)
	{
		m_pt.x = randrange(m_rtClient.left, m_rtClient.right);
		m_pt.y = randrange(m_rtClient.top, m_rtClient.bottom);
		m_rect = makeRect();
	}

	void setClientRect(const CRect& rect)
	{
		m_rtClient = rect;
		m_rtClient.left += m_nWid;
		m_rtClient.right -= m_nWid;
		m_rtClient.top += m_nHt;
		m_rtClient.bottom -= m_nHt;
	}

	bool isHit(const CRect& rect);

	virtual void draw(CDC* pDC);

	CRect getRect() const { return m_rect; } // 과일의 위치 반환

protected:
	int m_nWid = 83;
	int m_nHt = 81;
	CPoint m_pt;	
	CRect m_rect;	
	CRect m_rtClient;	
	bool m_bEnable = false;

	CRect makeRect(void) const
	{
		return CRect(m_pt.x - m_nWid / 2, m_pt.y - m_nHt / 2, m_pt.x + m_nWid / 2, m_pt.y + m_nHt / 2); // (l, t), (r, b)?? ????? ????
	}
};
