#pragma once
class Wall
{
public:
	Wall(void)
	{}
	~Wall()
	{}

	void setRect(const CRect& rect)
	{
		m_rect = rect;
		m_rect.right++;
		m_rect.bottom++;

		m_rect.InflateRect(m_nThick, m_nThick);
	}

	void draw(CDC * pDC);

	bool isHit(const CRect& rect) const;

	int getThick() const { return m_nThick; } // 벽 두께 반환

protected:
	CRect m_rect;
	int m_nThick = 10;
};

