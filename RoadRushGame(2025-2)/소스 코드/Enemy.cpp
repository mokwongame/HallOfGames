#include "pch.h"
#include "Enemy.h"
#include "resource.h"
#include "BaseScreen.h"
#include "Road.h"

Enemy::Enemy(void)
{
	SetBitmap(100, 106, IDB_ENEMY_BACK, IDB_ENEMY_FORE);
	m_nStep = 5;
}

void Enemy::SetPtStart(const CRect& rect) 
{
    // rect: m_road.GetSafeRect()로 받은 도로 안쪽 영역
    const int laneCount = Road::LANE_COUNT;

    // 도로 안쪽 한 칸의 폭
    double laneWidth = static_cast<double>(rect.Width()) / laneCount;

    // 0 ~ 3 중에서 랜덤 레인 번호 선택
    int laneIndex = BaseScreen::RandRange(0, laneCount);  // 0,1,2,3 중 하나

    // 선택한 레인의 중앙 x 좌표
    int laneCenterX = static_cast<int>(rect.left + laneWidth * (laneIndex + 0.5));

    int y = -m_nBmpHt / 2;   // 위에서 내려오게
    m_ptCtr = CPoint(laneCenterX, y);
}