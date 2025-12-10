#include "pch.h"
#include "Enemy.h"
#include "resource.h"
#include "BaseScreen.h"

Enemy::Enemy(void)
	: m_type(EnemyType::Type1_SportCar)
{
	// 기본 타입을 스포츠카로 설정
	SetType(m_type);
}

void Enemy::SetType(EnemyType type)
{
	m_type = type;

	switch (m_type)
	{
	case EnemyType::Type1_SportCar:
		// 빠른 스포츠카 (기존 Enemy 크기/속도 기준)
		SetBitmap(80, 148, IDB_ENEMY_BACK, IDB_ENEMY_FORE);
		m_nStep = 7;
		break;

	case EnemyType::Type2_Bus:
		// 버스: 조금 더 느리게
		SetBitmap(80, 177, IDB_ENEMY2_BACK, IDB_ENEMY2_FORE);
		m_nStep = 4;
		break;

	case EnemyType::Type3_Sedan:
	default:
		// 세단: 중간 정도
		SetBitmap(80, 146, IDB_ENEMY3_BACK, IDB_ENEMY3_FORE);
		m_nStep = 5;
		break;
	}
}

void Enemy::SetPtStart(const CRect& rect)
{
	// 도로 안전 영역 내에서 x 좌표 랜덤,
	// y는 화면 위쪽 바깥에서 등장
	int x = BaseScreen::RandRange(rect.left + m_nBmpWid / 2,
		rect.right - m_nBmpWid / 2);
	int y = -m_nBmpHt / 2;

	m_ptCtr = CPoint(x, y);
}
