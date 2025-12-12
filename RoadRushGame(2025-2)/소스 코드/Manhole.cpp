#include "pch.h"
#include "Manhole.h"
#include "BaseScreen.h"
#include "resource.h"
#include "Road.h"

Manhole::Manhole(void)
{
    m_nCloseBackId = IDB_MANHOLE_CLOSE_BACK;
    m_nCloseForeId = IDB_MANHOLE_CLOSE_FORE;
    m_nOpenBackId = IDB_MANHOLE_OPEN_BACK;
    m_nOpenForeId = IDB_MANHOLE_OPEN_FORE;

    // 맨홀 크기 (원하는 픽셀 크기로 조정)
    SetBitmap(80, 80, m_nCloseBackId, m_nCloseForeId);

    m_nStep = 0;      // 스스로는 안 움직임 (도로 속도로만 아래로 밀림)
    m_bOpen = false;
}

void Manhole::SetPtStart(const CRect& roadRect)
{
    const int laneCount = Road::LANE_COUNT;
    double laneWidth = static_cast<double>(roadRect.Width()) / laneCount;
    
    int laneIndex = BaseScreen::RandRange(0, laneCount);  // 0~3
    int laneCenterX = static_cast<int>(roadRect.left + laneWidth * (laneIndex + 0.5));

    int y = -m_nBmpHt / 2;
    m_ptCtr = CPoint(laneCenterX, y);
}

void Manhole::SetOpen(bool open)
{
    m_bOpen = open;
    if (m_bOpen)
    {
        SetBitmap(m_nBmpWid, m_nBmpHt, m_nOpenBackId, m_nOpenForeId);
    }
    else
    {
        SetBitmap(m_nBmpWid, m_nBmpHt, m_nCloseBackId, m_nCloseForeId);
    }
}
