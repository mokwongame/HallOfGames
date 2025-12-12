#pragma once
#include "BaseCar.h"

class Water : public BaseCar
{
public:
    Water(void);

    void SetPtStart(const CRect& roadRect);  // 4레인 중앙에만 생성
};
