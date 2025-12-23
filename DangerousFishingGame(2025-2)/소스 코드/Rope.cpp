#include "pch.h"
#include "Rope.h"
#include "resource.h"

Rope::Rope(void)
{
	m_nCollider = ColliderType::CIRCLE;
	SetBitmap(250, 250, IDB_BOATROPE_BACK, IDB_BOATROPE);
	SetSpeed(10);
}

void Rope::SetPtStart(const CPoint& pt)
{
	m_ptCtr = pt; // 배 줄 시작점에서 시작
}

CPoint Rope::GetHookTip(void) const
{
	const int TIP_X = 70;
	const int TIP_Y = 220;

	// 화면에서 bullet의 topLeft
	CPoint topLeft(m_ptCtr.x - 125, m_ptCtr.y - 125);
	return topLeft + CPoint(TIP_X, TIP_Y);
}
