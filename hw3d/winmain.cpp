#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	try
	{
		Window wnd(500, 500, "game");

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (gResult == -1)
		{
			return -1;
		}
		return msg.wParam;
	}
	catch (const MyException& e)//�ռ� ������ ���ܿ�����
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)//���Ĵٵ� exception
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)//���� �𸣸�
	{
		MessageBox(nullptr,"unknown error","WTF", MB_OK | MB_ICONEXCLAMATION);
	}

}