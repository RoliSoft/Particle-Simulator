#include "stdafx.h"
#include "Vector.h"

Vector::Vector(void)
{
	X = 0;
	Y = 0;
	Z = 0;
}

Vector::Vector(double x, double y, double z)
{
	X = x;
	Y = y;
	Z = z;
}

double Vector::Magnitude()
{
	return sqrt((X * X) + (Y * Y) + (Z * Z));
}

double Vector::DistanceTo(Vector v)
{
	return sqrt((X * v.X) + (Y * v.Y) + (Z * v.Z));
}

bool Vector::operator == (Vector v)
{
	return X == v.X
		&& Y == v.Y
		&& Z == v.Z;
}

bool Vector::operator != (Vector v)
{
	return X != v.X
		|| Y != v.Y
		|| Z != v.Z;
}

Vector Vector::operator + (Vector v)
{
	return Vector(X + v.X,
				  Y + v.Y,
				  Z + v.Z);
}

Vector Vector::operator - (Vector v)
{
	return Vector(X - v.X,
				  Y - v.Y,
				  Z - v.Z);
}

Vector Vector::operator - ()
{
	return Vector(-X,
				  -Y,
				  -Z);
}

Vector Vector::operator * (Vector v)
{
	return Vector(X * v.X,
				  Y * v.Y,
				  Z * v.Z);
}

Vector Vector::operator / (Vector v)
{
	return Vector(X / v.X,
				  Y / v.Y,
				  Z / v.Z);
}

Vector Vector::operator + (double v)
{
	return Vector(X + v,
				  Y + v,
				  Z + v);
}

Vector Vector::operator - (double v)
{
	return Vector(X - v,
				  Y - v,
				  Z - v);
}

Vector Vector::operator * (double v)
{
	return Vector(X * v,
				  Y * v,
				  Z * v);
}

Vector Vector::operator / (double v)
{
	return Vector(X / v,
				  Y / v,
				  Z / v);
}

void Vector::operator += (Vector v)
{
	X += v.X;
	Y += v.Y;
	Z += v.Z;
}

void Vector::operator -= (Vector v)
{
	X -= v.X;
	Y -= v.Y;
	Z -= v.Z;
}

void Vector::operator *= (Vector v)
{
	X *= v.X;
	Y *= v.Y;
	Z *= v.Z;
}

void Vector::operator /= (Vector v)
{
	X /= v.X;
	Y /= v.Y;
	Z /= v.Z;
}

void Vector::operator += (double v)
{
	X += v;
	Y += v;
	Z += v;
}

void Vector::operator -= (double v)
{
	X -= v;
	Y -= v;
	Z -= v;
}

void Vector::operator *= (double v)
{
	X *= v;
	Y *= v;
	Z *= v;
}

void Vector::operator /= (double v)
{
	X /= v;
	Y /= v;
	Z /= v;
}