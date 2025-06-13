
#pragma once
#include <windows.h>
#include <vector>
#include "Light.h"
#include "Vertex4D.h"
#include "Matrix4D.h"

using namespace std;
//ACCESSORS TO TRIANGLES OF MODEL
class Triangle
{
public:
	Triangle();
	Triangle(int index0, int index1, int index2);
	~Triangle();
	Triangle(const Triangle& other);
	int GetIndex(int i) const;
	Vertex GetNormal() const;
	void SetNormal(Vertex & normal);
	Vertex GetCenter() const;
	void SetCenter(Vertex& center);
	bool IsVisible() const;
	void SetVisible(bool value);
	COLORREF GetColor() const;
	void SetColor(COLORREF color);
		
	Triangle& operator= (const Triangle &rhs);

private:
	int Indi[3];
	bool Visibility;
	Vertex Normal;
	Vertex inCenter;
	COLORREF Colour;
};

class Model
{
public:
	Model();
	~Model();
	
	void Clear();
	void ClearVertices();
	void ClearTriangles();

	// ACCESSORS
	vector<Triangle>& GetTriangles();
	vector<Vertex>& GetVertices();
	size_t GetTriangleCount() const;
	size_t GetVertexCount() const;
	void AddVertex(float x, float y, float z);
	void AddTriangle(int i0, int i1, int i2);

	void CopyModel(Model& rhs);

	void ApplyTransform(const Matrix &transform);
	void Dehomogenise();

	// CALCULATIONS
	void CalculateNormals(bool CCW);
	void CalculateCenters();
	void MarkBackfaces(float eX, float eY, float eZ, bool ortho);
	void ApplyDepthSorting();
	void CalcFlatShading(HDC hdc, Light light, float eX, float eY, float eZ, bool ortho);

	// ACCESORS FOR COLORS
	COLORREF GetColorKa() const;
	void SetColorKa(COLORREF color);
	COLORREF GetColorKd() const;
	void SetColorKd(COLORREF color);
	COLORREF GetColorKs() const;
	void SetColorKs(COLORREF color);
	float GetShininess() const;
	void SetShininess(float value);

	// DRAW MODEL IN DIFFERENT FASHIONS
	void DrawWireFrame(HDC hdc);
	void DrawSolidFill(HDC hdc);
	void DrawFlatShading(HDC hdc);

private:
	vector<Triangle> triangles;
	vector<Vertex> vertices;
	COLORREF Ka;
	COLORREF Kd;
	COLORREF Ks;
	float shine;

};
