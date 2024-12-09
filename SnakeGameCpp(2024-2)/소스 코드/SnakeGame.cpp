#include <conio.h>
#include <Windows.h> // Sleep�� ����
#include "SnakeGame.h"
#include <chrono>
#include <iostream>

SnakeGame::SnakeGame(void)
{
    // ���� �ʱ�ȭ
    mglib::randseed();

    m_rectWall.setColor(mglib::DAKR_YELLOW, mglib::DARK_VOILET);
    m_rectWall.make(Point2(0, 0), Point2(m_nWallWid, m_nWallHt));


    // �� ������ ������ ��ü�� ����
    m_fruitA.setShape('A');
    m_fruitA.setColor(mglib::BLACK, mglib::RED);  // ������
    m_fruitA.setScore(10);  // 10��
    m_fruitA.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_fruitB.setShape('B');
    m_fruitB.setColor(mglib::BLACK, mglib::YELLOW);  // �����
    m_fruitB.setScore(15);  // 15��
    m_fruitB.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1));

    m_fruitC.setShape('C');
    m_fruitC.setColor(mglib::BLACK, mglib::WHITE);  // ���
    m_fruitC.setScore(20);  // 20��
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
    m_lastObstacleTime = std::chrono::steady_clock::now();  // �ʱ�ȭ

    drawLogo();
    drawScore();
    mglib::hidecursor(); // Ŀ�� �����
}

int SnakeGame::m_nScore = 0;  // static ���� �ʱ�ȭ
double SnakeGame::m_gameTime = 0;

// �Ʒ� �Լ� ���Ǹ� ��� ���Ϸ� �ű� ���� �տ� inline �߰�
void SnakeGame::start(void)
{
    // ���� ���� �ð� ���
    m_startTime = std::chrono::steady_clock::now();

    // ���� ����� Ÿ�̸�
    //auto lastFruitTime = std::chrono::steady_clock::now();  // ������ ���� �ð� ���

    while (1)
    {
        if (_kbhit()) // Ű����(kb: keyboard)�� ��������(if hit)
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

        // ���� �浹 Ȯ��
        if (m_fruitA.isHit(m_snake.getPt())) {
            m_nScore += m_fruitA.getScore();  // A ���� ���� �߰�
            m_snake.eatFood();               // ���� ���� ����
            increaseSnakeSpeed();            // �ӵ� ����
            drawScore();
            m_fruitA.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // ���� ���ġ
        }
        if (m_fruitB.isHit(m_snake.getPt())) {
            m_nScore += m_fruitB.getScore();  // B ���� ���� �߰�
            m_snake.eatFood();               // ���� ���� ����
            increaseSnakeSpeed();            // �ӵ� ����
            drawScore();
            m_fruitB.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // ���� ���ġ
        }
        if (m_fruitC.isHit(m_snake.getPt())) {
            m_nScore += m_fruitC.getScore();  // C ���� ���� �߰�
            m_snake.eatFood();               // ���� ���� ����
            increaseSnakeSpeed();            // �ӵ� ����
            drawScore();
            m_fruitC.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // ���� ���ġ
        }
        if (m_fruitD.isHit(m_snake.getPt())) {
            m_nScore += m_fruitD.getScore(); 
            m_snake.eatFood();               // ���� ���� ����
            increaseSnakeSpeed();            // �ӵ� ����
            drawScore();
            m_fruitD.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // ���� ���ġ
        }
        if (m_fruitE.isHit(m_snake.getPt())) {
            m_nScore += m_fruitE.getScore();
            m_snake.eatFood();               // ���� ���� ����
            increaseSnakeSpeed();            // �ӵ� ����
            drawScore();
            m_fruitE.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // ���� ���ġ
        }
        if (m_fruitF.isHit(m_snake.getPt())) {
            m_nScore += m_fruitF.getScore();
            m_snake.eatFood();               // ���� ���� ����
            increaseSnakeSpeed();            // �ӵ� ����
            drawScore();
            m_fruitF.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // ���� ���ġ
        }
        if (m_fruitG.isHit(m_snake.getPt())) {
            m_nScore += m_fruitG.getScore();
            m_snake.eatFood();               // ���� ���� ����
            increaseSnakeSpeed();            // �ӵ� ����
            drawScore();
            m_fruitG.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // ���� ���ġ
        }
        if (m_fruitH.isHit(m_snake.getPt())) {
            m_nScore += m_fruitH.getScore();
            m_snake.eatFood();               // ���� ���� ����
            increaseSnakeSpeed();            // �ӵ� ����
            drawScore();
            m_fruitH.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // ���� ���ġ
        }
        if (m_fruitI.isHit(m_snake.getPt())) {
            m_nScore += m_fruitI.getScore();
            m_snake.eatFood();               // ���� ���� ����
            increaseSnakeSpeed();            // �ӵ� ����
            drawScore();
            m_fruitI.draw(Point2(1, 1), Point2(m_nWallWid - 1, m_nWallHt - 1)); // ���� ���ġ
        }

        // ���� �浹 üũ
        if (m_rectWall.isHit(m_snake.getPt())) {
            std::cout << "Game Over!" << std::endl;
            break;
        }
        // ���� �ð� ��� (���� �ð� - ���� �ð�)
        auto endTime = std::chrono::steady_clock::now();
        m_gameTime = std::chrono::duration<double>(endTime - m_startTime).count();  // ���� �ð� �� ������ ���



        ::Sleep(m_snake.getSleep()); // ���� ������ ��� ����(���ڱ�, sleep); ������ msec
    }
}

void SnakeGame::increaseSnakeSpeed()
{
    int currentSpeed = m_snake.getSleep();
    if (currentSpeed > 50) // �ּ� �ӵ� ���� (50ms �̻�)
    {
        int speedIncrement = (m_nScore / 50) * 25; // ������ ����� �ӵ� ������ (�⺻ 25ms�� ����)
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
    // ������ ���ڿ��� ��ȯ: to_string() �Լ� ���
    std::string sScore = std::to_string(m_nScore);
    pt.printStr(str + sScore, mglib::GREEN);
}