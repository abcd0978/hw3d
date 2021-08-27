#pragma once
#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept :hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);//윈도우 클래스 구조체의 크기를 저장함
	wc.style = CS_OWNDC;//스타일, 윈도우가 하나의 DC를 독점적으로 사용한다.
	wc.lpfnWndProc = HandleMsgSetup;//WndProc, 메세지 처리
	wc.cbClsExtra = 0;//여분메모리량 건들필요없음
	wc.cbWndExtra = 0;//여분메모리량 건들필요없음
	wc.hInstance = GetInstance();//hWnd인스턴스
	wc.hIcon = nullptr;//아이콘
	wc.hCursor = nullptr;//커서
	wc.hbrBackground = nullptr;//백그라운드
	wc.lpszMenuName = nullptr;//메뉴네임
	wc.lpszClassName = GetName();//윈도우 창이름
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()//소멸자
{
	UnregisterClass(wndClassName, GetInstance());
}//인스턴스가 종료되면 자동으로 클래스가 해제된다. 하지만 소멸자에 기입할것

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;//멤버인 wndClassName을 반환
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;//wndClass는 WindowClass를 참조한다
}

Window::Window(int width, int height, const char* name) noexcept
{
	RECT wr;//사각형 구조체 
	wr.left = 100;//윈도우의 안쪽의 100px에서부터 시작한다.
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	//RECT구조체, 윈도우 스타일 매크로, 메뉴여부-->사용안함
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
	//윈도우 창이 생성될때 WM_CREATE 가 불리기전 WM_NCCREATE가 불린다. 
	if (msg == WM_NCCALCSIZE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
	}
}