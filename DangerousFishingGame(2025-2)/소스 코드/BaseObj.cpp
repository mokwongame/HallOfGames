#include "pch.h"
#include "BaseObj.h"
#include "BaseScreen.h"

BaseObj::BaseObj(void)
{
	m_nCollider = ColliderType::NULL_DEF;
	m_nBmpWid = 0;
	m_nBmpHt = 0;
	m_nBackId = NULL;
	m_nForeId = NULL;
	m_nStep = 1;
}

void BaseObj::SetBitmap(int nWid, int nHt, int nBackId, int nForeId)
{
	m_nBmpWid = nWid;
	m_nBmpHt = nHt;
	m_nBackId = nBackId;
	m_nForeId = nForeId;
}

void BaseObj::Draw(CDC* pDC) const
{
	BaseScreen::DrawBitmap(pDC, TopLeft(), m_nBmpWid, m_nBmpHt, m_nBackId, m_nForeId);
}

int BaseObj::GetRad(void) const
{
	return __max(m_nBmpWid / 2, m_nBmpHt / 2);
}

CRect BaseObj::MakeRect(void) const
{
	// (l, t, r, b)
	return CRect(Left(), Top(), Right(), Bottom());
}

bool BaseObj::HitTest(const BaseObj& obj) const
{
	switch (m_nCollider)
	{
	case ColliderType::RECT:
		switch (obj.m_nCollider)
		{
		case ColliderType::RECT:
			return HitTest(MakeRect(), obj.MakeRect());
		case ColliderType::CIRCLE:
			return HitTest(MakeRect(), obj.GetPtCtr(), obj.GetRad());
		default: return false;
		}
		break;
	case ColliderType::CIRCLE:
		switch (obj.m_nCollider)
		{
		case ColliderType::RECT:
			return HitTest(obj.MakeRect(), GetPtCtr(), GetRad());
		case ColliderType::CIRCLE:
			return HitTest(GetPtCtr(), GetRad(), obj.GetPtCtr(), obj.GetRad());
		default: return false;
		}
		break;
	default: return false;
	}
}

bool BaseObj::HitTest(const CRect& rect1, const CRect& rect2)
{
	return HitTestSub(rect1, rect2) || HitTestSub(rect2, rect1);
}

bool BaseObj::HitTest(const CRect& rect1, const CPoint& pt2, int rad2)
{
	return HitTestSub(rect1.TopLeft(), pt2, rad2) || HitTestSub(rect1.BottomRight(), pt2, rad2) || HitTestSub(CPoint(rect1.right, rect1.top), pt2, rad2) || HitTestSub(CPoint(rect1.left, rect1.bottom), pt2, rad2);
}

bool BaseObj::HitTest(const CPoint& pt1, int rad1, const CPoint& pt2, int rad2)
{
	return HitTestSub(pt1, pt2, rad1 + rad2);
}

bool BaseObj::HitTestSub(const CPoint& pt1, const CPoint& pt2, int rad)
{
	CPoint ptDiff = pt2 - pt1;
	return ptDiff.x * ptDiff.x + ptDiff.y * ptDiff.y <= rad * rad; // x^2 + y^2 < r^2
}

bool BaseObj::HitTestSub(const CRect& rect1, const CRect& rect2)
{
	return rect1.PtInRect(rect2.TopLeft()) || rect1.PtInRect(rect2.BottomRight()) || rect1.PtInRect(CPoint(rect2.right, rect2.top)) || rect1.PtInRect(CPoint(rect2.left, rect2.bottom));
}