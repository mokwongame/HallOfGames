#include "pch.h"
#include "Boat.h"
#include "resource.h"

Boat::Boat(void)
{
	SetBitmap(250, 250, IDB_BOAT_BACK, IDB_BOAT);
	SetSpeed(6);
}

void Boat::SetPtStart(const CRect& rect)
{
	m_ptCtr.x = rect.CenterPoint().x;
	m_ptCtr.y = rect.top + (150 / 2) + 10; // 낚시 연출을 위해 화면 위로 변경
}

CPoint Boat::GetLineAnchor(void) const
{
	// 배 비트맵이 250x250이라 반쪽이 125
	CPoint topLeft(m_ptCtr.x - 125, m_ptCtr.y - 125);

	// 그림판에서 찍은 줄 시작 좌표로 바꿔 넣기
	const int ANCHOR_X = 78;
	const int ANCHOR_Y = 150;

	return topLeft + CPoint(ANCHOR_X, ANCHOR_Y);
}
