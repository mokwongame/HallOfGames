#include "pch.h"
#include "Wall.h"

void Wall::draw(CDC* pDC)
{
    // 벽 색상 설정
    CBrush brush(RGB(0, 255, 0)); // 녹색 브러시
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // 테두리 제거 (불투명 벽 생성)
    CPen pen(PS_SOLID, 1, RGB(0, 255, 0)); // 녹색 테두리
    CPen* pOldPen = pDC->SelectObject(&pen);

    // 벽의 각 부분을 그리기
    pDC->Rectangle(m_rect.left, m_rect.top, m_rect.right, m_rect.top + m_nThick);      // 상단 벽
    pDC->Rectangle(m_rect.left, m_rect.bottom - m_nThick, m_rect.right, m_rect.bottom); // 하단 벽
    pDC->Rectangle(m_rect.left, m_rect.top, m_rect.left + m_nThick, m_rect.bottom);   // 좌측 벽
    pDC->Rectangle(m_rect.right - m_nThick, m_rect.top, m_rect.right, m_rect.bottom);  // 우측 벽

    // 원래 브러시와 펜 복원
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
