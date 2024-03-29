#pragma once
#include "stdafx.h"
#include <boost/circular_buffer.hpp>
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
	boost::circular_buffer<Vector> History;

	Particle(void);
	Particle(COLORREF color, double size, double mass, Vector location, Vector velocity, Vector acceleration);
	~Particle(void);

	void Update(bool history, bool vlimit);
	void Draw(CHwndRenderTarget* d, bool history);

private:
	CD2DSolidColorBrush *lineBrush, *fillBrush;
};