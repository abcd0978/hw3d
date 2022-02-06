#pragma once
#include "Window.h"
#include "resource.h"


#define FAILED(Status) ((HRESULT)(Status)<0)
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
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));//LoadImage를한 후 HICON으로 캐스팅한다.
	wc.hCursor = nullptr;//커서
	wc.hbrBackground = nullptr;//백그라운드
	wc.lpszMenuName = nullptr;//메뉴네임
	wc.lpszClassName = GetName();//윈도우 창이름
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));//LoadImage를한 후 HICON으로 캐스팅한다.
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
	Window::width = width;
	Window::height = height;
	//RECT구조체, 윈도우 스타일 매크로, 메뉴여부-->사용안함
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	//CreateWindow-->
	hWnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}
Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//윈도우 창이 생성될때 WM_CREATE 가 불리기전 WM_NCCREATE가 불린다. 
	if (msg == WM_NCCREATE)
	{
		//reinterpret_cast --> lParam을 CREATESTRUCTW*로 강제 캐스팅
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		//CREATESTCURTW 구조체 멤버에있는 lpCreateParams 를 Window*로 캐스팅해서 pWnd로 저장
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//setWindowLongPtr-->(윈도우 핸들, 설정변경상수, 설정 변경값)
		//setWindowLongPtr-->WindowAPI의 값을 저장할수 있게 한다.
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//윈도우 프로시저 Window::handleMsgThunk로 변경 
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	//WM_NCCREATE메세지를 받기전에 메세지를 받았으면(윈도우가 생성되기 전에) DefWindowProc를 리턴
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//윈도우 핸들 hWnd와, 가져오고싶은 값을 넣으면 WindowPtr로 돌려주고 Window*로 cast해서 pWnd에 저장
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//pWnd(Window*)의 HandleMsg메소드 값을 리턴
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)//메세지가 들어오면
	{
	case WM_CLOSE://윈도우 창을 닫는 메세지가 들어온다면
		PostQuitMessage(0);//끝냄
		return 0;
	

	//keypress를 한 후 포커스가 옮겨갔을시에 KeyRelease 사인은 윈도우로 다시 안온다. 그때를 대비해 key버퍼를 지움
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	/***************키보드 메세지******************/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		//사용자가 키를 꾹 누르고있을때 반복을 수행을하지 않도록 한다.
		//AutorepeatIsEnabled가 true이면 키를 꾹 누르고있으면 반복을 수행한다.
		if (!(lParam & 0x40000000/*KF_REPEAT*/) || kbd.AutorepeatIsEnabled() )
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	/***************키보드 메세지******************/

	/******************마우스 메세지****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		//마우스가 window의 영역 안에 들어와있다면,
		if (pt.x >= 0 && pt.x < Window::width && pt.y >= 0 && pt.y < Window::height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.isInWindow())
			{
				SetCapture(hWnd);//이 함수로 window 바깥에서도 마우스를 잡을수 있음
				mouse.OnMouseEnter();
			}
		}
		else
		{	//R버튼이나 L버튼을 누르는 중이였다면(드래깅 중이였다면)
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			//드래깅 중이아니면 MouseLeave이벤트 호출
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}



	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}
}
//Window Exception 클래스 
//예외 throw는 "Window.h"아래에 define한 매크로로 한다.
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:MyException(line, file),
	hr(hr)//hr 즉 HRESULT가 결과를 담고있다
{}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code]" << GetErrorCode() << std::endl
		<< "[Description]" << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string Window::Exception::TraslateErrorCode(HRESULT hr) noexcept//여기서 HRESULT에대한 번역을 담당한다.
{
	char* pMsgBuf = nullptr;//메세지버퍼

	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |//메세지의 메모리를 시스템에서 할당
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,//운영체제로부터 오류를 가져옴, 입력무시
		nullptr, 
		hr, //에러번호-->HRESULT
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),//메세지의 언어 LANG_NEUTRAL, SUBLANG_DEFAULT로 정한경우에는 제어판에서 설정한 한국어가 나온다.
		reinterpret_cast<LPSTR>(&pMsgBuf),//에러를 담을 버퍼 char* 를 LPSTR로 캐스팅했다
		0, 
		nullptr
	);

	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

const char* Window::Exception::GetType() const noexcept
{
	return "Window Exception";
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return Exception::hr;//여기서 raw로 된 HRESULT를 반환하고, what에서 이를 출력한다.
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TraslateErrorCode(Exception::hr);//여기서 Translate된 HRESULT를 반환하고, what에서 이를 출력한다.
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	//큐에 메세지가 남아있을때, dequeue한다.
	//Peekmessage()는 GetMessage()와 다르게 무한정 대기하지않는다.
	//
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//quit메세지(프로그램종료)를 체크한다.
		if (msg.message == WM_QUIT)
		{
			//quit이면 wParam을 리턴한다.
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	//만약 message가 없다면 빈 optional을 리턴한다.
	return {};
}