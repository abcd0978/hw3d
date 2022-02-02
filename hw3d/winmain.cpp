#include "Window.h"
#include "App.h"
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
)
{
	try
	{
		App app1;
		app1.Go();
	}
	catch (const MyException& e)//앞서 정의한 예외에대해
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)//스탠다드 exception
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)//뭔지 모르면
	{
		MessageBox(nullptr,"unknown error","WTF", MB_OK | MB_ICONEXCLAMATION);
	}

}