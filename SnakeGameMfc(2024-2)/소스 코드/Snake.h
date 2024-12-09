#pragma once
#include <vector>
#include <afxwin.h> // MFC의 CRect 및 기타 유틸리티 사용

enum class SnakeDir
{
	NULL_DEF = 0, LEFT, RIGHT, UP, DOWN
};

class Snake
{
public:
	Snake(void)
	{}
	~Snake()
	{}
	CRect getRect(void)const
	{
		return m_rect;
	}
	void setPt(const CPoint& pt)
	{
		m_pt = pt;
		m_rect = makeRect();
		m_rect = CRect(pt.x - m_nWid / 2, pt.y - m_nHt / 2, pt.x + m_nWid / 2, pt.y + m_nHt / 2);
	}

	void setDir(SnakeDir nDir)
	{
		m_nDir = nDir;
	}
	void DrawBitmapWithShadow(CDC* pDC, CBitmap* pShadow, CPoint position, CSize size);

	void DrawBitmapWithMask(CDC* pDC, CBitmap* pBitmap, CBitmap* pMask, CPoint position, CSize size);

	void draw(CDC* pDC);

	void move(void);

	void increaseStep()
	{
		m_step += 1;
	}

	void setStep(int step) { m_step = step; } // m_step 설정 메서드 추가

protected:
	int m_nWid = 100;
	int m_nHt = 117;
	int m_step = 5; //snake이 움직일 간격
	CPoint m_pt; // 비트맵의 중심좌표
	CRect m_rect; // 충돌 테스트용
	SnakeDir m_nDir = SnakeDir::NULL_DEF;
	CRect makeRect(void) const;

private:
	std::vector<CRect> m_body; // Snake의 모든 파트를 저장 (머리 포함)
};

