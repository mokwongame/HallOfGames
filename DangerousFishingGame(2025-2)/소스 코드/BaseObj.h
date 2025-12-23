#pragma once 


enum class ColliderType
{
	NULL_DEF = 0, RECT, CIRCLE
};

class BaseObj
{
public:
	BaseObj(void);

	int GetSpeed(void) const { return m_nStep; }
	CPoint GetPtCtr(void) const { return m_ptCtr; }
	void SetSpeed(int nStep) { m_nStep = nStep; }
	void SetBitmap(int nWid, int nHt, int nBackId, int nForeId);
	void SetPtCtr(const CPoint& pt) { m_ptCtr = pt; }
	virtual void SetPtStart(const CRect& rect)
	{
		m_ptCtr = CPoint(0, 0);
	}
	virtual void SetPtStart(const CPoint& pt)
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

	int Left(void) const
	{
		return m_ptCtr.x - m_nBmpWid / 2;
	}
	int Right(void) const
	{
		return m_ptCtr.x + m_nBmpWid / 2;
	}
	int Top(void) const
	{
		return m_ptCtr.y - m_nBmpHt / 2;
	}
	int Bottom(void) const
	{
		return m_ptCtr.y + m_nBmpHt / 2;
	}
	CPoint TopLeft(void) const
	{
		return CPoint(Left(), Top());
	}
	CPoint TopRight(void) const
	{
		return CPoint(Right(), Top());
	}
	CPoint BottomLeft(void) const
	{
		return CPoint(Left(), Bottom());
	}
	CPoint BottomRight(void) const
	{
		return CPoint(Right(), Bottom());
	}

	virtual int GetRad(void) const;
	CRect MakeRect(void) const;
	bool HitTest(const BaseObj& obj) const;

protected:
	ColliderType m_nCollider;
	CPoint m_ptCtr;
	int m_nBmpWid;
	int m_nBmpHt;
	int m_nBackId;
	int m_nForeId;
	int m_nStep;

	static bool HitTest(const CRect& rect1, const CRect& rect2); // rect <-> rect
	static bool HitTest(const CRect& rect1, const CPoint& pt2, int rad2); // rect <-> circle
	static bool HitTest(const CPoint& pt1, int rad1, const CPoint& pt2, int rad2); // circle <-> circle
	static bool HitTestSub(const CPoint& pt1, const CPoint& pt2, int rad);
	static bool HitTestSub(const CRect& rect1, const CRect& rect2);
};