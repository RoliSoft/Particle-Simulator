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

void Particle::Draw(Gdiplus::Graphics* g, bool history)
{
	using namespace Gdiplus;

	if (history && !this->History.empty())
	{
		Pen pn2(Gdiplus::Color((byte)125, (byte)125, (byte)125, (byte)125));

		int size = this->History.size();
		auto history = this->History.linearize();

		for (int i = 1; i < size; i++)
		{
			g->DrawLine(&pn2, (REAL)history[i - 1].X, (REAL)history[i - 1].Y, (REAL)history[i].X, (REAL)history[i].Y);
		}

		g->DrawLine(&pn2, (REAL)history[size].X, (REAL)history[size].Y, (REAL)this->Location.X, (REAL)this->Location.Y);
	}

	Pen pn(Gdiplus::Color((byte)255, GetRValue(this->Color), GetGValue(this->Color), GetBValue(this->Color)), 2);
	SolidBrush br(Gdiplus::Color((byte)125, GetRValue(this->Color), GetGValue(this->Color), GetBValue(this->Color)));

	g->DrawEllipse(&pn, (REAL)(this->Location.X - this->Size), (REAL)(this->Location.Y - this->Size), (REAL)(this->Size + this->Size), (REAL)(this->Size + this->Size));
	g->FillEllipse(&br, (REAL)(this->Location.X - this->Size), (REAL)(this->Location.Y - this->Size), (REAL)(this->Size + this->Size), (REAL)(this->Size + this->Size));
}