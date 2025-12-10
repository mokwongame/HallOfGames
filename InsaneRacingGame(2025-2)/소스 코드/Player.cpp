#include "pch.h"
#include "Player.h"
#include "resource.h"

Player::Player(void)
{
	SetBitmap(80, 156, IDB_CAR_BACK, IDB_CAR_FORE);
	m_nStep = 10;
}

void Player::SetPtStart(const CRect& rect)
{
	m_ptCtr = CPoint(rect.Width() / 2, rect.bottom - m_nBmpHt / 2);
}
