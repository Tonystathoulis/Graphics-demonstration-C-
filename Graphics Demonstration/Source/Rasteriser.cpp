
#include <windowsx.h>
#include "MD2Loader.h"
#include "Rasteriser.h"

Rasteriser app;

void Rasteriser::AddModel(Model & model)
{
	scenemodels.push_back(model);
}

void Rasteriser::TransformModel(int modelinx, Matrix transform)
{
	if (modelinx > scenemodels.size()-1)
		return;

	scenemodels[modelinx].ApplyTransform(transform);
}

void Rasteriser::SetViewTransform(Matrix view)
{
	view.SetMatrix(view);
}

void Rasteriser::SetCameraTransform(Matrix cam)
{
	camera.SetMatrix(cam);
}

void Rasteriser::SetProjectionTransform(Matrix proj)
{
	projection.SetMatrix(proj);
}

void Rasteriser::SetViewport(int left, int right, int top, int bot)
{
	viewrect.left = left;
	viewrect.top = top;
	viewrect.right = right;
	viewrect.bottom = bot;
	viewport.SetViewport(left, right, top, bot);
}

void Rasteriser::DrawString(HDC hdc, int xPos, int yPos, int fSize, COLORREF textColor, LPCTSTR text)
{
	HFONT hFont, hOldFont;

	// RETRIEVE A HANDLE TO MY FONT
	hFont = hFont = CreateFont(fSize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// SELECT THE VARIABLE FONT
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{

		SetTextColor(hdc, textColor);
		SetBkColor(hdc, RGB(255, 255, 255));


		TextOut(hdc, xPos, yPos, text, lstrlen(text));

		// RESET THE FONT        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}

void Rasteriser::RenderScene(HDC hdc)
{
	Model model;
	Vertex lightPos;

	// EYE POSITION
	float eX, eY, eZ;
	eX = -camera.GetM(0, 3);
	eY = -camera.GetM(1, 3);
	eZ = -camera.GetM(2, 3);

	DrawViewportFrame(hdc);

	for (size_t i = 0; i < scenemodels.size(); i++)
	{
		model = scenemodels[i];

		model.ApplyTransform(camera);

		model.CalculateCenters();

		if (depths)	model.ApplyDepthSorting();

		model.CalculateCenters();
		model.CalculateNormals(true);

		lightPos = camera * lPos;
		forlight.SetLPos(lightPos);

		if (rendermode == 2) model.CalcFlatShading(hdc, forlight, eX, eY, eZ, orth);

		model.ApplyTransform(projection);
		model.Dehomogenise();

		model.CalculateCenters();
		model.CalculateNormals(true);

		if (facecul)
		{
			model.MarkBackfaces(eX, eY, eZ, orth);
		}

		model.ApplyTransform(viewport);

		switch (rendermode)
		{
		case  0: model.DrawWireFrame(hdc);
			break;
		case  1: model.DrawSolidFill(hdc);
			break;
		case  2: model.DrawFlatShading(hdc);
			break;
		default: model.DrawWireFrame(hdc);
			break;
		}

	}

	DrawTextFrame(hdc);
}

void Rasteriser::DrawTextFrame(HDC hdc)
{
	POINT points[4];
	wchar_t outstr[256];
	

	//TEXT FOR MODES AND CONTROLS
	switch (rendermode)
	{
	case  0: DrawString(hdc, 920 + 10, 50 + 10, 18, RGB(0, 0, 128), L"Draw: Wireframe    (Space)");
		break;
	case  1: DrawString(hdc, 920 + 10, 50 + 10, 18, RGB(128, 0, 0), L"Draw: Solid-Fill    (Space)");
		break;
	case  2: DrawString(hdc, 920 + 10, 50 + 10, 18, RGB(128, 0, 128), L"Draw: Flat-Shading    (Space)");
		break;
	default: DrawString(hdc, 920 + 10, 50 + 10, 18, RGB(0, 128, 0), L"Draw: Wireframe    (Space)");
		break;
	}

	if (depths)	DrawString(hdc, 920 + 10, 50 + 30, 18, RGB(0, 128, 0), L"Depth Sorting: ON    (T)");
	else DrawString(hdc, 920 + 10, 50 + 30, 18, RGB(255, 0, 0), L"Depth Sorting: OFF    (T)");

	if (facecul)	DrawString(hdc, 920 + 10, 50 + 50, 18, RGB(0, 128, 0), L"Back-face Culling: ON    (F)"); 
	else DrawString(hdc, 920 + 10, 50 + 50, 18, RGB(255, 0, 0), L"Back-face Culling: OFF    (F)");

	if (orth)	DrawString(hdc, 920 + 10, 50 + 70, 18, RGB(128, 128, 128), L"Projection: Ortho    (P)");
	else DrawString(hdc, 920 + 10, 50 + 70, 18, RGB(96, 96, 96), L"Projection: Perspective    (P)");

	swprintf(outstr,256,L"Camera: RotX=%.2f; RotY=%.2f; RotZ=%.2f; PosZ=%.2f;\0", camRx, camRy, camRz, camZ);
	DrawString(hdc, 920 + 10, 50 + 90, 18, RGB(64, 64, 64), (LPCTSTR)outstr);

	if (direction)	DrawString(hdc, 920 + 10, 50 + 110, 18, RGB(128, 128, 0), L"Light: Directional    (L)");
	else DrawString(hdc, 920 + 10, 50 + 110, 18, RGB(0, 192, 0), L"Light: Point-light    (L)");

	swprintf(outstr, 256, L"Light Position: PosX=%.2f; PosY=%.2f; PosZ=%.2f;\0", 
							forlight.GetLPos().GetX(), forlight.GetLPos().GetY(), forlight.GetLPos().GetZ());
	DrawString(hdc, 920 + 10, 50 + 130, 18, RGB(64, 64, 0), (LPCTSTR)outstr);

	swprintf(outstr, 256, L"Light Color: La(%d, %d, %d); Ld(%d, %d, %d); Ls(%d, %d, %d);\0", 
							GetRValue(forlight.GetColorLa()), GetGValue(forlight.GetColorLa()), GetBValue(forlight.GetColorLa()),
							GetRValue(forlight.GetColorLd()), GetGValue(forlight.GetColorLd()), GetBValue(forlight.GetColorLd()), 
							GetRValue(forlight.GetColorLs()), GetGValue(forlight.GetColorLs()), GetBValue(forlight.GetColorLs()) );
	DrawString(hdc, 920 + 10, 50 + 150, 18, RGB(96, 96, 0), (LPCTSTR)outstr);

DrawString(hdc, 920 + 10, 50 + 170, 18, RGB(96, 96, 96), L"Controls:");
DrawString(hdc, 920 + 10, 50 + 190, 18, RGB(96, 96, 96), L"(WASD) or Arrow keys to rotate, (LMB) zoom in / (RMB) zoom out");
DrawString(hdc, 920 + 10, 50 + 210, 18, RGB(96, 96, 96), L"(R) to reset, (Space) to draw, (L) change light, (P) change projection ");
DrawString(hdc, 920 + 10, 50 + 230, 18, RGB(96, 96, 96), L"(F) Backface culling, (T) Depth sorting");


}

void Rasteriser::InitializeScene(HWND hWnd)
{
	// LOAD AND ADD MODEL
	Model MyModel;
	MD2Loader MyLoader;

	//FIRST MODEL
	MyLoader.LoadModel("fighter.md2", MyModel);
	MyModel.SetColorKa(RGB(100, 100, 100));
	MyModel.SetColorKd(RGB(100, 100, 100));
	MyModel.SetColorKs(RGB(150, 111, 214));
	MyModel.SetShininess(80.8f);
	AddModel(MyModel);
	//SECOND MODEL
	MyLoader.LoadModel("marvin.md2", MyModel);
	MyModel.SetColorKa(RGB(135, 0, 0));
	MyModel.SetColorKd(RGB(135, 0, 0));
	MyModel.SetColorKs(RGB(135, 111, 214));
	MyModel.SetShininess(83.2f);
	AddModel(MyModel);


	// MODEL TRANSFORMATION MATRIX
	Matrix model_transform, transform;
	// TRANSFORMATIONS
	model_transform.SetIdentity();
	transform.SetIdentity();
	transform.SetScale(0.4,0.4,0.4);
	model_transform = transform * model_transform;

	
	transform.SetRotationY(-180);
	model_transform = transform * model_transform;
	

	transform.SetTranslate(30, 30, 0);
	model_transform = transform * model_transform;

	
	TransformModel(0, model_transform);


	model_transform.SetIdentity();
	transform.SetIdentity();
	
	transform.SetRotationY(-90);
	model_transform = transform * model_transform;
	
	TransformModel(1, model_transform);


	
	SetViewport(50, 50 + 600, 50, 50 + 600);

	// CAMERA MATRIX
	camRx = 0.0f;
	camRy = 0.0f;
	camRz = 0.0f;
	camZ = 80.0f;
	Matrix camera;
	camera.SetCamera(camRx, camRy, camRz, 0, 0, camZ);
	SetCameraTransform(camera);

	// PROJECTION MATRIX
	orth = false;
	Matrix projection;
	if (orth) projection.SetOrtho(6.0f);
		else projection.SetPerspective(200.0f, 1.0f);
	
	// VIEWVOLUME MATRIX
	Matrix volume;
	volume.SetViewvolume(-200, +200, -200, +200, +200, -200);

	
	Matrix view;
	view = volume * projection;
	SetProjectionTransform(view);

	rendermode = 0;
	depths = false;
	facecul = false;
	direction = true;
	forlight.SetDirectional(direction);
	lPos.SetX(-100.0f);
	lPos.SetY(100.0f);
	lPos.SetZ(100.0f);
	forlight.SetLPos(lPos);
	forlight.SetColorLa(RGB(32, 32, 32));
	forlight.SetColorLd(RGB(255, 255, 0));
	forlight.SetColorLs(RGB(255, 255, 255));

	forlight.SetColorLa(RGB(128, 128, 128));
	forlight.SetColorLd(RGB(255, 255, 255));
	forlight.SetColorLs(RGB(255, 255, 255));

}

void Rasteriser::SetRenderParams(int action)
{
	Matrix camera, projection, volume, view;

	switch (action)
	{
	case  0: if (rendermode < 2) rendermode = rendermode + 1; else rendermode = 0;
		break;
	case  1: orth = !orth;
		
			 if (orth) projection.SetOrtho(6.0f);
			 else projection.SetPerspective(200.0f, 1.0f);

			 volume.SetViewvolume(-200, +200, -200, +200, +200, -200);
			 view = volume * projection;
			 SetProjectionTransform(view);
		break;
	case  2: facecul = !facecul;
		break;
	case  3: depths = !depths;
		break;
	case  4: direction = !direction;
			 forlight.SetDirectional(direction);
		break;
	case 10:	 camRx = 0.0f;
				 camRy = 0.0f;
				 camRz = 0.0f;
				 camZ = 80.0f;
				 camera.SetCamera(camRx, camRy, camRz, 0, 0, camZ);
				 SetCameraTransform(camera);
		break;
	case 11:	 camRx = (int)(camRx + 10.0f) % 360;
				camera.SetCamera(camRx, camRy, camRz, 0, 0, camZ);
				SetCameraTransform(camera);
		break;
	case 12:	camRx = (int)(camRx - 10.0f) % 360;
				camera.SetCamera(camRx, camRy, camRz, 0, 0, camZ);
				SetCameraTransform(camera);
		break;
	case 13:	camRy = (int)(camRy + 10.0f) % 360;
				camera.SetCamera(camRx, camRy, camRz, 0, 0, camZ);
				SetCameraTransform(camera);
		break;
	case 14:	camRy = (int)(camRy - 10.0f) % 360;
				camera.SetCamera(camRx, camRy, camRz, 0, 0, camZ);
				SetCameraTransform(camera);
		break;
	case 15:	camRz = (int)(camRz + 10.0f) % 360;
				camera.SetCamera(camRx, camRy, camRz, 0, 0, camZ);
				SetCameraTransform(camera);
		break;
	case 16:	camRz = (int)(camRz - 10.0f) % 360;
				camera.SetCamera(camRx, camRy, camRz, 0, 0, camZ);
				SetCameraTransform(camera);
		break;
	case 17:	camZ = camZ + 10.0f;
				camera.SetCamera(camRx, camRy, camRz, 0, 0, camZ);
				SetCameraTransform(camera);
		break;
	case 18:	camZ = camZ - 10.0f;
				camera.SetCamera(camRx, camRy, camRz, 0, 0, camZ);
				SetCameraTransform(camera);
		break;
	}
}

void Rasteriser::DrawViewportFrame(HDC hdc)
{
	POINT points[4];
	//VIEWPORT FRAME MATCHES WITH BACKGROUND SO NO DELETION IS NEEDED
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HGDIOBJ oldPen = SelectPen(hdc, pen);

	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH oldBrush = SelectBrush(hdc, brush);

	points[0].x = (int)viewrect.left;      points[0].y = (int)viewrect.top;
	points[1].x = (int)viewrect.right - 1; points[1].y = (int)viewrect.top;
	points[2].x = (int)viewrect.right - 1; points[2].y = (int)viewrect.bottom - 1;
	points[3].x = (int)viewrect.left;      points[3].y = (int)viewrect.bottom - 1;

	Polygon(hdc, points, 4);

	SelectBrush(hdc, oldBrush);
	DeleteObject(brush);

	SelectPen(hdc, oldPen);
	DeleteObject(pen);

}


