#pragma once

#include "Point2.h"
#include <deque>

// C++ ���: enum class ���
enum class SnakeDir
{
    DEF_NULL = 0, LEFT, RIGHT, UP, DOWN
};

class Snake
{
public:
    // ������
    Snake(void) : m_nSleep(500), m_pt(0, 0) {} // �⺻ �ӵ� 500ms�� ����
    Snake(int textCol, int backCol) : m_nSleep(500), m_pt(0, 0)
    {
        setShape('.');
        setColor(textCol, backCol);
        m_nDir = SnakeDir::DEF_NULL; // ������ ����(null)
    }
    ~Snake() {}

    // Getter
    char getShape(void) const { return m_shape; }
    int getTextColor(void) const { return m_textCol; }
    int getBackColor(void) const { return m_backCol; }
    SnakeDir getDir(void) const { return m_nDir; }
    Point2 getPt(void) const { return m_pt; } // ���� �Ӹ� ��ǥ ��ȯ
    int getSleep(void) const { return m_nSleep; } // ���� �̵� �ӵ� ��ȯ

    // Setter
    void setShape(char shape) { m_shape = shape; }
    void setColor(int textCol, int backCol)
    {
        m_textCol = textCol;
        m_backCol = backCol;
    }
    void setDir(SnakeDir nDir) { m_nDir = nDir; }
    void setSleep(int sleep) { m_nSleep = sleep; } // �ӵ� ����

    // ���� �Ӹ��� �׸��� �Լ�
    void draw(const Point2& pt)
    {
        m_body.push_front(pt); // ���� �Ӹ��� �߰�
        m_pt = pt;             // �Ӹ� ��ǥ�� ������Ʈ
        m_pt.printChar(m_shape, m_textCol, m_backCol);
    }

    // ������ ����� �Լ�
    void eraseTail(void)
    {
        if (!m_body.empty())
        {
            Point2 tail = m_body.back();
            m_body.pop_back(); // ������ ����
            tail.printChar(' ', mglib::BLACK, mglib::BLACK);
        }
    }

    // ���� �̵� ����
    void move(void)
    {
        if (m_nDir == SnakeDir::DEF_NULL) return;

        // �Ӹ��� ���� ��ġ ���
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

        // �Ӹ� �̵�
        draw(nextHead);

        // ������ ���� (���̸� ���� ��� ������ ����)
        if (!m_bAteFood)
        {
            eraseTail();
        }
        else
        {
            m_bAteFood = false; // ���̸� ���� ���� �ʱ�ȭ
        }
    }

    // ���̸� �Ծ��� �� ȣ��Ǵ� �Լ�
    void eatFood(void)
    {
        m_bAteFood = true; // ���̸� ������ ������ ��������� ����
    }

private:
    char m_shape;              // ���� ���
    int m_textCol;             // �ؽ�Ʈ ����
    int m_backCol;             // ��� ����
    Point2 m_pt;               // ���� �Ӹ� ��ǥ
    std::deque<Point2> m_body; // ���� �� (�Ӹ����� �������� ����)
    SnakeDir m_nDir;           // ���� �̵� ����
    int m_nSleep;              // ���� �̵� �ӵ�(ms ����)
    bool m_bAteFood = false;   // ���̸� �Ծ����� ����
};
