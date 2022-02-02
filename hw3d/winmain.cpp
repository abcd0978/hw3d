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