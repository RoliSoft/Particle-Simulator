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

	double Magnitude();
	double DistanceTo(Vector v);

	bool operator == (Vector v);
	bool operator != (Vector v);
	Vector operator + (Vector v);
	Vector operator - (Vector v);
	Vector operator - ();
	Vector operator * (Vector v);
	Vector operator + (double v);
	Vector operator - (double v);
	Vector operator * (double v);
	Vector operator / (double v);
	void operator += (Vector v);
	void operator -= (Vector v);
	void operator *= (Vector v);
	void operator /= (Vector v);
	void operator += (double v);
	void operator -= (double v);
	void operator *= (double v);
	void operator /= (double v);
};