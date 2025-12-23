#include "pch.h"
#include <stdlib.h> // 난수 발생용 헤더 파일
#include <time.h> // 시간 함수용 헤더 파일
#include "BaseScreen.h"

BaseScreen::BaseScreen(void)
{
	RandSeed(); // 난수 발생용 seed 초기화

	m_nBackColor = DEF_BACK_COLOR;
}

BaseScreen::~BaseScreen()
{
}

void BaseScreen::Create(int nId, CWnd* pParent)
{
	CWnd* pWnd = pParent->GetDlgItem(nId);
	pWnd->ShowWindow(SW_HIDE);
	CRect rect;
	pWnd->GetWindowRect(rect);
	pParent->ScreenToClient(rect);

	CStatic::Create(NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rect, pParent);
}
BEGIN_MESSAGE_MAP(BaseScreen, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void BaseScreen::DrawBack(CDC* pDC) const
{
	CBrush brush;
	brush.CreateSolidBrush(m_nBackColor);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(rect, &brush);
}

void BaseScreen::DrawBitmap(CDC* pDC, const CPoint& pt, int nWid, int nHt, int nBackId, int nForeId)
{
	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitmap;

	// 배경을 투명으로 그리기
	bitmap.LoadBitmap(nBackId); // 메모리 할당
	CBitmap* pOldBitmap = memDc.SelectObject(&bitmap);
	pDC->BitBlt(pt.x, pt.y, nWid, nHt, &memDc, 0, 0, SRCAND);
	bitmap.DeleteObject(); // 메모리 해제
	bitmap.LoadBitmap(nForeId); // 메모리 할당
	memDc.SelectObject(&bitmap);
	pDC->BitBlt(pt.x, pt.y, nWid, nHt, &memDc, 0, 0, SRCPAINT);

	memDc.SelectObject(pOldBitmap);
}

void BaseScreen::DrawBitmap(CDC* pDC, const CPoint& pt, int nWid, int nHt, int nForeId)
{
	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitmap;
	bitmap.LoadBitmap(nForeId); // 메모리 할당
	CBitmap* pOldBitmap = memDc.SelectObject(&bitmap);
	pDC->BitBlt(pt.x, pt.y, nWid, nHt, &memDc, 0, 0, SRCCOPY);

	memDc.SelectObject(pOldBitmap);
}

void BaseScreen::RandSeed(void)
{
	time_t nTime = time(NULL); // _t 의미: typedef으로 정의한 자료형
	unsigned int seed = unsigned int(llabs(nTime * nTime));
	srand(seed);
}

double BaseScreen::RandUni(void)
{
	return rand() / double(RAND_MAX);
}

int BaseScreen::RandRange(int rmin, int rmax)
{
	double val = RandUni() * (rmax - rmin) + rmin;
	return int(val);
}

BOOL BaseScreen::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CBrush brush;
	//brush.CreateSolidBrush(m_nBackColor);
	//CRect rect;
	//GetClientRect(rect);
	//pDC->FillRect(rect, &brush);
	//return CStatic::OnEraseBkgnd(pDC);
	return FALSE; // 배경을 지우지 않음
}

void BaseScreen::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CStatic::OnPaint()을(를) 호출하지 마십시오.
}
