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

    // ����ũ ��Ʈ�� �ε�
    CBitmap maskBitmap;
    maskBitmap.LoadBitmap(IDB_BITMAP8); // ����� ����ũ ��Ʈ��
    CBitmap* pOldMask = memDC.SelectObject(&maskBitmap);

    // ����ũ ���� (AND ����)
    CPoint pt = m_rect.TopLeft();
    pDC->BitBlt(pt.x, pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCAND);

    // ����� ��Ʈ�� �ε�
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_BITMAP9); // ����� ��Ʈ��
    memDC.SelectObject(&bitmap);

    // ��Ʈ�� ���� (OR ����)
    pDC->BitBlt(pt.x, pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCPAINT);

    // �޸� DC ����
    memDC.SelectObject(pOldMask);
}



bool isHit(const CRect& rect)
{
	return false;
}

void draw(CDC* pDC)
{
}
