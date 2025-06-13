
#pragma once

#include <windows.h>
#include "resource.h"
#include "Model.h"
#include "Matrix4D.h"

class Framework
{
public:
	Framework();
	virtual ~Framework();

	int Run(HINSTANCE hInstance, int nCmdShow);

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void InitializeScene(HWND _hWnd);
	virtual void RenderScene(HDC hdc);

	virtual void SetRenderParams(int action);

private:
	HINSTANCE	hinstance;
	HWND		hWnd;

	bool InitialiseMainWindow(int nCmdShow);
	int MainLoop();

};