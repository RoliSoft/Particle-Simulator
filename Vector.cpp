#include "stdafx.h"
#include "Vector.h"

Vector::Vector(void)
{
	this->X = 0;
	this->Y = 0;
	this->Z = 0;
}

Vector::Vector(double x, double y, double z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

bool Vector::operator == (Vector rhs)
{
	return this->X == rhs.X
		&& this->Y == rhs.Y
		&& this->Z == rhs.Z;
}

bool Vector::operator != (Vector rhs)
{
	return this->X != rhs.X
		|| this->Y != rhs.Y
		|| this->Z != rhs.Z;
}

Vector Vector::operator + (Vector rhs)
{
	return *new Vector(this->X + rhs.X,
					   this->Y + rhs.Y,
					   this->Z + rhs.Z);
}

Vector Vector::operator - (Vector rhs)
{
	return *new Vector(this->X - rhs.X,
					   this->Y - rhs.Y,
					   this->Z - rhs.Z);
}

Vector Vector::operator - ()
{
	return *new Vector(-this->X,
					   -this->Y,
					   -this->Z);
}

Vector Vector::operator * (double rhs)
{
	return *new Vector(this->X * rhs,
					   this->Y * rhs,
					   this->Z * rhs);
}

Vector Vector::operator / (double rhs)
{
	return *new Vector(this->X / rhs,
					   this->Y / rhs,
					   this->Z / rhs);
}

void Vector::operator += (Vector rhs)
{
	this->X += rhs.X;
	this->Y += rhs.Y;
	this->Z += rhs.Z;
}

void Vector::operator -= (Vector rhs)
{
	this->X -= rhs.X;
	this->Y -= rhs.Y;
	this->Z -= rhs.Z;
}

void Vector::operator *= (double rhs)
{
	this->X *= rhs;
	this->Y *= rhs;
	this->Z *= rhs;
}

void Vector::operator /= (double rhs)
{
	this->X /= rhs;
	this->Y /= rhs;
	this->Z /= rhs;
}