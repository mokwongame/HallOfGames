/*
* MyScreen은 "월드, 렌더링, 사용자 입력 처리"만 담당. 즉, 보이는 부분과 직접 상호작용하는 부분만 처리한다.
* InsaneRacingGameFlow는 "게임 진행 흐름 관리"만 담당. 즉, 게임 상태, 점수, 연료량, 시간 제한, 적 스폰 관리 등 게임의 내부 로직을 처리한다.
* InsaneRacingGameFlow 스크립트의 목적 : 레이어 분리를 통해 코드의 가독성과 유지보수성을 높이고, 각 클래스가 자신의 역할에 집중할 수 있도록 함
*/

#pragma once
#include "InsaneRacingTypes.h"

class InsaneRacingGameFlow
{
public:
	InsaneRacingGameFlow(void); // 생성자

	// 초기 세팅
	void SetDifficulty(DifficultyLevel level);	// 난이도 설정
	void ResetForNewGame();						// Title → Ready → Playing 전 초기화

	// 매 프레임 호출
    void Update
    (
		UINT  deltaMs,                  // 경과 시간 (밀리초)
		int   roadSpeed,                // 도로 속도
		bool  isCollidingWithEnemy,     // 적과 충돌 중인지 여부
		bool  isInGasStation,           // 주유소 안에 있는지 여부
		bool  enemyPassedFromBottom,    // 적이 플레이어 아래로 지나갔는지 여부
		bool  enemyPassedFromTop,       // 적이 플레이어 위로 지나갔는지 여부
		bool enemyExists				// 적이 현재 존재하는지 여부
    );

	// setters
	void SetState(GameState state); // 게임 상태 설정

	// 상태 조회 (HUD/UI 에서 사용), getters
	GameState GetState() const;     // 현재 게임 상태
	bool IsGameOver() const;        // 게임 오버 여부

	double GetScore() const;        // 현재 점수
	double GetFuel() const;         // 현재 연료량
	double GetFuelMax() const;      // 최대 연료량

	UINT GetElapsedTimeMs() const;  // 누적 시간 (밀리초 단위)

	DifficultyLevel GetDifficultyLevel() const;				// 난이도 레벨 조회
	const DifficultyConfig& GetDifficultyConfig() const;	// 난이도 설정 조회
	
	bool ConsumeEnemySpawnRequest(); // spawn 요청 → true 리턴, 내부 플래그 리셋

private:
	// 게임 상태 멤버 변수

	GameState m_state;					// 현재 게임 상태
	DifficultyLevel m_diffLevel;		// 현재 난이도 레벨
	DifficultyConfig m_currentDiff;		// 현재 난이도 설정

	bool m_bGameOver;		// 게임 오버 여부
	double m_score;			// 현재 점수
	double m_fuel;			// 현재 연료량
	double m_fuelMax;		// 최대 연료량

	UINT m_nElapsedTimeMs;			// 누적 시간 (밀리초 단위)
	UINT m_nEnemySpawnElapsed;		// 적 스폰 누적 시간 (밀리초 단위)
	UINT m_nEnemySpawnIntervalMs;	// 적 스폰 간격 (밀리초 단위)

	int m_nEnemyDistMargin;	// Enemy 기준으로 득점을 올린 거리 여유(distance margin)
	bool m_bNeedSpawnEnemy; // 적 스폰 필요 여부 플래그

	// Update 관련 세부 함수들
	void UpdateTime(UINT deltaMs);										// 시간 갱신 및 타임아웃 검사
	void UpdateEnemySpawn(UINT deltaMs, bool enemyExists);				// 적 스폰 관리
	void UpdateCollisionAndGameOver(bool isCollidingWithEnemy);			// 충돌 검사 및 게임 오버 처리
	void UpdateFuel(UINT deltaMs, int roadSpeed, bool isInGasStation);	// 연료량 업데이트
	void UpdateScore(UINT deltaMs, int roadSpeed);						// 점수 업데이트
};
