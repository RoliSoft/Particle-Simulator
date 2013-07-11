#pragma once
#include "stdafx.h"

class Vector
{
public:
	double X;
	double Y;
	double Z;

	Vector(void);
	Vector(double x, double y, double z);

	bool operator == (Vector rhs);
	bool operator != (Vector rhs);
	Vector operator + (Vector rhs);
	Vector operator - (Vector rhs);
	Vector operator - ();
	Vector operator * (double rhs);
	Vector operator / (double rhs);
	void operator += (Vector rhs);
	void operator -= (Vector rhs);
	void operator *= (double rhs);
	void operator /= (double rhs);
};