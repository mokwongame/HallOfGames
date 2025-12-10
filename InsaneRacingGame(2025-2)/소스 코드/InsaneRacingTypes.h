#pragma once
#include "pch.h"

enum class GameState
{
	Title,		// 타이틀 화면
	Ready,		// 게임 시작 전 대기 상태
	Playing,	// 게임 진행 중
	Paused,		// 게임 일시 정지
	GameOver,	// 게임 오버
	Cleared		// 게임 클리어
};

enum class DifficultyLevel
{
	Easy,		// 쉬움
	Normal,		// 보통
	Hard		// 어려움
};

// Enemy 차량 타입 (스프라이트/속도 등 바리에이션용)
enum class EnemyType
{
	Type1_SportCar,	// 스포츠카
	Type2_Bus,		// 버스
	Type3_Sedan		// 세단
};

struct DifficultyConfig
{
	UINT timeLimitMs;				// 시간 제한 (밀리초 단위)
	UINT enemySpawnIntervalMs;		// 적 스폰 간격 (밀리초 단위)
	double baseFuelDecreasePerSec;	// 기본 연료 감소량 (초당)

	// 속도 1당 추가 연료 소모량(초당) : 연비 시스템
	// 실제로는 고속 항속 주행시 연비가 좋아지지만, 게임의 난이도 조절을 위해 속도가 높을수록 연료 소모가 커지도록 설계
	double speedFuelFactor;
	double baseScorePerSec; 	// 기본 점수 증가량 (초당)
	double speedScoreFactor;	// 속도 1당 추가 점수 증가량 (초당)
};