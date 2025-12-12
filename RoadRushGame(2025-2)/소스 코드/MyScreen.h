#pragma once
#include "BaseScreen.h"
#include "Road.h"
#include "Player.h"
#include "Enemy.h"
#include "Manhole.h"
#include "Water.h"
#include "Drink.h"

class MyScreen :
	public BaseScreen
{
public:
	MyScreen(void);
	~MyScreen();

	void ResetGame();         // 새 게임 시작용
	double GetScore() const { return m_score; }  // 통계용
	int    GetHP()    const { return m_nHP; }    // 필요하면 사용

	static const int MAX_HP = 5;

	Road m_road;
	Player m_player;
	Enemy* m_pEnemy;
	Manhole* m_pManhole;
	Water* m_pWater;
	Drink* m_pDrink;

protected:
	double m_fps;	// frame per second
	int m_nDeltaTime;	// 단위: msec
	int m_nEnemyDistMargin;	// Enemy 시준으로 득점을 올린 거이 여유(distance margin)
	bool m_bGameOver;
	double m_score;
	int m_nHP; // 플레이어 체력

	void CreateEnemy(void);
	void MoveEnemy(void);
	void CheckEnemy(void);
	bool HitTest(void) const;

	void DrawScore(CDC* pDC) const;
	void DrawHpBar(CDC* pDC) const;	// 체력바

	// 맨홀
	void CreateManhole(void);
	void MoveManhole(void);
	void CheckManhole(void);

	// 물웅덩이
	void CreateWater(void);
	void MoveWater(void);
	void CheckWater(void);

	// 에너지드링크
	void CreateDrink(void);
	void MoveDrink(void);
	void CheckDrink(void);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

