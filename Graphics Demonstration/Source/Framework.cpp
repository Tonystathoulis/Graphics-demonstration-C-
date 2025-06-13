
#include "Framework.h"

//  REFERENCE TO MY FRAMEWORK TO ACCESS MESSAGE HANDLER
Framework *	myFramework = NULL;

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// ENTRY FOR WINDOWS APPLICATION
int APIENTRY wWinMain(_In_	   HINSTANCE hInstance,
				  	  _In_opt_ HINSTANCE hPrevInstance,
					  _In_	   LPWSTR    lpCmdLine,
					  _In_	   int       nCmdshow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	if (myFramework)
	{
		return myFramework->Run(hInstance, nCmdshow);
	}
	return -1;
}


Framework::Framework()
{
	myFramework = this;
}

Framework::~Framework()
{
}

int Framework::Run(HINSTANCE hInstance, int nCmdshow)
{
	hinstance = hInstance;
	if (!InitialiseMainWindow(nCmdshow))
	{
		return -1;
	}
	return MainLoop();
}

int Framework::MainLoop()
{
	MSG msg;

	HACCEL hAccelTable = LoadAccelerators(hinstance, MAKEINTRESOURCE(IDC_RASTERISER));

	// MAIN MESSAGE
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

bool Framework::InitialiseMainWindow(int nCmdshow)
{
	#define MAX_LOADSTRING 100

	WCHAR windowTitle[MAX_LOADSTRING];          
	WCHAR windowClass[MAX_LOADSTRING];            
	
	LoadStringW(hinstance, IDS_APP_TITLE, windowTitle, MAX_LOADSTRING);
	LoadStringW(hinstance, IDC_RASTERISER, windowClass, MAX_LOADSTRING);

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;
	wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_RASTERISER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = windowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassExW(&wcex))
	{
		return false;
	}

	hWnd = CreateWindowW(windowClass, 
						  windowTitle, 
					      WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
					      nullptr, nullptr, hinstance, nullptr);
	if (!hWnd)
	{
		return false;
	}

	

	InitializeScene(hWnd);


	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	return true;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT mesg, WPARAM wParam, LPARAM lParam)
{
	if (myFramework != NULL)
	{
		// IF FRAMEWORK STARTED THE CREATED MESSAGE PROC IS CALLED
		return myFramework->MsgProc(hWnd, mesg, wParam, lParam);
	}
	else
	{
		// ELSE THE DEFAULT MESSAGE PROC IS CALLED
		return DefWindowProc(hWnd, mesg, wParam, lParam);
	}
}

LRESULT Framework::MsgProc(HWND hWnd, UINT mesg, WPARAM wParam, LPARAM lParam)
{
	switch (mesg)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			
			RenderScene(hdc);

			EndPaint(hWnd, &ps);
		}
		break;

		// MOUSE CONTROLS
		case WM_LBUTTONDOWN:
			SetRenderParams(18);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case WM_RBUTTONDOWN:
			SetRenderParams(17);
			InvalidateRect(hWnd, NULL, TRUE);
			break;	

		
		// KEYBOARD CONTROLS
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_SPACE:
				SetRenderParams(0);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case 'P':
			case 'p':
				SetRenderParams(1);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case 'L':
			case 'l':
				SetRenderParams(4);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case VK_RIGHT:
			case 'D':
			case 'd':
				SetRenderParams(13);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case VK_LEFT:
			case 'A':
			case 'a':
				SetRenderParams(14);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case VK_UP:
			case 'W':
			case 'w':
				SetRenderParams(12);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case VK_DOWN:
			case 'S':
			case 's':
				SetRenderParams(11);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case 'R':
			case 'r':
				SetRenderParams(10);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case 'F':
			case 'f':
				SetRenderParams(2);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case 'T':
			case 't':
				SetRenderParams(3);
				InvalidateRect(hWnd, NULL, TRUE);
				break;


			default:
				break;
			}
		}
		break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, mesg, wParam, lParam);
	}
	return 0;
}

void Framework::InitializeScene(HWND hWnd)
{

}

void Framework::RenderScene(HDC hdc)
{
}

void Framework::SetRenderParams(int action)
{
}



