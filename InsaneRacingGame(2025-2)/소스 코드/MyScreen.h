#pragma once
#include "BaseScreen.h"
#include "Road.h"
#include "Player.h"
#include "Enemy.h"
#include "GasStation.h"

// 프로젝트 개발 과제에서 사용할 나의 헤더 파일
#include "UISprite.h"				// UI 스프라이트 클래스
#include "InsaneRacingTypes.h"		// 게임 상태 및 난이도/Enemy 타입 정의
#include "InsaneRacingGameFlow.h"	// 게임 진행 흐름 관리 클래스

class MyScreen :
	public BaseScreen
{
public:
	MyScreen(void);
	~MyScreen();

	// 월드 객체들
	Road       m_road;
	Player     m_player;
	Enemy* m_pEnemy;
	GasStation* m_pGasStation;	// 주유소

	// Title UI 스프라이트들
	UISprite m_uiTitleLogo;     // 타이틀 로고
	UISprite m_uiBtnStart;      // 게임 시작 버튼
	UISprite m_uiBtnDifficulty; // 난이도 선택 버튼
	UISprite m_uiBtnExit;       // 게임 종료 버튼

	// Difficulty 선택 화면용 UI 스프라이트들
	UISprite m_uiBtnEasy;       // Easy 난이도 버튼
	UISprite m_uiBtnNormal;     // Normal 난이도 버튼
	UISprite m_uiBtnHard;       // Hard 난이도 버튼
	UISprite m_uiBtnBack;       // Title 메인 메뉴로 돌아가는 버튼

	// 카운트다운용 UI 스프라이트들
	UISprite m_uiReady;         // "READY"
	UISprite m_uiCount3;        // "3"
	UISprite m_uiCount2;        // "2"
	UISprite m_uiCount1;        // "1"
	UISprite m_uiRun;           // "RUN!"

	// Pause UI 스프라이트들
	UISprite m_uiPausePanel;        // Pause 패널 배경
	UISprite m_uiBtnPauseResume;    // Resume 버튼
	UISprite m_uiBtnPauseExitToTitle; // Exit to Title 버튼

	// GameOver / Clear 결과 UI 스프라이트들
	UISprite m_uiResultPanel;          // 결과 패널 배경
	UISprite m_uiResultLabelGameOver;  // "Game Over" 라벨
	UISprite m_uiResultLabelCleared;   // "Game Clear" 라벨
	UISprite m_uiBtnResultRetry;       // Retry 버튼
	UISprite m_uiBtnResultExitToTitle; // Exit to Title 버튼

public:
	// 일시 정지/재개 전환 함수 (ESC에서 호출)
	void TogglePause();

	// 게임 시작 함수 (Title → Ready 카운트다운 시작)
	void StartGame();

	// Title로 복귀 (Pause / GameOver / Clear 등에서 사용)
	void ReturnToTitle();

protected:
	// Title 상태의 하위 모드
	enum class TitleMode
	{
		MainMenu,         // Start / Difficulty / Exit
		DifficultySelect  // Easy / Normal / Hard
	};

	// 카운트다운 단계
	enum class CountdownPhase
	{
		None,   // 카운트다운 없음
		Ready,  // READY
		Three,  // 3
		Two,    // 2
		One,    // 1
		Run,    // RUN!
		Done    // 카운트다운 종료
	};

	TitleMode      m_titleMode;          // 현재 Title UI 모드
	CountdownPhase m_countdownPhase;     // 현재 카운트다운 단계
	UINT           m_nCountdownElapsedMs;// 현재 단계에서 경과 시간(ms)

	static const UINT COUNTDOWN_STEP_MS = 800; // 단계당 0.8초

	double m_fps;        // frame per second
	int    m_nDeltaTime; // 단위: msec

	InsaneRacingGameFlow m_gameFlow; // 게임 진행 흐름 관리 객체

	int   m_nEnemyDistMargin;	// Enemy 기준 득점 거리 여유

	// 이동형 주유소 상태
	bool  m_bInGasStation;          // 플레이어가 주유소 차량과 겹치는지 여부
	UINT  m_nGasSpawnElapsedMs;     // 주유소 스폰 타이머 누적(ms)
	UINT  m_nGasSpawnIntervalMs;    // 주유소 스폰 간격(ms)

protected:
	// Enemy 타입 선택 헬퍼 (난이도 + 랜덤)
	EnemyType ChooseEnemyTypeByDifficulty() const;

	// 적 생성, 이동, 통과 검사 전용 함수
	void CreateEnemy(void); // 적 생성 함수
	void MoveEnemy(void);   // 적 이동 함수
	void CheckEnemy(bool& outPassedFromBottom,
		bool& outPassedFromTop); // 적 통과 검사 함수
	bool HitTest(void) const;                 // 플레이어와 적의 충돌 검사 함수

	// 주유소 관련 함수
	void UpdateGasStationSpawn(UINT deltaMs, int roadSpeed); // 스폰 + 이동 + 제거
	void UpdateGasStationFlag();                             // 플레이어와 주유소 충돌 여부 갱신
	void DrawGasStation(CDC* pDC) const;				     // 주유소 그리기

	// 초기화 관련 함수
	void InitWorld(const CRect& rect);       // 도로, 플레이어 등 월드 객체 초기화
	void InitTitleUI(const CRect& rect);     // 타이틀, 카운트다운, Pause, 결과 UI 스프라이트 초기화

	// Update 관련 함수
	void UpdateGame();                 // 게임 진행 상태에 따른 업데이트 함수
	void UpdateCountdown(UINT deltaMs);// Ready 상태일 때 카운트다운 진행

	// 그리기 관련 함수
	void DrawTitleScreen(CDC* pDC) const;     // 타이틀 화면 그리기
	void DrawInGameScreen(CDC* pDC) const;    // 게임 진행 화면 그리기
	void DrawHUD(CDC* pDC) const;             // HUD 그리기
	void DrawCountdown(CDC* pDC) const;       // 카운트다운 UI 그리기
	void DrawPauseOverlay(CDC* pDC) const;    // Pause 오버레이 UI 그리기
	void DrawResultOverlay(CDC* pDC) const;   // GameOver / Clear 결과 오버레이 UI 그리기

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
