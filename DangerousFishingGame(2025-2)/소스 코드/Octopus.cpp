#include "pch.h"
#include "Octopus.h"

#include "pch.h"
#include "Octopus.h"
#include "resource.h"
#include "BaseScreen.h"

Octopus::Octopus(void)
{
    m_nCollider = ColliderType::CIRCLE;
    SetBitmap(120, 120, IDB_OCTO_BACK, IDB_OCTO);
  
    m_dir = 1;
}

void Octopus::SetPtStart(const CRect& rect)
{
    bool fromLeft = (BaseScreen::RandUni() < 0.5);

    m_ptCtr.y = BaseScreen::RandRange(rect.CenterPoint().y, rect.bottom - 80);

    int spd = BaseScreen::RandRange(3, 7);
    SetSpeed(spd);

    if (fromLeft)
    {
        m_ptCtr.x = -60;
        m_dir = +1;
    }
    else
    {
        m_ptCtr.x = rect.right + 60;
        m_dir = -1;
    }
}
