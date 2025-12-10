#pragma once
#include <afxwin.h>

#define DEF_BACK_COLOR	RGB(255, 0, 0)

class BaseScreen :
	public CStatic
{
public:
	BaseScreen(void);
	~BaseScreen();

	void Create(int nId, CWnd* pParent);

	static void DrawBitmap(CDC* pDC, const CPoint& pt, int nWid, int nHt, int nBackId, int nForeId);

	static void RandSeed(void);
	static double RandUni(void); // [0, 1] 범위에서 실수인 난수 발생
	static int RandRange(int rmin, int rmax);

	COLORREF m_nBackColor; // 배경색

protected:
	void DrawBack(CDC* pDC) const;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};

