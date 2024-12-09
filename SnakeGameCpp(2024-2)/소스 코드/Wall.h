#pragma once

//#include "LibConsole.hpp"
#include "Point2.h"

class Wall
{
public:
	// 생성자
	Wall(void)
	{
		setShape(' ');
		setColor(mglib::BLACK, mglib::WHITE);
	}
	// 소멸자
	~Wall()
	{}

	// getter
	char getShape(void) const
	{
		return m_shape;
	}
	int getTextColor(void) const
	{
		return m_textCol;
	}
	int getBackColor(void) const
	{
		return m_backCol;
	}

	// setter
	void setShape(char shape)
	{
		m_shape = shape;
	}
	void setColor(int textCol, int backCol)
	{
		m_textCol = textCol;
		m_backCol = backCol;
	}

	void makeHWall(Point2 pt, int count) // 수평(horizontal) 벽
	{
		for (int i = 0; i < count; i++)
		{
			pt.printChar(m_shape, m_textCol, m_backCol);
			pt.moveRight();
		}
	}

	void makeVWall(Point2 pt, int count) // 수직(vertical) 벽
	{
		for (int i = 0; i < count; i++)
		{
			pt.printChar(m_shape, m_textCol, m_backCol);
			pt.moveDown();
		}
	}

protected: // 외부 접근 X; 상속 받아서 접근 O
	char m_shape;
	int m_textCol;
	int m_backCol;

	//private:
};