#include "pch.h"
#include "Water.h"
#include "BaseScreen.h"
#include "resource.h"
#include "Road.h"

Water::Water(void)
{
	SetBitmap(80, 80, IDB_WATER_BACK, IDB_WATER_FORE);
	m_nStep = 0;  
}

void Water::SetPtStart(const CRect& roadRect)
{
    const int laneCount = Road::LANE_COUNT;;
    double laneWidth = static_cast<double>(roadRect.Width()) / laneCount;

    int laneIndex = BaseScreen::RandRange(0, laneCount);      // 0~3
    int laneCenter = static_cast<int>(roadRect.left + laneWidth * (laneIndex + 0.5));

    int y = -m_nBmpHt / 2;
    m_ptCtr = CPoint(laneCenter, y);
}