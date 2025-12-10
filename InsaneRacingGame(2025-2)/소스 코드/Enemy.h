#pragma once
#include "BaseCar.h"
#include "InsaneRacingTypes.h"	// EnemyType 정의

class Enemy :
	public BaseCar
{
public:
	Enemy(void);

	// Enemy 타입 설정 (스프라이트/속도 등)
	void SetType(EnemyType type);
	EnemyType GetType(void) const { return m_type; }

	// 화면 상단에서 시작 위치 설정 (랜덤 x, y는 화면 위 바깥)
	virtual void SetPtStart(const CRect& rect) override;

protected:
	EnemyType m_type;	// 현재 Enemy 차량의 타입
};


