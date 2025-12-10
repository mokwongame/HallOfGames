#pragma once

class BaseCar
{
public:
	BaseCar(void);

	int GetSpeed(void) const { return m_nStep; }
	void SetBitmap(int nWid, int nHt, int nBackId, int nForeId);
	void SetPtCtr(const CPoint& pt) { m_ptCtr = pt; }
	virtual void SetPtStart(const CRect& rect) // 가상 함수로 선언
	{
		m_ptCtr = CPoint(0, 0);
	}

	void Draw(CDC* pDC) const;

	void Move(int dx, int dy)
	{
		m_ptCtr += CPoint(dx, dy);
	}
	void MoveLeft(void)
	{
		Move(-m_nStep, 0);
	}
	void MoveRight(void)
	{
		Move(m_nStep, 0);
	}
	void MoveUp(void)
	{
		Move(0, -m_nStep);
	}
	void MoveDown(void)
	{
		Move(0, m_nStep);
	}

	int Top(void) const
	{
		return m_ptCtr.y - m_nBmpHt / 2;
	}
	int Bottom(void) const
	{
		return m_ptCtr.y + m_nBmpHt / 2;
	}

	CRect MakeRect(void) const;
	bool HitTest(const BaseCar& car) const;

protected:
	CPoint m_ptCtr;
	int m_nBmpWid;
	int m_nBmpHt;
	int m_nBackId;
	int m_nForeId;
	int m_nStep;
};

