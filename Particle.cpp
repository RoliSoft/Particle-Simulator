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
}

Particle::Particle(COLORREF color, double size, double mass, Vector location, Vector velocity, Vector acceleration)
{
	this->Color = color;
	this->Location = location;
	this->Velocity = velocity;
	this->Acceleration = acceleration;
	this->Size = size == 0 ? 20 * mass / 1000 : size;
	this->Mass = mass;
}

void Particle::Update()
{
	this->Velocity += this->Acceleration;
	this->Location += this->Velocity;
}

void Particle::Draw(Gdiplus::Graphics* g)
{
	using namespace Gdiplus;
	
	Pen pn(Gdiplus::Color((byte)255, GetRValue(this->Color), GetGValue(this->Color), GetBValue(this->Color)), 2);
	SolidBrush br(Gdiplus::Color((byte)125, GetRValue(this->Color), GetGValue(this->Color), GetBValue(this->Color)));

	g->DrawEllipse(&pn, (REAL)(this->Location.X - this->Size), (REAL)(this->Location.Y - this->Size), (REAL)(this->Size + this->Size), (REAL)(this->Size + this->Size));
	g->FillEllipse(&br, (REAL)(this->Location.X - this->Size), (REAL)(this->Location.Y - this->Size), (REAL)(this->Size + this->Size), (REAL)(this->Size + this->Size));
}

/*void Particle::Draw(CPaintDC* dc, CRect* rect)
{
	if (this->Location.X + this->Size >= rect->Width() || this->Location.Y + this->Size >= rect->Height()
	 || this->Location.X - this->Size <= 0 || this->Location.Y - this->Size <= 0)
	{
		return;
	}
	
	CBrush br(this->Color);
	CPen pn(PS_SOLID, 3, RGB((GetRValue(this->Color) + 255) / 2, (GetGValue(this->Color) + 255) / 2, (GetBValue(this->Color) + 255) / 2));

	dc->BeginPath();
	dc->Ellipse(this->Location.X - this->Size, this->Location.Y - this->Size, this->Location.X + this->Size, this->Location.Y + this->Size);
	dc->EndPath();
	dc->SelectObject(br);
	dc->FillPath();
	dc->SelectObject(pn);
	dc->Ellipse(this->Location.X - this->Size, this->Location.Y - this->Size, this->Location.X + this->Size, this->Location.Y + this->Size);
}*/