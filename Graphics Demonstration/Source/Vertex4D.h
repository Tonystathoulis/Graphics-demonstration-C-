
#pragma once

class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z);
 
	Vertex(const Vertex& v);

	~Vertex();

	void Dehomogenise();
	void Normalise();
	float Length();

	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);
	float GetW() const;
	void SetW(const float w);

	Vertex& operator= (const Vertex& rhs);

	bool operator== (const Vertex& rhs) const;

	// SUBSTRACTION AND ADDITION OF VERTICES
	const Vertex operator+ (const Vertex& rhs) const;
	const Vertex operator- (const Vertex& rhs) const;

	// DOT AND CROSS PRODUCT
	float DotProduct(const Vertex& rhs) const;
	Vertex CrossProduct(const Vertex& rhs) const;

private:
	float xx;
	float yy;
	float zz;
	float ww;
};

