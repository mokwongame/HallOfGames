#include "pch.h"
#include "Snake.h"
#include "resource.h"


#include "pch.h"
#include "Snake.h"
#include "resource.h"

// ����ũ�� �׸��ڸ� ����ϴ� Snake::draw �޼���
void Snake::draw(CDC* pDC)
{
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    // ����ũ ��Ʈ�� �ε�
    CBitmap maskBitmap;
    maskBitmap.LoadBitmap(IDB_FLY_MASK);
    CBitmap* pOldMask = memDC.SelectObject(&maskBitmap);

    // ����ũ ���� (AND ����)
    pDC->BitBlt(m_pt.x, m_pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCAND);

    // ������ũ ��Ʈ�� �ε�
    CBitmap snakeBitmap;
    snakeBitmap.LoadBitmap(IDB_BITMAP7);
    memDC.SelectObject(&snakeBitmap);

    // ��Ʈ�� ���� (OR ����)
    pDC->BitBlt(m_pt.x, m_pt.y, m_nWid, m_nHt, &memDC, 0, 0, SRCPAINT);

    // �޸� DC ����
    memDC.SelectObject(pOldMask);
}

// �׸��� ��Ʈ���� �׸��� �Լ�
void Snake::DrawBitmapWithShadow(CDC* pDC, CBitmap* pShadow, CPoint position, CSize size)
{
    CDC shadowDC;
    shadowDC.CreateCompatibleDC(pDC);

    CBitmap* pOldShadow = shadowDC.SelectObject(pShadow);

    // �׸��ڸ� ���� �׸� (�ణ ������)
    pDC->BitBlt(position.x + 5, position.y + 5, size.cx, size.cy, &shadowDC, 0, 0, SRCPAINT);

    shadowDC.SelectObject(pOldShadow);
}

// ����ũ�� ���� ��Ʈ���� �׸��� �Լ�
void Snake::DrawBitmapWithMask(CDC* pDC, CBitmap* pBitmap, CBitmap* pMask, CPoint position, CSize size)
{
    CDC memDC, maskDC;
    memDC.CreateCompatibleDC(pDC);
    maskDC.CreateCompatibleDC(pDC);

    // ��Ʈ�� ����
    CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);
    CBitmap* pOldMask = maskDC.SelectObject(pMask);

    // ����ũ�� ���� ����
    pDC->BitBlt(position.x, position.y, size.cx, size.cy, &maskDC, 0, 0, SRCAND);

    // ���� ��Ʈ���� ������
    pDC->BitBlt(position.x, position.y, size.cx, size.cy, &memDC, 0, 0, SRCPAINT);

    // ���� ��Ʈ�� ����
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
        return; // ������ �������� ���� ��� �̵����� ����
    }

    // ���� ��ġ�� ȭ�� ������ ������ �ʵ��� ����
    CRect clientRect;
    AfxGetMainWnd()->GetClientRect(&clientRect);

    if (m_pt.x < clientRect.left) m_pt.x = clientRect.left;
    if (m_pt.x > clientRect.right - m_nWid) m_pt.x = clientRect.right - m_nWid;
    if (m_pt.y < clientRect.top) m_pt.y = clientRect.top;
    if (m_pt.y > clientRect.bottom - m_nHt) m_pt.y = clientRect.bottom - m_nHt;
    m_rect = makeRect(); // �̵� �� Rect ����
}



CRect Snake::makeRect(void) const
{
	return CRect(m_pt.x - m_nWid/2, m_pt.y - m_nHt/2, m_pt.x + m_nWid 
				/ 2, m_pt.y + m_nHt / 2); // (l,t0, (r, b) ������ �Է�
}

