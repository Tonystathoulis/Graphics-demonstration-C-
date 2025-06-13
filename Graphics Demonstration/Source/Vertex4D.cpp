#include <math.h>
#include "Vertex4D.h"

Vertex::Vertex()
{
	xx = 0.0f;
	yy = 0.0f;
	zz = 0.0f;
	ww = 1.0f;
}

Vertex::Vertex(float x, float y, float z)
{
	xx = x;
	yy = y;
	zz = z;
	ww = 1.0f;
}

Vertex::Vertex(const Vertex& v)
{
	xx = v.GetX();
	yy = v.GetY();
	zz = v.GetZ();
	ww = v.GetW();
}

Vertex::~Vertex()
{
}

void Vertex::Dehomogenise()
{
	xx = xx/ww;
	yy = yy/ww;
	zz = zz/ww;
	ww = 1.0f;
}

float Vertex::Length()
{
	Dehomogenise();
	return sqrt(xx*xx+yy*yy+zz*zz);
}

void Vertex::Normalise()
{
	Dehomogenise();
	float len = Length();
	xx = xx/len;
	yy = yy/len;
	zz = zz/len;
	ww = 1.0f;
}


float Vertex::GetX() const
{
	return xx;
}

void Vertex::SetX(const float x)
{
	xx = x;
}

float Vertex::GetY() const
{
	return yy;
}

void Vertex::SetY(const float y)
{
	yy = y;
}

float Vertex::GetZ() const
{
	return zz;
}

void Vertex::SetZ(const float z)
{
	zz = z;
}

float Vertex::GetW() const
{
	return ww;
}

void Vertex::SetW(const float w)
{
	ww = w;
}

Vertex& Vertex::operator=(const Vertex& rhs)
{
	
	if (this != &rhs)
	{
		xx = rhs.GetX();
		yy = rhs.GetY();
		zz = rhs.GetZ();
		ww = rhs.GetW();
	}
	return *this;
}

bool Vertex::operator==(const Vertex& rhs) const
{
	// DEHOMOGENIZATION AND COMPARISON
	return ((xx == rhs.GetX()) && (yy == rhs.GetY()) && (zz == rhs.GetZ()));
}

const Vertex Vertex::operator+(const Vertex& rhs) const
{
	// DEHOMOGENIZATION AND ADDITION CALCULATION 
	return Vertex(xx + rhs.GetX(), yy + rhs.GetY(), zz + rhs.GetZ());
}

const Vertex Vertex::operator-(const Vertex& rhs) const
{
	// DEHOMOGENIZATION SUBSTRACTION CALCULATION
	return Vertex(xx - rhs.GetX(), yy - rhs.GetY(), zz - rhs.GetZ());
}

float Vertex::DotProduct(const Vertex & rhs) const
{
	// DEHOMOGENIZATION DOT PRODUTC CALCULATION
	return xx * rhs.GetX() + yy * rhs.GetY() +zz * rhs.GetZ();
}

Vertex Vertex::CrossProduct(const Vertex & rhs) const
{
	// DEHOMOGENIZATION AND CROSS PRODUTC CALCULATION
	return Vertex(yy*rhs.GetZ() - zz*rhs.GetY(), zz*rhs.GetX() - xx*rhs.GetZ(), xx*rhs.GetY() - yy*rhs.GetX());
}
