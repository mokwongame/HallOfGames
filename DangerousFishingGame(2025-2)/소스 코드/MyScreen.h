#pragma once

#include "BaseScreen.h"
#include "Boat.h"
#include "Rope.h"
#include "Fish.h"
#include "Octopus.h"
#include "Eel.h"

class MyScreen :
	public BaseScreen
{
public:
	MyScreen(void);
	~MyScreen();

	void ResetGame();
	int GetScore() const;


	void MoveHunterLeft(void);
	void MoveHunterRight(void);
	void MakeRope(void);


	void MyScreen::DrawScore(CDC* pDC) const;
protected:
	CPoint m_ptField;
	CRect m_rtClient;

	Boat m_Boat;
	Rope* m_pRope;
	Fish* m_pFish1;
	Fish* m_pFish2;
	Octopus* m_pOctopus; // 문어
	Eel* m_pEel; // 뱀장어

	bool m_gameOverSent = false;

	// 먹물 상태
	bool m_isInkOn;
	ULONGLONG m_inkEndTick;

	double m_fps;	// frame per second
	int m_nDeltaTime;	// 단위: msec

	void DrawMount(CDC* pDC) const;
	void DrawField(CDC* pDC) const;

	void MoveRope(void);
	void UnmakeRope(void);

	void MakeFish1(void);
	void MakeFish2(void);
	void MoveFish1(void);
	void MoveFish2(void);
	void UnmakeFish1(void);
	void UnmakeFish2(void);
	void CheckFish1(void);
	void CheckFish2(void);


	void MakeOctopus(void);
	void MoveOctopus(void);
	void UnmakeOctopus(void);

	void MakeEel(void);
	void MoveEel(void);
	void UnmakeEel(void);
	void CheckEel(void);

	void DrawHp(CDC* pDC) const;

	void CheckOctopus(void);
	void DrawInk(CDC* pDC) const;


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

