#include "pch.h"
#include "Fruit.h"
#include "resource.h"



bool Fruit::isHit(const CRect& rect)
{
	if (m_rect.PtInRect(rect.TopLeft()) || m_rect.PtInRect(rect.BottomRight()) ||
		m_rect.PtInRect(CPoint(rect.left, rect.bottom)) ||
		m_rect.PtInRect(CPoint(rect.right, rect.top))) return true;
	else if (rect.PtInRect(m_rect.TopLeft()) || rect.PtInRect(m_rect.BottomRight()) || 
		rect.PtInRect(CPoint(m_rect.left, m_rect.bottom))|| rect.PtInRect(CPoint(m_rect.right
		, m_rect.top))) return true;
		else return false;
}



void Fruit::draw(CDC* pDC)
{
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    // 마스크 비트맵 로드
    CBitmap maskBitmap;
    maskBitmap.LoadBitmap(IDB_BITMAP8); // 독사과 마스크 비트맵
    CBitmap* pOldMask = memDC.SelectObject(&maskBitmap);

    // 마스크 적용 (AND 연산)
    CPoint pt = m_rect.TopLeft();
    pDC->BitBlt(pt.x, pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCAND);

    // 독사과 비트맵 로드
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_BITMAP9); // 독사과 비트맵
    memDC.SelectObject(&bitmap);

    // 비트맵 적용 (OR 연산)
    pDC->BitBlt(pt.x, pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCPAINT);

    // 메모리 DC 정리
    memDC.SelectObject(pOldMask);
}



bool isHit(const CRect& rect)
{
	return false;
}

void draw(CDC* pDC)
{
}
