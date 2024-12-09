#pragma once

#include "Point2.h"
#include <deque>

// C++ 언어: enum class 사용
enum class SnakeDir
{
    DEF_NULL = 0, LEFT, RIGHT, UP, DOWN
};

class Snake
{
public:
    // 생성자
    Snake(void) : m_nSleep(500), m_pt(0, 0) {} // 기본 속도 500ms로 설정
    Snake(int textCol, int backCol) : m_nSleep(500), m_pt(0, 0)
    {
        setShape('.');
        setColor(textCol, backCol);
        m_nDir = SnakeDir::DEF_NULL; // 방향이 없음(null)
    }
    ~Snake() {}

    // Getter
    char getShape(void) const { return m_shape; }
    int getTextColor(void) const { return m_textCol; }
    int getBackColor(void) const { return m_backCol; }
    SnakeDir getDir(void) const { return m_nDir; }
    Point2 getPt(void) const { return m_pt; } // 뱀의 머리 좌표 반환
    int getSleep(void) const { return m_nSleep; } // 뱀의 이동 속도 반환

    // Setter
    void setShape(char shape) { m_shape = shape; }
    void setColor(int textCol, int backCol)
    {
        m_textCol = textCol;
        m_backCol = backCol;
    }
    void setDir(SnakeDir nDir) { m_nDir = nDir; }
    void setSleep(int sleep) { m_nSleep = sleep; } // 속도 설정

    // 뱀의 머리를 그리는 함수
    void draw(const Point2& pt)
    {
        m_body.push_front(pt); // 뱀의 머리를 추가
        m_pt = pt;             // 머리 좌표를 업데이트
        m_pt.printChar(m_shape, m_textCol, m_backCol);
    }

    // 꼬리를 지우는 함수
    void eraseTail(void)
    {
        if (!m_body.empty())
        {
            Point2 tail = m_body.back();
            m_body.pop_back(); // 꼬리를 제거
            tail.printChar(' ', mglib::BLACK, mglib::BLACK);
        }
    }

    // 뱀의 이동 로직
    void move(void)
    {
        if (m_nDir == SnakeDir::DEF_NULL) return;

        // 머리의 다음 위치 계산
        Point2 nextHead = m_pt;
        switch (m_nDir)
        {
        case SnakeDir::LEFT:
            nextHead = nextHead - Point2(1, 0);
            break;
        case SnakeDir::RIGHT:
            nextHead = nextHead + Point2(1, 0);
            break;
        case SnakeDir::UP:
            nextHead = nextHead - Point2(0, 1);
            break;
        case SnakeDir::DOWN:
            nextHead = nextHead + Point2(0, 1);
            break;
        }

        // 머리 이동
        draw(nextHead);

        // 꼬리를 지움 (먹이를 먹은 경우 지우지 않음)
        if (!m_bAteFood)
        {
            eraseTail();
        }
        else
        {
            m_bAteFood = false; // 먹이를 먹은 상태 초기화
        }
    }

    // 먹이를 먹었을 때 호출되는 함수
    void eatFood(void)
    {
        m_bAteFood = true; // 먹이를 먹으면 꼬리가 길어지도록 설정
    }

private:
    char m_shape;              // 뱀의 모양
    int m_textCol;             // 텍스트 색상
    int m_backCol;             // 배경 색상
    Point2 m_pt;               // 뱀의 머리 좌표
    std::deque<Point2> m_body; // 뱀의 몸 (머리부터 꼬리까지 저장)
    SnakeDir m_nDir;           // 뱀의 이동 방향
    int m_nSleep;              // 뱀의 이동 속도(ms 단위)
    bool m_bAteFood = false;   // 먹이를 먹었는지 여부
};
