
#include <windowsx.h>
#include <math.h>
#include <algorithm>  
#include "Model.h"

Triangle::Triangle()
{
	Indi[0] = 0;
	Indi[1] = 0;
	Indi[2] = 0;
	Visibility = true;
	Normal.SetX(0.0f);
	Normal.SetY(0.0f);
	Normal.SetZ(0.0f);
	Normal.SetW(1.0f);
	inCenter.SetX(0.0f);
	inCenter.SetY(0.0f);
	inCenter.SetZ(0.0f);
	inCenter.SetW(1.0f);
	Colour = RGB(128, 128, 128);
}

Triangle::Triangle(int index0, int index1, int index2)
{
	Indi[0] = index0;
	Indi[1] = index1;
	Indi[2] = index2;
	Visibility = true;
	Normal.SetX(0.0f);
	Normal.SetY(0.0f);
	Normal.SetZ(0.0f);
	Normal.SetW(1.0f);
	inCenter.SetX(0.0f);
	inCenter.SetY(0.0f);
	inCenter.SetZ(0.0f);
	inCenter.SetW(1.0f);
	Colour = RGB(128, 128, 128);
}

Triangle::Triangle(const Triangle& other)
{
	Indi[0] = other.GetIndex(0);
	Indi[1] = other.GetIndex(1);
	Indi[2] = other.GetIndex(2);
	Visibility = other.IsVisible();
	Normal = other.GetNormal();
	inCenter = other.GetCenter();
	Colour = other.GetColor();
}

Triangle::~Triangle()
{
}

int Triangle::GetIndex(int i) const
{
	return Indi[i];
}

bool Triangle::IsVisible() const
{
	return Visibility;
}

void Triangle::SetVisible(bool value)
{
	Visibility = value;
}


Vertex Triangle::GetNormal() const
{
	return Normal;
}

void Triangle::SetNormal(Vertex & normal)
{
	Normal = normal;
}

Vertex Triangle::GetCenter() const
{
	return inCenter;
}

void Triangle::SetCenter(Vertex& center)
{
	inCenter = center;
}

COLORREF Triangle::GetColor() const
{
	return Colour;
}

void Triangle::SetColor(COLORREF color)
{
	Colour = color;
}

Triangle& Triangle::operator=(const Triangle& rhs)
{
	if (this != &rhs)
	{
		Indi[0] = rhs.GetIndex(0);
		Indi[1] = rhs.GetIndex(1);
		Indi[2] = rhs.GetIndex(2);
		Visibility = rhs.IsVisible();
		Normal = rhs.GetNormal();
		inCenter = rhs.GetCenter();
	}
	return *this;
}

Model::Model()
{
	Ka = RGB(192, 192, 192);
	Kd = RGB(192, 192, 192);
	Ks = RGB(192, 192, 192);
	shine = 30.0f;
}

Model::~Model()
{
}

void Model::Clear()
{
	ClearTriangles();
	ClearVertices();
}

void Model::ClearVertices()
{
	vertices.clear();
}

void Model::ClearTriangles()
{
	triangles.clear();
}

vector<Triangle>& Model::GetTriangles()
{
	return triangles;
}

vector<Vertex>& Model::GetVertices()
{
	return vertices;
}

size_t Model::GetTriangleCount() const
{
	return triangles.size();
}

size_t Model::GetVertexCount() const
{
	return vertices.size();
}

void Model::AddVertex(float x, float y, float z)
{
	Vertex v;
	v = Vertex(x,y,z);
	vertices.push_back (v);
}

void Model::AddTriangle(int i0, int i1, int i2)
{
	Triangle t;
	t = Triangle(i0, i1, i2);
	triangles.push_back (t);
}

COLORREF Model::GetColorKa() const
{
	return Ka;
}

void Model::SetColorKa(COLORREF color)
{
	Ka = color;
}

COLORREF Model::GetColorKd() const
{
	return Kd;
}

void Model::SetColorKd(COLORREF color)
{
	Kd = color;
}

COLORREF Model::GetColorKs() const
{
	return Ks;
}

void Model::SetColorKs(COLORREF color)
{
	Ks = color;
}

float Model::GetShininess() const
{
	return shine;
}
void Model::SetShininess(float value)
{
	shine = value;
}


void Model::CopyModel(Model& rhs)
{
	vector<Triangle> triangles;
	vector<Vertex> vertices;
	
	
	triangles = rhs.GetTriangles();
	vertices = rhs.GetVertices();
	for ( int i = 0; i < rhs.GetTriangleCount(); ++i )
	{
		// ADD NEW POLYGON TO TRIANGLES
		AddTriangle(triangles[i].GetIndex(0), triangles[i].GetIndex(1), triangles[i].GetIndex(2));
	}

	// VERTEX ARRAY
	for( int i = 0; i < rhs.GetVertexCount(); ++i )
	{
		// ADD NEW VERTEX TO LIST OF VERTICES	
		AddVertex(vertices[i].GetX(),vertices[i].GetY(),vertices[i].GetZ());
	}

	Ka = rhs.GetColorKa();
	Kd = rhs.GetColorKd();
	Ks = rhs.GetColorKs();
	shine = rhs.GetShininess();
	
}


void Model::ApplyTransform(const Matrix &transform)
{
	Vertex vC;
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i] = transform * vertices[i];
	}
	for (size_t i = 0; i < triangles.size(); i++)
	{
		vC = transform * triangles[i].GetCenter();
		triangles[i].SetCenter(vC);
	}
}

void Model::Dehomogenise()
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].Dehomogenise();
	}
}

// MODEL MODES

//WIRE FRAME
void Model::DrawWireFrame(HDC hdc)
{
	int v0_inx, v1_inx, v2_inx;

	if (triangles.size() == 0)
	{
		return;
	}
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	HGDIOBJ oldPen = SelectObject(hdc, pen);

	for (size_t i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].IsVisible() == true)
		{
			v0_inx = triangles[i].GetIndex(0);
			v1_inx = triangles[i].GetIndex(1);
			v2_inx = triangles[i].GetIndex(2);

			MoveToEx(hdc, (int)vertices[v0_inx].GetX(), (int)vertices[v0_inx].GetY(), NULL);

			LineTo(hdc, (int)vertices[v1_inx].GetX(), (int)vertices[v1_inx].GetY());
			LineTo(hdc, (int)vertices[v2_inx].GetX(), (int)vertices[v2_inx].GetY());
			LineTo(hdc, (int)vertices[v0_inx].GetX(), (int)vertices[v0_inx].GetY());
		}

	}
	SelectObject(hdc, oldPen);
	DeleteObject(pen);

}
//SOLID FILL
void Model::DrawSolidFill(HDC hdc)
{
	int v0_inx, v1_inx, v2_inx;
	POINT points[3];
	POINT point;

	if (triangles.size() == 0)
	{
		return;
	}
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	HGDIOBJ oldPen = SelectPen(hdc, pen);

	HBRUSH brush = CreateSolidBrush(RGB(0, 255, 255));
	HBRUSH oldBrush = SelectBrush(hdc, brush);

	for (size_t i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].IsVisible() == true)
		{
			v0_inx = triangles[i].GetIndex(0);
			v1_inx = triangles[i].GetIndex(1);
			v2_inx = triangles[i].GetIndex(2);

			point.x = (int)vertices[v0_inx].GetX(); point.y = (int)vertices[v0_inx].GetY();
			points[0] = point;
			point.x = (int)vertices[v1_inx].GetX(); point.y = (int)vertices[v1_inx].GetY();
			points[1] = point;
			point.x = (int)vertices[v2_inx].GetX(); point.y = (int)vertices[v2_inx].GetY();
			points[2] = point;

			Polygon(hdc, points, 3);

			point.x = (int)triangles[i].GetCenter().GetX(); point.y = (int)triangles[i].GetCenter().GetY();

			MoveToEx(hdc, point.x, point.y, NULL);

			LineTo(hdc, point.x + 1, point.y);
			LineTo(hdc, point.x + 1, point.y + 1);
			LineTo(hdc, point.x, point.y + 1);
			LineTo(hdc, point.x, point.y);

		}

	}
	SelectBrush(hdc, oldBrush);
	DeleteObject(brush);

	SelectPen(hdc, oldPen);
	DeleteObject(pen);

}
// FLAT SHADING
void Model::CalcFlatShading(HDC hdc, Light light, float eX, float eY, float eZ, bool ortho)
{
	COLORREF faceColor;
	Vertex vN, vC, vL, vE, vH;
	float cD, cS;
	float totR, totG, totB;

	if (triangles.size() == 0)
	{
		return;
	}

	for (size_t i = 0; i < triangles.size(); i++)
	{
		vN = triangles[i].GetNormal();
		vN.Normalise();
		vC = triangles[i].GetCenter();

		if (light.IsDirectional()) vL = light.GetLPos();
		else vL = light.GetLPos() - vC;
		vL.Normalise();

		cD = vN.DotProduct(vL);

		if (ortho) vE = Vertex(eX, eY, eZ);
		else vE = Vertex(eX, eY, eZ) - vC;
		vE.Normalise();

		vH = vL + vE;
		vH.Normalise();

		cS = pow(vN.DotProduct(vH), GetShininess());

		if (cD <= 0) { cD = 0; cS = 0; }

		totR = (GetRValue(GetColorKa()) * GetRValue(light.GetColorLa()) / 255.0f +
			cD * GetRValue(GetColorKd()) * GetRValue(light.GetColorLd()) / 255.0f +
			cS * GetRValue(GetColorKs()) * GetRValue(light.GetColorLs()) / 255.0f);
		totG = (GetGValue(GetColorKa()) * GetGValue(light.GetColorLa()) / 255.0f +
			cD * GetGValue(GetColorKd()) * GetGValue(light.GetColorLd()) / 255.0f +
			cS * GetGValue(GetColorKs()) * GetGValue(light.GetColorLs()) / 255.0f);
		totB = (GetBValue(GetColorKa()) * GetBValue(light.GetColorLa()) / 255.0f +
			cD * GetBValue(GetColorKd()) * GetBValue(light.GetColorLd()) / 255.0f +
			cS * GetBValue(GetColorKs()) * GetBValue(light.GetColorLs()) / 255.0f);

		if (totR > 255.0f) totR = 255.0f;
		if (totG > 255.0f) totG = 255.0f;
		if (totB > 255.0f) totB = 255.0f;

		faceColor = RGB((int)totR, (int)totG, (int)totB);

		triangles[i].SetColor(faceColor);

	}

}

void Model::DrawFlatShading(HDC hdc)
{
	int v0inx, v1inx, v2inx;
	POINT points[3];
	POINT point;
	COLORREF faceColor;

	if (triangles.size() == 0)
	{
		return;
	}

	for (size_t i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].IsVisible() == true)
		{
			v0inx = triangles[i].GetIndex(0);
			v1inx = triangles[i].GetIndex(1);
			v2inx = triangles[i].GetIndex(2);

			point.x = (int)vertices[v0inx].GetX(); point.y = (int)vertices[v0inx].GetY();
			points[0] = point;
			point.x = (int)vertices[v1inx].GetX(); point.y = (int)vertices[v1inx].GetY();
			points[1] = point;
			point.x = (int)vertices[v2inx].GetX(); point.y = (int)vertices[v2inx].GetY();
			points[2] = point;

			faceColor = triangles[i].GetColor();

			HPEN pen = CreatePen(PS_SOLID, 1, faceColor);
			HGDIOBJ oldPen = SelectPen(hdc, pen);

			HBRUSH brush = CreateSolidBrush(faceColor);
			HBRUSH oldBrush = SelectBrush(hdc, brush);

			Polygon(hdc, points, 3);

			SelectBrush(hdc, oldBrush);
			DeleteObject(brush);

			SelectPen(hdc, oldPen);
			DeleteObject(pen);

		}

	}


}

// FACE NORMALS CALCULATION
void Model::CalculateNormals(bool CCW)
{
	Vertex v0, v1, v2, vA, vB, vN;
	
	for (size_t i = 0; i < triangles.size(); i++)
	{
		v0 = vertices[triangles[i].GetIndex(0)];
		v1 = vertices[triangles[i].GetIndex(1)];
		v2 = vertices[triangles[i].GetIndex(2)];

		vA = v1 - v0;
		vB = v2 - v0;
				
		if (CCW)
			vN = vA.CrossProduct(vB);
		else
			vN = vB.CrossProduct(vA);

		vN.Normalise();
		triangles[i].SetNormal(vN);
	}
}

// FACE CENTER CALCULATION
void Model::CalculateCenters()
{
	Vertex v0, v1, v2, vC;

	for (size_t i = 0; i < triangles.size(); i++)
	{
		v0 = vertices[triangles[i].GetIndex(0)];
		v1 = vertices[triangles[i].GetIndex(1)];
		v2 = vertices[triangles[i].GetIndex(2)];

		vC.SetX((v0.GetX() + v1.GetX() + v2.GetX()) / 3.0f);
		vC.SetY((v0.GetY() + v1.GetY() + v2.GetY()) / 3.0f);
		vC.SetZ((v0.GetZ() + v1.GetZ() + v2.GetZ()) / 3.0f);
		vC.SetW(1.0f);

		triangles[i].SetCenter(vC);
	}
}

// BACKFACE INDICATION
void Model::MarkBackfaces(float eX, float eY, float eZ, bool ortho)
{	
	float result;
	Vertex v0, v1, v2, vN, vC, vE; 

	for (size_t i = 0; i < triangles.size(); i++)
	{
		vN = triangles[i].GetNormal();
		vC = triangles[i].GetCenter();
		
		if (ortho) vE = Vertex(eX, eY, eZ);
		else vE = Vertex(eX, eY, eZ) - vC;

		result = vN.DotProduct(vE);
		
		if (result <= 0)
			triangles[i].SetVisible(false);
		else
			triangles[i].SetVisible(true);

	}
}

// DEPTH SORTING
void Model::ApplyDepthSorting()
{
	
	std::sort(triangles.begin(), triangles.end(), [](const Triangle& lhs, const Triangle& rhs) {return lhs.GetCenter().GetZ() < rhs.GetCenter().GetZ(); });
	
}
