//Vector3.h

#pragma once

#include <GL/glew.h>

struct Vector3
{
	Vector3(void)
	{
		x = y = z = 0.f;
	}
	Vector3(GLfloat x, GLfloat y, GLfloat z)
	{
		this->x = x, this->y = y, this->z = z;
	}

	GLfloat x, y, z;
};

struct Point2
{
	Point2(void)
	{
		x = y = 0;
	}
	Point2(int x, int y)
	{
		this->x = x, this->y = y;
	}

	int x, y;
};

struct Point3
{
	Point3(void)
	{
		x = y = z = 0;
	}
	Point3(int x, int y, int z)
	{
		this->x = x, this->y = y, this->z = z;
	}

	int x, y, z;
};