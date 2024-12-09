#pragma once

//#include "LibConsole.hpp"
#include "Point2.h"

class Wall
{
public:
	// ������
	Wall(void)
	{
		setShape(' ');
		setColor(mglib::BLACK, mglib::WHITE);
	}
	// �Ҹ���
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

	void makeHWall(Point2 pt, int count) // ����(horizontal) ��
	{
		for (int i = 0; i < count; i++)
		{
			pt.printChar(m_shape, m_textCol, m_backCol);
			pt.moveRight();
		}
	}

	void makeVWall(Point2 pt, int count) // ����(vertical) ��
	{
		for (int i = 0; i < count; i++)
		{
			pt.printChar(m_shape, m_textCol, m_backCol);
			pt.moveDown();
		}
	}

protected: // �ܺ� ���� X; ��� �޾Ƽ� ���� O
	char m_shape;
	int m_textCol;
	int m_backCol;

	//private:
};