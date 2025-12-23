#include "pch.h"
#include "Eel.h"
#include "resource.h"
#include "BaseScreen.h"

Eel::Eel(void)
{
    m_nCollider = ColliderType::CIRCLE;
    m_dir = +1;
}
void Eel::SetPtStart(const CRect& rect)
{
    bool fromLeft = (BaseScreen::RandUni() < 0.5);

    // 물고기랑 같은 높이대
    m_ptCtr.y = BaseScreen::RandRange(rect.CenterPoint().y, rect.bottom - 80);

    int spd = BaseScreen::RandRange(3, 7);
    SetSpeed(spd);

    if (fromLeft)
    {
        m_ptCtr.x = -60;
        m_dir = +1;

        // 오른쪽 보는 뱀장어
        SetBitmap(120, 120, IDB_EEL_BACK_R, IDB_EEL_R);
    }
    else
    {
        m_ptCtr.x = rect.right + 60;
        m_dir = -1;

        // 왼쪽 보는 뱀장어
        SetBitmap(120, 120, IDB_EEL_BACK_L, IDB_EEL_L);
    }
}
