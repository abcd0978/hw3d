#pragma once
#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept :hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);//������ Ŭ���� ����ü�� ũ�⸦ ������
	wc.style = CS_OWNDC;//��Ÿ��, �����찡 �ϳ��� DC�� ���������� ����Ѵ�.
	wc.lpfnWndProc = HandleMsgSetup;//WndProc, �޼��� ó��
	wc.cbClsExtra = 0;//���и޸𸮷� �ǵ��ʿ����
	wc.cbWndExtra = 0;//���и޸𸮷� �ǵ��ʿ����
	wc.hInstance = GetInstance();//hWnd�ν��Ͻ�
	wc.hIcon = nullptr;//������
	wc.hCursor = nullptr;//Ŀ��
	wc.hbrBackground = nullptr;//��׶���
	wc.lpszMenuName = nullptr;//�޴�����
	wc.lpszClassName = GetName();//������ â�̸�
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()//�Ҹ���
{
	UnregisterClass(wndClassName, GetInstance());
}//�ν��Ͻ��� ����Ǹ� �ڵ����� Ŭ������ �����ȴ�. ������ �Ҹ��ڿ� �����Ұ�

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;//����� wndClassName�� ��ȯ
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;//wndClass�� WindowClass�� �����Ѵ�
}

Window::Window(int width, int height, const char* name) noexcept
{
	RECT wr;//�簢�� ����ü 
	wr.left = 100;//�������� ������ 100px�������� �����Ѵ�.
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	//RECT����ü, ������ ��Ÿ�� ��ũ��, �޴�����-->������
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	//CreateWindow-->
	this->hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this);
}
Window::~Window()
{
	DestroyWindow(this->hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//������ â�� �����ɶ� WM_CREATE �� �Ҹ����� WM_NCCREATE�� �Ҹ���. 
	if (msg == WM_NCCALCSIZE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
	}
}