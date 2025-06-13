#pragma once
#include "Framework.h"
#include "Light.h"
#include "Model.h"
#include "Matrix4D.h"

using namespace std;


class Rasteriser : public Framework
{
	private:
	vector<Model> scenemodels; 
	vector<Model> transformedmodels; 
	RECT	 viewrect; 
	Matrix	 viewport; 
	Matrix	 view; 
	Matrix	 camera; 
	Matrix	 projection; 
	int      rendermode;
	bool     depths;
	bool     facecul;
	bool     orth;
	float    camRx, camRy, camRz, camZ;
	bool     direction;
	Light    forlight;
	Vertex   lPos;

	public:
	
	void AddModel(Model & model);
	void TransformModel(int model_inx, Matrix transform);
	void SetViewTransform(Matrix view);
	void SetCameraTransform(Matrix cam);
	void SetProjectionTransform(Matrix proj);
	void SetViewport(int left, int right, int top, int bottom);
	
	virtual void InitializeScene(HWND _hWnd);
	virtual void RenderScene(HDC hdc);
	virtual void SetRenderParams(int action);
	
	void DrawString(HDC hdc, int xPos, int yPos, int fSize, COLORREF textColor, LPCTSTR text);
	void DrawTextFrame(HDC hdc);
	void DrawViewportFrame(HDC hdc); 
};

