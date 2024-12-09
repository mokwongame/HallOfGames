#pragma once

#include "Fruit.h"

class PoisonFruit : public Fruit
{
public:
    PoisonFruit() {}
    ~PoisonFruit() {}

    void setRect(const CRect& rect) { m_rect = rect; } // 위치 설정
    int getWidth() const { return m_nWid; }           // 과일 폭 반환
    int getHeight() const { return m_nHt; }           // 과일 높이 반환
    CRect getRect() const { return m_rect; }  // 독사과의 위치를 반환
    void draw(CDC* pDC) override
    {
        CDC memDC;
        memDC.CreateCompatibleDC(pDC);

        // 마스크 비트맵 로드
        CBitmap maskBitmap;
        maskBitmap.LoadBitmap(IDB_BITMAP12); // 독사과 마스크 비트맵
        CBitmap* pOldMask = memDC.SelectObject(&maskBitmap);

        // 마스크 적용 (AND 연산)
        CPoint pt = m_rect.TopLeft();
        pDC->BitBlt(pt.x, pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCAND);

        // 독사과 비트맵 로드
        CBitmap bitmap;
        bitmap.LoadBitmap(IDB_BITMAP13); // 독사과 비트맵
        memDC.SelectObject(&bitmap);

        // 비트맵 적용 (OR 연산)
        pDC->BitBlt(pt.x, pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCPAINT);

        // 메모리 DC 정리
        memDC.SelectObject(pOldMask);
    }
};
