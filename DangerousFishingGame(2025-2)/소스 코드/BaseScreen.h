#pragma once
#include <afxwin.h>

#define DEF_BACK_COLOR	RGB(255, 0, 0)

class BaseScreen :
	public CStatic
{
public:
	BaseScreen(void);
	~BaseScreen();

	COLORREF GetBackColor(void) const { return m_nBackColor; }
	void SetBackColor(COLORREF col) { m_nBackColor = col; }
	void Create(int nId, CWnd* pParent);

	static void DrawBitmap(CDC* pDC, const CPoint& pt, int nWid, int nHt, int nBackId, int nForeId);
	static void DrawBitmap(CDC* pDC, const CPoint& pt, int nWid, int nHt, int nForeId);

	static int Round(double x) { return int(x + 0.5); }	// 반올림
	static void RandSeed(void);		// 난수 초기화
	static double RandUni(void);	// [0, 1] 범위에서 실수인 난수 발생
	static int RandRange(int rmin, int rmax);	// [rmin, rmax] 범위에서 정수인 난수 발생

protected:
	COLORREF m_nBackColor; // 배경색

	void DrawBack(CDC* pDC) const;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};

