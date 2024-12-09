#pragma once

#include "RectWall.h"
#include "Fruit.h"
#include "Snake.h"
#include <chrono>
#include <vector>
//#include "Obstacle.h"

class SnakeGame
{
public:
	SnakeGame(void);
	~SnakeGame()
	{
	}

	void start(void);

	static int m_nScore; // static 변수로 점수 저장
	static int getScore() { return m_nScore; }  // static 함수로 점수 반환

	static double getGameTime() { return m_gameTime; }

	void increaseSnakeSpeed();

protected:
	int m_nWallWid = 100;
	int m_nWallHt = 25;
	int m_nSleep = 800;
	RectWall m_rectWall;
	Snake m_snake;

	void drawLogo(void);
	void drawScore(void);
	void checkCollision(void);  // 충돌 검사 추가
	void addObstacle();         // 장애물 추가 함수


private:
	static double m_gameTime;  // 게임 시간 변수
	std::chrono::steady_clock::time_point m_startTime;  // 게임 시작 시간 기록

	//std::vector<Obstacle> m_obstacles;  // 장애물 리스트
	std::chrono::steady_clock::time_point m_lastObstacleTime;  // 마지막 장애물 생성 시간

	// 각 과일 객체를 독립적으로 선언
	Fruit m_fruitA;
	Fruit m_fruitB;
	Fruit m_fruitC;
	Fruit m_fruitD;
	Fruit m_fruitE;
	Fruit m_fruitF;
	Fruit m_fruitG;
	Fruit m_fruitH;
	Fruit m_fruitI;

};

//inline void SnakeGame::start(void)
//{
//}