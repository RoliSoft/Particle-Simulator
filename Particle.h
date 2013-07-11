#pragma once
#include "stdafx.h"
#include "Vector.h"

class Particle
{
public:
	Vector Location;
	Vector Velocity;
	Vector Acceleration;
	double Size;
	double Mass;
	COLORREF Color;

	Particle(void);
	Particle(COLORREF color, double size, double mass, Vector location, Vector velocity, Vector acceleration);

	void Update();
	void Draw(Gdiplus::Graphics* g);
};