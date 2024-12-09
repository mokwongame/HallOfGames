#pragma once

#include <iostream>
#include <string> // 확장자 h가 없음: C++ 표준 헤더 파일이라서
#include "LibConsole.hpp"

// 클래스는 객체(문제를 푸는 기본 구조)의 설계도
// 클래스 정의: class 클래스명 {...}
// 클래스명은 보통 파일명과 같음
// {...} 안에 멤버(member) 정의
// 멤버 등급(외부 접근): public(공공의: O), protected(보호된: X), private(사적인: X)
class Point2
{
	// public 그룹(외부에서 접근 가능): public 멤버의 집합체
public: // : 의미는 그룹을 뜻함
	// 클래스 안에 있는 함수 정의는 inline 함수 정의: inline을 생략
	// 클래스 밖에 있으면 inline을 써야 함

	// 생성자(constructor): 이 클래스의 인스턴스가 생성될 때 자동으로 호출되는 함수 -> 초기화; 클래스명과 같은 이름의 함수
	Point2(void) // 출력 자료형은 쓰면 안됨
	{
		setPt(0, 0); // 원점 초기화
	}
	Point2(int x, int y)
	{
		setPt(x, y);
	}
	// 복사 생성자(copy constructor): 클래스명(const 클래스명& 변수명)
	// 동일한 인스턴스(클래스명이 같음)의 레퍼런스(&: 읽기, 쓰기 O)를 상수(const: 쓰기 X) 형태로 받아서 현재 클래스를 초기화
	Point2(const Point2& pt)
	{
		//setPt(pt.m_x, pt.m_y);
		setPt(pt);
	}

	// 소멸자(destructor): 이 클래스의 인스턴스가 소멸될 때 자동으로 호출되는 함수 -> 뒷정리; 클래스명 앞에 ~(not 연산)을 붙인 함수
	~Point2() // 입력과 출력에 아무것도 안됨
	{
		//std::cout << "소멸되었음" << std::endl;
	}


	// 멤버 함수(메소드, method)
	// getter, setter: 프로퍼티 접근 함수
	int getX(void) const // const 원래 의미: 변수(variable)를 상수(constant number)로 취급; 메소드 뒤에 있는 const 의미: 프로퍼티를 상수로 취급
	{
		return m_x;
	}
	int getY(void) const
	{
		return m_y;
	}
	Point2 getPt(void) const
	{
		return Point2(m_x, m_y);
	}

	void setX(int x)
	{
		m_x = x;
	}
	void setY(int y)
	{
		m_y = y;
	}
	void setPt(int x, int y)
	{
		setX(x);
		setY(y);
	}
	void setPt(const Point2& pt)
	{
		setPt(pt.m_x, pt.m_y);
	}

	// 현재 위치를 (stepX, stepY)만큼 이동
	void move(int stepX, int stepY)
	{
		m_x += stepX; // m_x = m_x + stepX
		m_y += stepY;
	}
	void moveLeft(int step = 1)
	{
		move(-step, 0); // x 좌표가 줄어들면(-) 왼쪽으로 이동
	}
	void moveRight(int step = 1)
	{
		move(step, 0); // x 좌표가 늘어나면(+) 오른쪽으로 이동
	}
	void moveUp(int step = 1)
	{
		move(0, -step); // y 좌표가 줄어들면(-) 위로 이동
	}
	void moveDown(int step = 1)
	{
		move(0, step); // y 좌표가 늘어나면(+) 아래로 이동
	}

	void print(void) // protected, private 멤버는 클래스 안에서는 자유롭게 접근
	{
		// 네임스페이스의 멤버 접근 = 네임스페이스명::멤버
		std::cout << '(' << m_x << ", " << m_y << ')';
	}
	void printChar(char ch)
	{
		// 커서를 (m_x, m_y)로 이동
		mglib::gotoxy(m_x, m_y); // mglib는 namespace이고 LibConsole.hpp에 정의
		std::cout << ch;
	}
	// C++에서는 같은 이름의 함수가 허용됨: 대신 입력 변수가 달라야 함
	//  함수 입력에 있는 =의 의미: 입력하지 않으면 사용되는 디폴트 값
	void printChar(char ch, int textCol, int backCol = mglib::BLACK)
	{
		mglib::setbacktextcol(backCol, textCol);
		printChar(ch);
	}

	void printStr(const std::string& str, int textCol, int backCol = mglib::BLACK)
	{
		mglib::setbacktextcol(backCol, textCol);
		mglib::gotoxy(m_x, m_y);
		std::cout << str;
	}

	// 정적(static) 멤버(공통되는 멤버는 메모리를 고정해서 사용: 메모리 효율 향상) 연습용 코드
	static void printStr(const std::string& str) // 아래 코드는 멤버 변수(property)에 접근하지 않음 -> 정적 멤버로 선언하면 효율적임
	{
		using namespace std;
		//std::cout << "static test code" << std::endl;
		// string 클래스의 사용법 찾기: Microsoft Learn에서 string 클래스로 검색
		std::cout << str;
		// string 길이 얻기: length()
		//cout << str.length() << endl;
		//string str2 = "[" + str + "]"; // 문자열에 다른 문자열 붙이기
		//cout << str2 << endl;
		//cout << str2.length() << endl;
	}

	// 연산자 중복(operator overloading)
	// 자료형 operator 중복시킬_연산자_기호(입력 변수 선언): 마치 메소드처럼 정의
	Point2& operator=(const Point2& pt) // 할당 연산자의 중복: 출력 = 입력; x(왼쪽 값: &) = y(오른쪽 값: const)
	{
		m_x = pt.m_x;
		m_y = pt.m_y;
		// 레퍼런스 넘기는 방법: *pointer -> *this
		// 현재 인스턴스의 포인터는? this
		return *this; // w = x = y;
	}

	// 프렌드 함수: 메소드는 아니지만 멤버에 접근 가능한 함수
	// 사용 방법: 함수 선언 앞에 friend 붙임
	friend Point2 operator+(const Point2& pt1, const Point2& pt2);
	friend Point2 operator-(const Point2& pt1, const Point2& pt2);

	// protected 그룹(외부에서 접근 불가능)
protected:
	// 멤버 변수(프로퍼티, property) 선언 -> 정의도 가능
	int m_x = 0; // m_ 의미: 멤버(member)란 뜻
	int m_y = 0;
};

// ostream: output stream; cout
inline std::ostream& operator<<(std::ostream& stream, const Point2& pt)
{
	stream << "(" << pt.getX() << ", " << pt.getY() << ")";
	return stream;
}

// + 연산자 중복: c = a+b
// 아래 이항(항이 2개) 연산자는 인라인 함수라서 protected 멤버에 접근 불가능 -> 프렌드(friend) 함수 사용 가능
inline Point2 operator+(const Point2& pt1, const Point2& pt2)
{
	return Point2(pt1.m_x + pt2.m_x, pt1.m_y + pt2.m_y);
}

inline Point2 operator-(const Point2& pt1, const Point2& pt2)
{
	return Point2(pt1.m_x - pt2.m_x, pt1.m_y - pt2.m_y);
}

// 등호 연산자 중복
inline bool operator==(const Point2& pt1, const Point2& pt2)
{
	return (pt1.getX() == pt2.getX()) && (pt1.getY() == pt2.getY());
}