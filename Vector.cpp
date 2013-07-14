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

bool Vector::operator == (Vector rhs)
{
	return X == rhs.X
		&& Y == rhs.Y
		&& Z == rhs.Z;
}

bool Vector::operator != (Vector rhs)
{
	return X != rhs.X
		|| Y != rhs.Y
		|| Z != rhs.Z;
}

Vector Vector::operator + (Vector rhs)
{
	return Vector(X + rhs.X,
				  Y + rhs.Y,
				  Z + rhs.Z);
}

Vector Vector::operator - (Vector rhs)
{
	return Vector(X - rhs.X,
				  Y - rhs.Y,
				  Z - rhs.Z);
}

Vector Vector::operator - ()
{
	return Vector(-X,
				  -Y,
				  -Z);
}

Vector Vector::operator * (double rhs)
{
	return Vector(X * rhs,
				  Y * rhs,
				  Z * rhs);
}

Vector Vector::operator / (double rhs)
{
	return Vector(X / rhs,
				  Y / rhs,
				  Z / rhs);
}

void Vector::operator += (Vector rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	Z += rhs.Z;
}

void Vector::operator -= (Vector rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	Z -= rhs.Z;
}

void Vector::operator *= (double rhs)
{
	X *= rhs;
	Y *= rhs;
	Z *= rhs;
}

void Vector::operator /= (double rhs)
{
	X /= rhs;
	Y /= rhs;
	Z /= rhs;
}