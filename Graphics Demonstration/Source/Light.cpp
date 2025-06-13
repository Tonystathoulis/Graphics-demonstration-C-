
#include <windowsx.h>
#include <algorithm>  
#include "Light.h"

Light::Light()
{
	Ldirect = true;
	Lpos.SetX(0.0f);
	Lpos.SetY(0.0f);
	Lpos.SetZ(1.0f);
	Lpos.SetW(1.0f);
	La = RGB(32, 32, 32);
	Ld = RGB(128, 128, 128);
	Ls = RGB(255, 255, 255);
}

Light::Light(const Light& other)
{
	Ldirect = other.IsDirectional();
	Lpos = other.GetLPos();
	La = other.GetColorLa();
	Ld = other.GetColorLd();
	Ls = other.GetColorLs();
}

Light::~Light()
{
}


bool Light::IsDirectional() const
{
	return Ldirect;
}

void Light::SetDirectional(bool value)
{
	Ldirect = value;
}


Vertex Light::GetLPos() const
{
	return Lpos;
}

void Light::SetLPos(Vertex & pos)
{
	Lpos = pos;
}

COLORREF Light::GetColorLa() const
{
	return La;
}

void Light::SetColorLa(COLORREF color)
{
	La = color;
}

COLORREF Light::GetColorLd() const
{
	return Ld;
}

void Light::SetColorLd(COLORREF color)
{
	Ld = color;
}

COLORREF Light::GetColorLs() const
{
	return Ls;
}

void Light::SetColorLs(COLORREF color)
{
	Ls = color;
}



