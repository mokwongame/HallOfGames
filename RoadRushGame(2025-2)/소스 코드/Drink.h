#pragma once
#include "BaseCar.h"

class Drink : public BaseCar
{
public:
    Drink(void);

    void SetPtStart(const CRect& roadRect);
};