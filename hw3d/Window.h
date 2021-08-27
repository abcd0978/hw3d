#pragma once
#include "directWin.h"

class Window
{
private:
	//�̱���
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;//exception�� ����. ����ó�� ���Ѵٴ¶�
		~WindowClass();//�Ҹ���
		WindowClass(const WindowClass&) = delete;//�̻����ڴ� �������� �ʵ����Ѵ�
		WindowClass& operator=(const WindowClass&) = delete;//�� �����ڴ� �������� �ʵ��� �Ѵ�. ���Ǹ� ������ C++11Ű����
		static constexpr const char* wndClassName = "Direct 3D Engine Window";//Ŭ��������
		static WindowClass wndClass;
		HINSTANCE hInst;//hInstance
		//window class���� �����ϴ� �޼ҵ�� ������ �Ҹ��� getter�ۿ� ����
	};
public:
	Window(int width, int height, const char* name) noexcept;//�ʺ�,���� â�̸�
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hWnd;
};