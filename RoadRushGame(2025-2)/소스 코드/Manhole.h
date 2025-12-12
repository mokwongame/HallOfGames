#pragma once
#include "BaseCar.h"

class Manhole : public BaseCar
{
public:
    Manhole(void);

    // 도로 안 임의 위치에 시작점 배치
    void SetPtStart(const CRect& roadRect);

    void SetOpen(bool open);
    bool IsOpen() const { return m_bOpen; }

private:
    bool m_bOpen;

    // 닫힘/열림 비트맵 ID 저장
    int m_nCloseBackId;
    int m_nCloseForeId;
    int m_nOpenBackId;
    int m_nOpenForeId;
};