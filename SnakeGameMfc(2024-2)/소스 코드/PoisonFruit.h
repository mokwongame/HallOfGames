#pragma once

#include "Fruit.h"

class PoisonFruit : public Fruit
{
public:
    PoisonFruit() {}
    ~PoisonFruit() {}

    void setRect(const CRect& rect) { m_rect = rect; } // ��ġ ����
    int getWidth() const { return m_nWid; }           // ���� �� ��ȯ
    int getHeight() const { return m_nHt; }           // ���� ���� ��ȯ
    CRect getRect() const { return m_rect; }  // ������� ��ġ�� ��ȯ
    void draw(CDC* pDC) override
    {
        CDC memDC;
        memDC.CreateCompatibleDC(pDC);

        // ����ũ ��Ʈ�� �ε�
        CBitmap maskBitmap;
        maskBitmap.LoadBitmap(IDB_BITMAP12); // ����� ����ũ ��Ʈ��
        CBitmap* pOldMask = memDC.SelectObject(&maskBitmap);

        // ����ũ ���� (AND ����)
        CPoint pt = m_rect.TopLeft();
        pDC->BitBlt(pt.x, pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCAND);

        // ����� ��Ʈ�� �ε�
        CBitmap bitmap;
        bitmap.LoadBitmap(IDB_BITMAP13); // ����� ��Ʈ��
        memDC.SelectObject(&bitmap);

        // ��Ʈ�� ���� (OR ����)
        pDC->BitBlt(pt.x, pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCPAINT);

        // �޸� DC ����
        memDC.SelectObject(pOldMask);
    }
};
