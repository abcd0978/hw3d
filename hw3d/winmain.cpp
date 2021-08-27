#include "directWin.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);//wParam에 저장됨
		break;
	case WM_KEYDOWN:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, "Respects");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, "NoRespects");
		}
		break;
	case WM_CHAR:
	{
		static std::string title;//변수 선언할때는 중괄호로 묶어야..
		title.push_back((char)wParam);
		SetWindowText(hWnd, title.c_str());
		break;
	}

	case WM_LBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);//마우스 포인트
		std::ostringstream oss;
		oss << "(" << pt.x << "," << pt.y << "," << ")";//마우스포인트 x,y
		SetWindowText(hWnd, oss.str().c_str());
		break;
	}

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       mCmdShow)
{
	const auto className = "hw3d";
	//registerwindow class
	WNDCLASSEX wc = { 0 };//윈도우 클래스
	/*typedef struct tagWNDCLASSEXA { 이게 원형임
		UINT      cbSize;
		UINT      style;
		WNDPROC   lpfnWndProc;
		int       cbClsExtra;
		int       cbWndExtra;
		HINSTANCE hInstance;
		HICON     hIcon;
		HCURSOR   hCursor;
		HBRUSH    hbrBackground;
		LPCWSTR    lpszMenuName;
		LPCWSTR    lpszClassName;
		HICON     hIconSm;
	} WNDCLASSEXA, * PWNDCLASSEXA, * NPWNDCLASSEXA, * LPWNDCLASSEXA;*/
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = className;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
	//window instance
	HWND hWnd = CreateWindowEx(
		0, className, "this is window",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,//창테두리, 최소화, 시스템메뉴
		200,200,640,480,//창크기 등등
		nullptr,nullptr,hInstance,nullptr
	);
	ShowWindow(hWnd, SW_SHOW);

	MSG msg;
	BOOL gResult;
	while (gResult = (GetMessage(&msg, nullptr, 0, 0) > 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}