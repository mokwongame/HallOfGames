#include "pch.h"
#include "Player.h"
#include "resource.h"
#include "Road.h"

Player::Player(void)
{
	SetBitmap(100, 100, IDB_CAR_BACK, IDB_CAR_FORE);
	m_nStep = 10;
}

void Player::SetPtStart(const CRect& rect)
{
	m_screenWidth = rect.Width();

	m_ptCtr = CPoint(rect.Width() / 2, rect.bottom - m_nBmpHt / 2);
}

void Player::MoveLeft()
{
	if (m_screenWidth <= 0) return;

	const int curbSize = 20; // Road에서 쓰는 경계석 두께랑 맞춰주기
	const int laneCount = Road::LANE_COUNT;

	int roadLeft = curbSize;
	int roadRight = m_screenWidth - curbSize;
	double roadWidth = static_cast<double>(roadRight - roadLeft);
	double laneWidth = roadWidth / laneCount;

	// 현재 내가 어느 레인에 가까운지 계산 (0 ~ 3)
	double rel = (m_ptCtr.x - roadLeft) / laneWidth;
	int lane = static_cast<int>(rel + 0.5);  // 가장 가까운 레인으로 반올림

	if (lane < 0) lane = 0;
	if (lane >= laneCount) lane = laneCount - 1;

	// 한 레인 왼쪽으로 이동
	if (lane > 0)
		lane--;

	// 그 레인의 "가운데 x좌표" 계산
	int laneCenter = static_cast<int>(roadLeft + laneWidth * (lane + 0.5));

	// x를 레인 중앙으로 딱 맞추기
	m_ptCtr.x = laneCenter;
}

void Player::MoveRight()
{
	if (m_screenWidth <= 0) return;

	const int curbSize = 20;
	const int laneCount = Road::LANE_COUNT;

	int roadLeft = curbSize;
	int roadRight = m_screenWidth - curbSize;
	double roadWidth = static_cast<double>(roadRight - roadLeft);
	double laneWidth = roadWidth / laneCount;

	double rel = (m_ptCtr.x - roadLeft) / laneWidth;
	int lane = static_cast<int>(rel + 0.5);

	if (lane < 0) lane = 0;
	if (lane >= laneCount) lane = laneCount - 1;

	// 한 레인 오른쪽으로 이동
	if (lane < laneCount - 1)
		lane++;

	int laneCenter = static_cast<int>(roadLeft + laneWidth * (lane + 0.5));
	m_ptCtr.x = laneCenter;
}