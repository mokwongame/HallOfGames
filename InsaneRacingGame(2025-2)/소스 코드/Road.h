#pragma once

class Road
{
public:
	Road(void);

	CRect GetSafeRect(void) const; // 경계석(curb)을 제외한 rect 정보를 반환
	int GetSpeed(void) const { return m_nLineStep; }
	void SetRect(const CRect& rect)
	{
		m_rect = rect;
	}
	void Draw(CDC* pDC) const;
	void MoveDown(void); // 도로를 아래로 움직이는 효과

	void IncSpeed(void)
	{
		m_nLineStep++;
	}
	void DecSpeed(void)
	{
		m_nLineStep--;
		if (m_nLineStep < 0) m_nLineStep = 0;
	}

	int Top(void) const
	{
		return m_rect.top;
	}
	int Bottom(void) const
	{
		return m_rect.bottom;
	}

protected:
	CRect m_rect;

	int m_nCurbSize;
	COLORREF m_nCurbHatchCol;
	COLORREF m_nCurbCol;

	int m_nLaneSize;
	int m_nLineWid;
	int m_nLineHt;
	int m_nLineOffset;
	int m_nLineStep;
	COLORREF m_nLineCol;

	void DrawCurbs(CDC* pDC) const;
	void DrawLines(CDC* pDC) const;
	void DrawOneLine(CDC* pDC, int x) const;
};

