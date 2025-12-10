#include "pch.h"
#include "GasStation.h"
#include "resource.h"
#include "BaseScreen.h"

GasStation::GasStation(void)
{
	// 주유소 비트맵 설정
	SetBitmap(120, 158, IDB_GASSTATION_BACK, IDB_GASSTATION_FORE);
	m_nStep = 3; // 주유소의 이동 속도 (도로 스크롤 속도와의 상대 속도 계산에 사용)
}

void GasStation::SetPtStart(const CRect& rect)
{
	// 주유소는 항상 도로 우측 차선에 고정
	
	int margin = 10; // 도로 오른쪽 끝에서의 여유값
	int x = rect.right - (m_nBmpWid / 2) - margin;

	// y는 화면 위쪽 바깥에서 등장
	int y = -m_nBmpHt / 2;

	m_ptCtr = CPoint(x, y);
}

void GasStation::MoveWithRoad(int roadSpeed)
{
	// Enemy와 동일하게 roadSpeed - m_nStep 구조로 상대 속도를 적용
	int relativeSpeed = roadSpeed - m_nStep;
	Move(0, relativeSpeed);
}

bool GasStation::IsOffScreen(const CRect& worldRect) const
{
	// 화면 아래로 충분히 벗어나면 삭제 대상
	return (Bottom() > worldRect.bottom + m_nBmpHt);
}
