#include "directWin.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);//wParam�� �����
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
		static std::string title;//���� �����Ҷ��� �߰�ȣ�� �����..
		title.push_back((char)wParam);
		SetWindowText(hWnd, title.c_str());
		break;
	}

	case WM_LBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);//���콺 ����Ʈ
		std::ostringstream oss;
		oss << "(" << pt.x << "," << pt.y << "," << ")";//���콺����Ʈ x,y
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
	WNDCLASSEX wc = { 0 };//������ Ŭ����
	/*typedef struct tagWNDCLASSEXA { �̰� ������
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
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,//â�׵θ�, �ּ�ȭ, �ý��۸޴�
		200,200,640,480,//âũ�� ���
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