#include <conio.h>
#include <Windows.h> // Sleep이 정의
#include "SnakeGame.h"
#include <chrono>
#include <iostream>

SnakeGame::SnakeGame(void)
{
    // 난수 초기화
    mglib::randseed();

    m_rectWall.setColor(mglib::DAKR_YELLOW, mglib::DARK_VOILET);
    m_rectWall.make(Point2(0, 0), Point2(m_nWallWid, m_nWallHt));


    // 각 과일을 별도의 객체로 생성
    m_fruitA.setShape('A');
    m_fruitA.setColor(mglib::BLACK, mglib::RED);  // 빨간색
    m_fruitA.setScore(10);  // 10점
    m_fruitA.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_fruitB.setShape('B');
    m_fruitB.setColor(mglib::BLACK, mglib::YELLOW);  // 노란색
    m_fruitB.setScore(15);  // 15점
    m_fruitB.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_fruitC.setShape('C');
    m_fruitC.setColor(mglib::BLACK, mglib::WHITE);  // 흰색
    m_fruitC.setScore(20);  // 20점
    m_fruitC.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_fruitD.setShape('D');
    m_fruitD.setColor(mglib::BLACK, mglib::DARK_BLUE);  
    m_fruitD.setScore(25);  
    m_fruitD.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_fruitE.setShape('E');
    m_fruitE.setColor(mglib::BLACK, mglib::DARK_GREEN);  
    m_fruitE.setScore(30); 
    m_fruitE.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_fruitF.setShape('F');
    m_fruitF.setColor(mglib::BLACK, mglib::DARK_SKYBLUE); 
    m_fruitF.setScore(35);  
    m_fruitF.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_fruitG.setShape('G');
    m_fruitG.setColor(mglib::BLACK, mglib::DARK_RED);  
    m_fruitG.setScore(40);  
    m_fruitG.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_fruitH.setShape('H');
    m_fruitH.setColor(mglib::BLACK, mglib::SKYBLUE);  
    m_fruitH.setScore(45);  
    m_fruitH.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_fruitI.setShape('I');
    m_fruitI.setColor(mglib::BLACK, mglib::DAKR_YELLOW);  
    m_fruitI.setScore(50);  
    m_fruitI.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_snake.setShape('S');
    m_snake.setColor(mglib::RED, mglib::GREEN);
    m_snake.setDir(SnakeDir::DEF_NULL);
    m_snake.draw(Point2(m_nWallWid / 2, m_nWallHt / 2));

    m_startTime = std::chrono::steady_clock::now();
    m_lastObstacleTime = std::chrono::steady_clock::now();  // 초기화

    drawLogo();
    drawScore();
    mglib::hidecursor(); // 커서 숨기기
}

int SnakeGame::m_nScore = 0;  // static 변수 초기화
double SnakeGame::m_gameTime = 0;

// 아래 함수 정의를 헤더 파일로 옮길 때는 앞에 inline 추가
void SnakeGame::start(void)
{
    // 게임 시작 시간 기록
    m_startTime = std::chrono::steady_clock::now();

    // 과일 재생성 타이머
    //auto lastFruitTime = std::chrono::steady_clock::now();  // 마지막 과일 시간 기록

    while (1)
    {
        if (_kbhit()) // 키보드(kb: keyboard)가 눌러지면(if hit)
        {
            int key = mglib::getkey();
            switch (key)
            {
            case mglib::IK_LEFT:
                m_snake.setDir(SnakeDir::LEFT);
                m_snake.setShape('<');
                break;
            case mglib::IK_RIGHT:
                m_snake.setDir(SnakeDir::RIGHT);
                m_snake.setShape('>');
                break;
            case mglib::IK_UP:
                m_snake.setDir(SnakeDir::UP);
                m_snake.setShape('^');
                break;
            case mglib::IK_DOWN:
                m_snake.setDir(SnakeDir::DOWN);
                m_snake.setShape('v');
                break;
            }
        }

        m_snake.move();

        // 과일 충돌 확인
        if (m_fruitA.isHit(m_snake.getPt())) {
            m_nScore += m_fruitA.getScore();  // A 과일 점수 추가
            m_snake.eatFood();               // 뱀의 길이 증가
            increaseSnakeSpeed();            // 속도 증가
            drawScore();
            m_fruitA.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // 과일 재배치
        }
        if (m_fruitB.isHit(m_snake.getPt())) {
            m_nScore += m_fruitB.getScore();  // B 과일 점수 추가
            m_snake.eatFood();               // 뱀의 길이 증가
            increaseSnakeSpeed();            // 속도 증가
            drawScore();
            m_fruitB.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // 과일 재배치
        }
        if (m_fruitC.isHit(m_snake.getPt())) {
            m_nScore += m_fruitC.getScore();  // C 과일 점수 추가
            m_snake.eatFood();               // 뱀의 길이 증가
            increaseSnakeSpeed();            // 속도 증가
            drawScore();
            m_fruitC.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // 과일 재배치
        }
        if (m_fruitD.isHit(m_snake.getPt())) {
            m_nScore += m_fruitD.getScore(); 
            m_snake.eatFood();               // 뱀의 길이 증가
            increaseSnakeSpeed();            // 속도 증가
            drawScore();
            m_fruitD.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // 과일 재배치
        }
        if (m_fruitE.isHit(m_snake.getPt())) {
            m_nScore += m_fruitE.getScore();
            m_snake.eatFood();               // 뱀의 길이 증가
            increaseSnakeSpeed();            // 속도 증가
            drawScore();
            m_fruitE.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // 과일 재배치
        }
        if (m_fruitF.isHit(m_snake.getPt())) {
            m_nScore += m_fruitF.getScore();
            m_snake.eatFood();               // 뱀의 길이 증가
            increaseSnakeSpeed();            // 속도 증가
            drawScore();
            m_fruitF.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // 과일 재배치
        }
        if (m_fruitG.isHit(m_snake.getPt())) {
            m_nScore += m_fruitG.getScore();
            m_snake.eatFood();               // 뱀의 길이 증가
            increaseSnakeSpeed();            // 속도 증가
            drawScore();
            m_fruitG.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // 과일 재배치
        }
        if (m_fruitH.isHit(m_snake.getPt())) {
            m_nScore += m_fruitH.getScore();
            m_snake.eatFood();               // 뱀의 길이 증가
            increaseSnakeSpeed();            // 속도 증가
            drawScore();
            m_fruitH.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // 과일 재배치
        }
        if (m_fruitI.isHit(m_snake.getPt())) {
            m_nScore += m_fruitI.getScore();
            m_snake.eatFood();               // 뱀의 길이 증가
            increaseSnakeSpeed();            // 속도 증가
            drawScore();
            m_fruitI.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // 과일 재배치
        }

        // 벽과 충돌 체크
        if (m_rectWall.isHit(m_snake.getPt())) {
            std::cout << "Game Over!" << std::endl;
            break;
        }
        // 게임 시간 계산 (현재 시간 - 시작 시간)
        auto endTime = std::chrono::steady_clock::now();
        m_gameTime = std::chrono::duration<double>(endTime - m_startTime).count();  // 게임 시간 초 단위로 계산



        ::Sleep(m_snake.getSleep()); // 현재 실행을 잠시 정지(잠자기, sleep); 단위는 msec
    }
}

void SnakeGame::increaseSnakeSpeed()
{
    int currentSpeed = m_snake.getSleep();
    if (currentSpeed > 50) // 최소 속도 제한 (50ms 이상)
    {
        int speedIncrement = (m_nScore / 50) * 25; // 점수에 비례한 속도 증가폭 (기본 25ms씩 증가)
        m_snake.setSleep(currentSpeed - speedIncrement);
    }
}

void SnakeGame::drawLogo(void)
{
    Point2 pt1(m_nWallWid + 1, 0);
    pt1.printStr("[Snake Game]", mglib::YELLOW);
    Point2 pt2(m_nWallWid + 1, 1);
    pt2.printStr("RED: 10p", mglib::RED);
    Point2 pt3(m_nWallWid + 1, 2);
    pt3.printStr("YELLOW: 15p", mglib::YELLOW);
    Point2 pt4(m_nWallWid + 1, 3);
    pt4.printStr("WHITE: 20p", mglib::WHITE);
    Point2 pt5(m_nWallWid + 1, 4);
    pt5.printStr("DARK_BLUE: 25p", mglib::DARK_BLUE);
    Point2 pt6(m_nWallWid + 1, 5);
    pt6.printStr("DARK_GREEN: 30p", mglib::DARK_GREEN);
    Point2 pt7(m_nWallWid + 1, 6);
    pt7.printStr("DARK_SKYBLUE: 35p", mglib::DARK_SKYBLUE);
    Point2 pt8(m_nWallWid + 1, 7);
    pt8.printStr("DARK_RED: 40p", mglib::DARK_RED);
    Point2 pt9(m_nWallWid + 1, 8);
    pt9.printStr("SKYBLUE: 45p", mglib::SKYBLUE);
    Point2 pt10(m_nWallWid + 1, 9);
    pt10.printStr("DAKR_YELLOW: 50p", mglib::DAKR_YELLOW);

}

void SnakeGame::drawScore(void)
{
    Point2 pt(m_nWallWid + 1, 11);
    std::string str = "score = ";
    // 정수를 문자열로 변환: to_string() 함수 사용
    std::string sScore = std::to_string(m_nScore);
    pt.printStr(str + sScore, mglib::GREEN);
}