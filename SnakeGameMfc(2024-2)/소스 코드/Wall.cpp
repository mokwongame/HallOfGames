#include "pch.h"
#include "Wall.h"

void Wall::draw(CDC* pDC)
{
    // �� ���� ����
    CBrush brush(RGB(0, 255, 0)); // ��� �귯��
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // �׵θ� ���� (������ �� ����)
    CPen pen(PS_SOLID, 1, RGB(0, 255, 0)); // ��� �׵θ�
    CPen* pOldPen = pDC->SelectObject(&pen);

    // ���� �� �κ��� �׸���
    pDC->Rectangle(m_rect.left, m_rect.top, m_rect.right, m_rect.top + m_nThick);      // ��� ��
    pDC->Rectangle(m_rect.left, m_rect.bottom - m_nThick, m_rect.right, m_rect.bottom); // �ϴ� ��
    pDC->Rectangle(m_rect.left, m_rect.top, m_rect.left + m_nThick, m_rect.bottom);   // ���� ��
    pDC->Rectangle(m_rect.right - m_nThick, m_rect.top, m_rect.right, m_rect.bottom);  // ���� ��

    // ���� �귯�ÿ� �� ����
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
}

bool Wall::isHit(const CRect& rect) const
{
	if (rect.left <= m_rect.left+m_nThick || rect.right >= m_rect.right - m_nThick
		|| rect.top <= m_rect.top + m_nThick || rect.bottom >= m_rect.bottom - m_nThick) 
		return true;
	else return false;
}
