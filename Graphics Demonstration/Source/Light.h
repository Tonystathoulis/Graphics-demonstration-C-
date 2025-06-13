
#pragma once
#include <windows.h>
#include <vector>
#include "Vertex4D.h"

using namespace std;

//ACCESSORS FOR LIGHT
class Light
{
public:
	Light();
	~Light();
	Light(const Light& other);
	Vertex GetLPos() const;
	void SetLPos(Vertex& pos);
	bool IsDirectional() const;
	void SetDirectional(bool value);
	COLORREF GetColorLa() const;
	void SetColorLa(COLORREF color);
	COLORREF GetColorLd() const;
	void SetColorLd(COLORREF color);
	COLORREF GetColorLs() const;
	void SetColorLs(COLORREF color);
		
	Light& operator= (const Light &rhs);

private:
	COLORREF La;
	COLORREF Ld;
	COLORREF Ls;
	Vertex Lpos;
	bool Ldirect;
};


