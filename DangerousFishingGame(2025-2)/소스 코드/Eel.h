#pragma once
#include "BaseObj.h"
class Eel :
    public BaseObj
{
public:
    Eel(void);
    virtual void SetPtStart(const CRect& rect);

    int m_dir; // +1 오른쪽, -1 왼쪽
    virtual int GetRad(void) const { return 20; } // 충돌 반경 

protected:

};