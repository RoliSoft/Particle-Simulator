#include "stdafx.h"
#include "Particle.h"

Particle::Particle(void)
{
	this->Color = RGB(255, 0, 0);
	this->Location = Vector(0, 0, 0);
	this->Velocity = Vector(0, 0, 0);
	this->Acceleration = Vector(0, 0, 0);
	this->Size = 0;
	this->Mass = 0;
	this->History = boost::circular_buffer<Vector>(30);
}

Particle::Particle(COLORREF color, double size, double mass, Vector location, Vector velocity, Vector acceleration)
{
	this->Color = color;
	this->Location = location;
	this->Velocity = velocity;
	this->Acceleration = acceleration;
	this->Size = size == 0 ? 20 * mass / 1000 : size;
	this->Mass = mass;
	this->History = boost::circular_buffer<Vector>(30);
}

void Particle::Update(bool history)
{
	if (history && (this->History.empty() || abs(this->History.back().X - this->Location.X) > 3 || abs(this->History.back().Y - this->Location.Y) > 3))
	{
		this->History.push_back(this->Location);
	}

	if (!history && !this->History.empty())
	{
		this->History.clear();
	}

	this->Velocity += this->Acceleration;
	this->Location += this->Velocity;
}

void Particle::Draw(CHwndRenderTarget* d, bool history)
{
	using namespace D2D1;

	if (this->histBrush == nullptr && this->lineBrush == nullptr && this->fillBrush == nullptr)
	{
		this->histBrush = new CD2DSolidColorBrush(d, RGB(125, 125, 125), 125);
		this->lineBrush = new CD2DSolidColorBrush(d, this->Color);
		this->fillBrush = new CD2DSolidColorBrush(d, this->Color, 125);
	}

	if (history && !this->History.empty())
	{
		int size = this->History.size();
		auto history = this->History.linearize();

		for (int i = 1; i < size; i++)
		{
			auto from = Point2(history[i - 1].X, history[i - 1].Y);
			auto to   = Point2(history[i].X, history[i].Y);

			d->DrawLine(from, to, this->histBrush);
		}

		auto from = Point2(history[size - 1].X, history[size - 1].Y);
		auto to   = Point2(this->Location.X, this->Location.Y);

		d->DrawLine(from, to, this->histBrush);
	}
	
	auto center  = Point2(this->Location.X, this->Location.Y);
	auto ellipse = Ellipse(center, this->Size, this->Size);

	d->DrawEllipse(&ellipse, this->lineBrush, 2);
	d->FillEllipse(&ellipse, this->fillBrush);
}