#include "pch.h"
#include "Fish.h"
#include "resource.h"
#include "BaseScreen.h"

Fish::Fish(void)
{
	m_nCollider = ColliderType::CIRCLE;  // 2종 중 랜덤 선택
    m_kind = (BaseScreen::RandUni() < 0.5) ? 1 : 2; // 물고기 종류 랜덤
    m_dir = 1;  // 기본값
    
}

void Fish::SetPtStart(const CRect& rect)
{
    bool fromLeft = (BaseScreen::RandUni() < 0.5);


    // y는 아래쪽에서 랜덤
    m_ptCtr.y = BaseScreen::RandRange(rect.CenterPoint().y, rect.bottom - 80);

    // 속도 랜덤  (4 ~ 12)
    int spd = BaseScreen::RandRange(2, 7);
    SetSpeed(spd);

    if (fromLeft)
    {
        m_ptCtr.x = -60;     // 왼쪽 화면 밖
        m_dir = +1;          // 오른쪽으로 이동
    }
    else
    {
        m_ptCtr.x = rect.right + 60; // 오른쪽 화면 밖
        m_dir = -1;                  // 왼쪽으로 이동
    }

    // 방향에 맞는 비트맵 선택
    if (m_kind == 1)
    {
        if (m_dir > 0) SetBitmap(120, 120, IDB_FISH1_BACK_R, IDB_FISH1_R);
        else           SetBitmap(120, 120, IDB_FISH1_BACK_L, IDB_FISH1_L);
    }
    else
    {
        if (m_dir > 0) SetBitmap(120, 120, IDB_FISH2_BACK_R, IDB_FISH2_R);
        else           SetBitmap(120, 120, IDB_FISH2_BACK_L, IDB_FISH2_L);
    }
}
