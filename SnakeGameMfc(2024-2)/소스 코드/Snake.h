#pragma once
#include <vector>
#include <afxwin.h> // MFC�� CRect �� ��Ÿ ��ƿ��Ƽ ���

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

	void setStep(int step) { m_step = step; } // m_step ���� �޼��� �߰�

protected:
	int m_nWid = 100;
	int m_nHt = 117;
	int m_step = 5; //snake�� ������ ����
	CPoint m_pt; // ��Ʈ���� �߽���ǥ
	CRect m_rect; // �浹 �׽�Ʈ��
	SnakeDir m_nDir = SnakeDir::NULL_DEF;
	CRect makeRect(void) const;

private:
	std::vector<CRect> m_body; // Snake�� ��� ��Ʈ�� ���� (�Ӹ� ����)
};

