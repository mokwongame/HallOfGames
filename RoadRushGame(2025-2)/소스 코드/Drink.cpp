#include "pch.h"
#include "Drink.h"
#include "BaseScreen.h"
#include "Road.h"
#include "resource.h"

Drink::Drink(void)
{ 
    SetBitmap(60, 60, IDB_DRINK_BACK, IDB_DRINK_FORE);

    m_nStep = 0; // 따로 속도 안 쓸 거면 0
}

void Drink::SetPtStart(const CRect& roadRect)
{
    const int laneCount = Road::LANE_COUNT; 
    double laneWidth = static_cast<double>(roadRect.Width()) / laneCount;

    int laneIndex = BaseScreen::RandRange(0, laneCount); // 0 ~ laneCount-1
    int laneCenterX = static_cast<int>(roadRect.left + laneWidth * (laneIndex + 0.5));

    
    int y = -m_nBmpHt / 2;

    m_ptCtr = CPoint(laneCenterX, y);
}