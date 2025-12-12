#include "pch.h"
#include "Road.h"

Road::Road(void)
{
	m_nCurbSize = 20;
	m_nCurbHatchCol = RGB(255, 0, 0);
	m_nCurbCol = RGB(200, 200, 200);

	m_nLaneSize = 150;
	m_nLineWid = 10;
	m_nLineHt = 50;
	m_nLineCol = RGB(255, 255, 255);

	m_nLineOffset = 0;
	m_nLineStep = 5;
}

CRect Road::GetSafeRect(void) const
{
	return CRect(m_rect.left + m_nCurbSize, m_rect.top, m_rect.right - m_nCurbSize, m_rect.bottom);
}

void Road::Draw(CDC* pDC) const
{
	DrawCurbs(pDC);
	DrawLines(pDC);
}

void Road::MoveDown(void)
{
	m_nLineOffset += m_nLineStep;
	if (m_nLineOffset >= m_rect.Height())
	{
		//m_nLineOffset = 0; // 간단한 초기화
		// 정밀한 초기화
		int nLine = m_rect.Height() / (2 * m_nLineHt);
		m_nLineOffset -= nLine * (2 * m_nLineHt);
	}
}

void Road::DrawCurbs(CDC* pDC) const
{
	CBrush brush;
	brush.CreateHatchBrush(HS_DIAGCROSS, m_nCurbHatchCol);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	COLORREF oldBkCol = pDC->SetBkColor(m_nCurbCol);
	CPen pen;
	pen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->Rectangle(CRect(0, 0, m_nCurbSize, m_rect.Height()));
	pDC->Rectangle(CRect(m_rect.Width() - m_nCurbSize, 0, m_rect.Width(), m_rect.Height()));
	pDC->SelectObject(pOldBrush);
	pDC->SetBkColor(oldBkCol);
	pDC->SelectObject(pOldPen);
}

void Road::DrawLines(CDC* pDC) const
{
	const int laneCount = LANE_COUNT;

	// 1) 도로 안쪽 영역 계산 (양쪽 경계석 제외)
	int roadLeft = m_nCurbSize;
	int roadRight = m_rect.Width() - m_nCurbSize;
	int roadWidth = roadRight - roadLeft;

	// 2) 도로를 5칸으로 나누기
	int LANE_COUNT = 5;
	double laneWidth = static_cast<double>(roadWidth) / LANE_COUNT;

	// 3) 칸 사이 경계선 3개 그리기
	for (int i = 1; i < LANE_COUNT; ++i)
	{
		int x = static_cast<int>(roadLeft + laneWidth * i);
		// x는 선의 왼쪽, DrawOneLine 안에서 m_nLineWid 폭만큼 그려짐
		DrawOneLine(pDC, x - m_nLineWid / 2);
	}
}

void Road::DrawOneLine(CDC* pDC, int x) const
{
	CBrush brush;
	brush.CreateSolidBrush(m_nLineCol);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	// 밑으로 차선 그리기
	int y = m_nLineOffset;
	while (y < m_rect.Height())
	{
		pDC->Rectangle(CRect(x, y, x + m_nLineWid, y + m_nLineHt));
		y += m_nLineHt * 2;
	}
	// 위로 차선 그리기
	y = m_nLineOffset - m_nLineHt;
	while (y >= 0)
	{
		pDC->Rectangle(CRect(x, y - m_nLineHt, x + m_nLineWid, y));
		y -= m_nLineHt * 2;
	}

	pDC->SelectObject(pOldBrush);
}
