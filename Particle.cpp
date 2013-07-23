#include "stdafx.h"
#include "Particle.h"

Particle::Particle(void)
{
	Color        = RGB(255, 0, 0);
	Location     = Vector(0, 0, 0);
	Velocity     = Vector(0, 0, 0);
	Acceleration = Vector(0, 0, 0);
	Size         = 0;
	Mass         = 0;
	History      = boost::circular_buffer<Vector>(50);
}

Particle::Particle(COLORREF color, double size, double mass, Vector location, Vector velocity, Vector acceleration)
{
	Color        = color;
	Location     = location;
	Velocity     = velocity;
	Acceleration = acceleration;
	Size         = size == 0 ? 20 * mass / 1000 : size;
	Mass         = mass;
	History      = boost::circular_buffer<Vector>(50);
}

Particle::~Particle(void)
{
	delete lineBrush, fillBrush;
}

void Particle::Update(bool history, bool vlimit)
{
	if (history && (History.empty() || abs(History.back().X - Location.X) > 3 || abs(History.back().Y - Location.Y) > 3))
	{
		History.push_back(Location);
	}

	if (!history && !History.empty())
	{
		History.clear();
	}

	if (vlimit)
	{
		Velocity = (Velocity + Acceleration) / (1 + (Velocity * Acceleration).Magnitude());
	}
	else
	{
		Velocity += Acceleration;
	}

	Location += Velocity;
}

void Particle::Draw(CHwndRenderTarget* d, bool history)
{
	using namespace D2D1;

	if (lineBrush == nullptr && fillBrush == nullptr)
	{
		lineBrush = new CD2DSolidColorBrush(d, Color);
		fillBrush = new CD2DSolidColorBrush(d, Color, 125);
	}

	if (history && !History.empty())
	{
		int size = History.size();
		for (int i = 1; i < size; i++)
		{
			auto from = Point2(History[i - 1].X, History[i - 1].Y);
			auto to   = Point2(History[i].X, History[i].Y);

			lineBrush->SetOpacity((float)i / (float)size);
			d->DrawLine(from, to, lineBrush, 2);
		}

		auto from = Point2(History[size - 1].X, History[size - 1].Y);
		auto to   = Point2(Location.X, Location.Y);

		lineBrush->SetOpacity(1);
		d->DrawLine(from, to, lineBrush, 2);
	}
	
	auto center  = Point2(Location.X, Location.Y);
	auto ellipse = Ellipse(center, Size, Size);

	d->DrawEllipse(&ellipse, lineBrush, 2);
	d->FillEllipse(&ellipse, fillBrush);
}