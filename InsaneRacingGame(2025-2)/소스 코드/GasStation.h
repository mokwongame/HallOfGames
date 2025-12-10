#pragma once
#include "BaseCar.h"

// 주유소 클래스
class GasStation : public BaseCar
{
public:
	GasStation(void);

	// 도로 안전 영역 기준으로 시작 위치 설정 (우측 고정)
	void SetPtStart(const CRect& rect);

	// 도로 스크롤 속도 기준으로 아래로 이동
	void MoveWithRoad(int roadSpeed);

	// 화면 아래로 완전히 벗어났는지 여부
	bool IsOffScreen(const CRect& worldRect) const;
};
