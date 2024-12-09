#include "pch.h"
#include "Snake.h"
#include "resource.h"


#include "pch.h"
#include "Snake.h"
#include "resource.h"

// 마스크와 그림자를 사용하는 Snake::draw 메서드
void Snake::draw(CDC* pDC)
{
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    // 마스크 비트맵 로드
    CBitmap maskBitmap;
    maskBitmap.LoadBitmap(IDB_FLY_MASK);
    CBitmap* pOldMask = memDC.SelectObject(&maskBitmap);

    // 마스크 적용 (AND 연산)
    pDC->BitBlt(m_pt.x, m_pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCAND);

    // 스네이크 비트맵 로드
    CBitmap snakeBitmap;
    snakeBitmap.LoadBitmap(IDB_BITMAP7);
    memDC.SelectObject(&snakeBitmap);

    // 비트맵 적용 (OR 연산)
    pDC->BitBlt(m_pt.x, m_pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCPAINT);

    // 메모리 DC 정리
    memDC.SelectObject(pOldMask);
}

// 그림자 비트맵을 그리는 함수
void Snake::DrawBitmapWithShadow(CDC* pDC, CBitmap* pShadow, CPoint position, CSize size)
{
    CDC shadowDC;
    shadowDC.CreateCompatibleDC(pDC);

    CBitmap* pOldShadow = shadowDC.SelectObject(pShadow);

    // 그림자를 먼저 그림 (약간 오프셋)
    pDC->BitBlt(position.x + 5, position.y + 5, size.cx, size.cy, &shadowDC, 0, 0, SRCPAINT);

    shadowDC.SelectObject(pOldShadow);
}

// 마스크와 원본 비트맵을 그리는 함수
void Snake::DrawBitmapWithMask(CDC* pDC, CBitmap* pBitmap, CBitmap* pMask, CPoint position, CSize size)
{
    CDC memDC, maskDC;
    memDC.CreateCompatibleDC(pDC);
    maskDC.CreateCompatibleDC(pDC);

    // 비트맵 선택
    CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);
    CBitmap* pOldMask = maskDC.SelectObject(pMask);

    // 마스크를 먼저 적용
    pDC->BitBlt(position.x, position.y, size.cx, size.cy, &maskDC, 0, 0, SRCAND);

    // 원본 비트맵을 렌더링
    pDC->BitBlt(position.x, position.y, size.cx, size.cy, &memDC, 0, 0, SRCPAINT);

    // 원래 비트맵 복원
    memDC.SelectObject(pOldBitmap);
    maskDC.SelectObject(pOldMask);
}

void Snake::move(void)
{
    switch (m_nDir)
    {
    case SnakeDir::LEFT:
        m_pt.x -= m_step;
        break;
    case SnakeDir::RIGHT:
        m_pt.x += m_step;
        break;
    case SnakeDir::UP:
        m_pt.y -= m_step;
        break;
    case SnakeDir::DOWN:
        m_pt.y += m_step;
        break;
    default:
        return; // 방향이 설정되지 않은 경우 이동하지 않음
    }

    // 뱀의 위치가 화면 밖으로 나가지 않도록 제한
    CRect clientRect;
    AfxGetMainWnd()->GetClientRect(&clientRect);

    if (m_pt.x < clientRect.left) m_pt.x = clientRect.left;
    if (m_pt.x > clientRect.right - m_nWid) m_pt.x = clientRect.right - m_nWid;
    if (m_pt.y < clientRect.top) m_pt.y = clientRect.top;
    if (m_pt.y > clientRect.bottom - m_nHt) m_pt.y = clientRect.bottom - m_nHt;
    m_rect = makeRect(); // 이동 후 Rect 갱신
}



CRect Snake::makeRect(void) const
{
	return CRect(m_pt.x - m_nWid/2, m_pt.y - m_nHt/2, m_pt.x + m_nWid 
				/ 2, m_pt.y + m_nHt / 2); // (l,t0, (r, b) 순서로 입력
}

