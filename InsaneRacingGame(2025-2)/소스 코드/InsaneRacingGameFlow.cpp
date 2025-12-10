#include "pch.h"
#include "InsaneRacingGameFlow.h"

InsaneRacingGameFlow::InsaneRacingGameFlow(void)
{
	// 생성자 초기화

	m_diffLevel = DifficultyLevel::Normal;  // 기본 난이도 설정
	SetDifficulty(m_diffLevel);             // 난이도 설정 함수 호출

	m_state = GameState::Title;	// 초기 상태는 타이틀 화면

	m_bGameOver = false;	// 게임 종료 여부 플래그 (GameOver / Cleared 공통)
	m_score = 0.0;			// 점수 초기화

	m_fuelMax = 100.0;		// 최대 연료량 설정
	m_fuel = m_fuelMax;		// 현재 연료량 초기화

	m_nElapsedTimeMs = 0;			// 누적 시간 초기화
	m_nEnemySpawnElapsed = 0;		// 적 스폰 누적 시간 초기화
	m_nEnemySpawnIntervalMs = m_currentDiff.enemySpawnIntervalMs;	// 적 스폰 간격 설정

	m_nEnemyDistMargin = 50;	// Enemy 기준으로 득점을 올린 거리 여유(distance margin)
	m_bNeedSpawnEnemy = false;	// 적 스폰 필요 여부 플래그 초기화
}

void InsaneRacingGameFlow::SetDifficulty(DifficultyLevel level)
{
	m_diffLevel = level;

	switch (m_diffLevel)
	{
	case DifficultyLevel::Easy:                         // 쉬움 난이도
		m_currentDiff.timeLimitMs = 15 * 1000;          // 15초
		m_currentDiff.enemySpawnIntervalMs = 2000;      // 2.0초
		m_currentDiff.baseFuelDecreasePerSec = 2.0;     // 초당 기본 2.0 연료 감소
		m_currentDiff.speedFuelFactor = 0.1;            // 속도 1당 초당 0.1 연료 추가 감소
		m_currentDiff.baseScorePerSec = 1.0;            // 초당 기본 1.0 점수 증가
		m_currentDiff.speedScoreFactor = 0.2;           // 속도 1당 초당 0.2 점수 추가 증가
		break;

	case DifficultyLevel::Normal:						// 보통 난이도
		m_currentDiff.timeLimitMs = 30 * 1000;			// 30초
		m_currentDiff.enemySpawnIntervalMs = 1500;		// 1.5초
		m_currentDiff.baseFuelDecreasePerSec = 3.0;		// 초당 기본 3.0 연료 감소
		m_currentDiff.speedFuelFactor = 0.15;			// 속도 1당 초당 0.15 연료 추가 감소
		m_currentDiff.baseScorePerSec = 1.0;			// 초당 기본 1.0 점수 증가
		m_currentDiff.speedScoreFactor = 0.3;			// 속도 1당 초당 0.3 점수 추가 증가
		break;

	case DifficultyLevel::Hard:							// 어려움 난이도
		m_currentDiff.timeLimitMs = 60 * 1000;			// 60초
		m_currentDiff.enemySpawnIntervalMs = 1000;		// 1.0초
		m_currentDiff.baseFuelDecreasePerSec = 4.0;		// 초당 기본 4.0 연료 감소
		m_currentDiff.speedFuelFactor = 0.2;			// 속도 1당 초당 0.2 연료 추가 감소
		m_currentDiff.baseScorePerSec = 1.0;			// 초당 기본 1.0 점수 증가
		m_currentDiff.speedScoreFactor = 0.4;			// 속도 1당 초당 0.4 점수 추가 증가
		break;

	default:
		break;
	}

	m_nEnemySpawnIntervalMs = m_currentDiff.enemySpawnIntervalMs;   // 적 스폰 간격 설정
}

void InsaneRacingGameFlow::ResetForNewGame()
{
	// 게임 상태 초기화
	m_state = GameState::Ready;
	m_bGameOver = false;

	// 점수 / 연료 / 시간 / 스폰 타이머 초기화
	m_score = 0.0;
	m_fuel = m_fuelMax;

	m_nElapsedTimeMs = 0;
	m_nEnemySpawnElapsed = 0;

	// 난이도 설정 값에서 스폰 간격 가져오기
	m_nEnemySpawnIntervalMs = m_currentDiff.enemySpawnIntervalMs;

	m_bNeedSpawnEnemy = false; // 적 스폰 필요 여부 플래그 초기화
}

void InsaneRacingGameFlow::Update(UINT  deltaMs, int   roadSpeed, bool  isCollidingWithEnemy, bool  isInGasStation, bool  enemyPassedFromBottom, bool  enemyPassedFromTop, bool  enemyExists)
{
	// 게임이 종료 상태면 더 이상 진행하지 않음 (GameOver / Cleared 포함)
	if (m_bGameOver || m_state == GameState::GameOver || m_state == GameState::Cleared) { return; }

	// 시간 업데이트 및 타임 아웃 체크
	UpdateTime(deltaMs);

	// 시간 제한으로 Cleared가 되었다면, 추가 갱신 불필요
	if (m_bGameOver || m_state == GameState::GameOver || m_state == GameState::Cleared) { return; }

	UpdateEnemySpawn(deltaMs, enemyExists);             // 적 스폰 관리
	UpdateCollisionAndGameOver(isCollidingWithEnemy);   // 충돌 검사 및 게임 오버 처리

	if (m_bGameOver || m_state == GameState::GameOver) { return; }

	if (enemyPassedFromBottom)
	{
		// 추월 성공: 점수 보너스
		m_score += 100.0;
	}
	else if (enemyPassedFromTop)
	{
		// 추월 실패: 감점
		m_score -= 100.0;
		if (m_score < 0.0)
		{
			m_score = 0.0;
		}
	}

	UpdateFuel(deltaMs, roadSpeed, isInGasStation);	// 연료량 업데이트

	if (m_bGameOver || m_state == GameState::GameOver) { return; }

	UpdateScore(deltaMs, roadSpeed); // 점수 업데이트
}

void InsaneRacingGameFlow::SetState(GameState state)
{
	m_state = state;
	// m_bGameOver는 "게임이 끝났는가?"를 의미하는 플래그로 사용
	m_bGameOver = (state == GameState::GameOver || state == GameState::Cleared);
}

// ----------------------------------- [ getters ] ----------------------------------- //
GameState InsaneRacingGameFlow::GetState() const
{
	return m_state;
}

bool InsaneRacingGameFlow::IsGameOver() const
{
	return m_bGameOver || (m_state == GameState::GameOver) || (m_state == GameState::Cleared);
}

double InsaneRacingGameFlow::GetScore() const
{
	return m_score;
}

double InsaneRacingGameFlow::GetFuel() const
{
	return m_fuel;
}

double InsaneRacingGameFlow::GetFuelMax() const
{
	return m_fuelMax;
}

UINT InsaneRacingGameFlow::GetElapsedTimeMs() const
{
	return m_nElapsedTimeMs;
}

DifficultyLevel InsaneRacingGameFlow::GetDifficultyLevel() const
{
	return m_diffLevel;
}

const DifficultyConfig& InsaneRacingGameFlow::GetDifficultyConfig() const
{
	return m_currentDiff;
}

bool InsaneRacingGameFlow::ConsumeEnemySpawnRequest()
{
	if (!m_bNeedSpawnEnemy) { return false; }

	// spawn 요청을 소비했으므로 플래그 리셋
	m_bNeedSpawnEnemy = false;
	return true;
}

void InsaneRacingGameFlow::UpdateTime(UINT deltaMs)
{
	m_nElapsedTimeMs += deltaMs;

	// 시간 제한 초과 시 GameClear 처리
	if (m_nElapsedTimeMs >= m_currentDiff.timeLimitMs)
	{
		m_state = GameState::Cleared;
		m_bGameOver = true; // 게임 종료 (성공)
	}
}

// ----------------------------------- [ 멤버 함수 정의 ] ----------------------------------- //
void InsaneRacingGameFlow::UpdateEnemySpawn(UINT deltaMs, bool enemyExists)
{
	// Enemy가 이미 있으면, 스폰 타이머는 초기화하고 Spawn 요청 취소
	if (enemyExists)
	{
		m_nEnemySpawnElapsed = 0;
		m_bNeedSpawnEnemy = false;
		return;
	}

	// Enemy가 없을 때만 스폰 타이머를 누적
	m_nEnemySpawnElapsed += deltaMs;

	if (m_nEnemySpawnElapsed >= m_nEnemySpawnIntervalMs)
	{
		m_nEnemySpawnElapsed -= m_nEnemySpawnIntervalMs;
		m_bNeedSpawnEnemy = true;
	}
}

void InsaneRacingGameFlow::UpdateCollisionAndGameOver(bool isCollidingWithEnemy)
{
	if (isCollidingWithEnemy)
	{
		m_state = GameState::GameOver;
		m_bGameOver = true;
	}
}

void InsaneRacingGameFlow::UpdateFuel(UINT deltaMs, int roadSpeed, bool isInGasStation)
{
	double deltaSec = deltaMs / 1000.0;	// 밀리초 → 초 변환

	if (isInGasStation)
	{
		// 주유소 안에 있으면 연료 충전
		m_fuel += 20.0 * deltaSec;

		if (m_fuel > m_fuelMax)
		{
			m_fuel = m_fuelMax;
		}
	}
	else
	{
		// 연료 소모 계산
		double consume = m_currentDiff.baseFuelDecreasePerSec * deltaSec + m_currentDiff.speedFuelFactor * roadSpeed * deltaSec;

		m_fuel -= consume; // 연료 소모 적용

		if (m_fuel <= 0.0)
		{
			// 연료 소진 시 게임 오버
			m_fuel = 0.0;
			m_state = GameState::GameOver;
			m_bGameOver = true;
		}
	}
}

void InsaneRacingGameFlow::UpdateScore(UINT deltaMs, int roadSpeed)
{
	double deltaSec = deltaMs / 1000.0;	// 밀리초 → 초 변환

	// 점수 증가 계산
	double addScore = m_currentDiff.baseScorePerSec * deltaSec + m_currentDiff.speedScoreFactor * roadSpeed * deltaSec;

	m_score += addScore; // 점수 적용
}
