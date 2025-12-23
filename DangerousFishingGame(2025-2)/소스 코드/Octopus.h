#pragma once
#include "BaseObj.h"
class Octopus :
    public BaseObj
{
public: 
    Octopus(void);
    virtual void SetPtStart(const CRect& rect);

    virtual int GetRad(void) const { return 20; }  // 문어 충돌 반경

    int m_dir;
protected:

};

