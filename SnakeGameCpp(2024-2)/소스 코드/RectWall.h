#pragma once

#include "Wall.h"

class RectWall
{
public:
	// ������: �ڵ� �ʱ�ȭ
	RectWall(void)
	{}
	RectWall(int textCol, int backCol)
	{
		setColor(textCol, backCol);
	}
	// �Ҹ���
	~RectWall()
	{}

	void setColor(int textCol, int backCol)
	{
		m_HWall.setShape('#');
		m_HWall.setColor(textCol, backCol);
		m_VWall.setShape('$');
		m_VWall.setColor(textCol, backCol);
	}

	// �簢 �� �����
	void make(const Point2& pt1, const Point2& pt2)
	{
		m_pt1 = pt1; // Point2�� �Ҵ� ������ ���� �ȵ�: ����Ʈ �Ҵ� ������(��� �������� �״�� ����)�� ������
		m_pt2 = pt2;
		int xLen = pt2.getX() - pt1.getX();
		int yLen = pt2.getY() - pt1.getY();
		m_HWall.makeHWall(pt1, xLen);
		m_HWall.makeHWall(Point2(pt1.getX(), pt1.getY() + yLen), xLen);
		m_VWall.makeVWall(pt1, yLen);
		m_VWall.makeVWall(Point2(pt1.getX() + xLen - 1, pt1.getY()), yLen);
	}

	bool isHit(const Point2& pt)
	{
		if (m_pt1.getX() == pt.getX()) return true;
		else if (m_pt2.getX() == pt.getX()) return true;
		else if (m_pt1.getY() == pt.getY()) return true;
		else if (m_pt2.getY() == pt.getY()) return true;
		else return false;
	}

protected:
	Wall m_HWall;	// ���� ��
	Wall m_VWall;	// ���� ��
	Point2 m_pt1, m_pt2;	// m_pt1: ������, m_pt2: ����
};