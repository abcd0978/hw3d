#pragma once
#include "Window.h"
#include "resource.h"


#define FAILED(Status) ((HRESULT)(Status)<0)
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
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));//LoadImage���� �� HICON���� ĳ�����Ѵ�.
	wc.hCursor = nullptr;//Ŀ��
	wc.hbrBackground = nullptr;//��׶���
	wc.lpszMenuName = nullptr;//�޴�����
	wc.lpszClassName = GetName();//������ â�̸�
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));//LoadImage���� �� HICON���� ĳ�����Ѵ�.
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
	Window::width = width;
	Window::height = height;
	//RECT����ü, ������ ��Ÿ�� ��ũ��, �޴�����-->������
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
	//������ â�� �����ɶ� WM_CREATE �� �Ҹ����� WM_NCCREATE�� �Ҹ���. 
	if (msg == WM_NCCREATE)
	{
		//reinterpret_cast --> lParam�� CREATESTRUCTW*�� ���� ĳ����
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		//CREATESTCURTW ����ü ������ִ� lpCreateParams �� Window*�� ĳ�����ؼ� pWnd�� ����
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//setWindowLongPtr-->(������ �ڵ�, ����������, ���� ���氪)
		//setWindowLongPtr-->WindowAPI�� ���� �����Ҽ� �ְ� �Ѵ�.
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//������ ���ν��� Window::handleMsgThunk�� ���� 
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	//WM_NCCREATE�޼����� �ޱ����� �޼����� �޾�����(�����찡 �����Ǳ� ����) DefWindowProc�� ����
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//������ �ڵ� hWnd��, ����������� ���� ������ WindowPtr�� �����ְ� Window*�� cast�ؼ� pWnd�� ����
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//pWnd(Window*)�� HandleMsg�޼ҵ� ���� ����
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)//�޼����� ������
	{
	case WM_CLOSE://������ â�� �ݴ� �޼����� ���´ٸ�
		PostQuitMessage(0);//����
		return 0;
	

	//keypress�� �� �� ��Ŀ���� �Űܰ����ÿ� KeyRelease ������ ������� �ٽ� �ȿ´�. �׶��� ����� key���۸� ����
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	/***************Ű���� �޼���******************/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		//����ڰ� Ű�� �� ������������ �ݺ��� ���������� �ʵ��� �Ѵ�.
		//AutorepeatIsEnabled�� true�̸� Ű�� �� ������������ �ݺ��� �����Ѵ�.
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
	/***************Ű���� �޼���******************/

	/******************���콺 �޼���****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		//���콺�� window�� ���� �ȿ� �����ִٸ�,
		if (pt.x >= 0 && pt.x < Window::width && pt.y >= 0 && pt.y < Window::height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.isInWindow())
			{
				SetCapture(hWnd);//�� �Լ��� window �ٱ������� ���콺�� ������ ����
				mouse.OnMouseEnter();
			}
		}
		else
		{	//R��ư�̳� L��ư�� ������ ���̿��ٸ�(�巡�� ���̿��ٸ�)
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			//�巡�� ���̾ƴϸ� MouseLeave�̺�Ʈ ȣ��
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
//Window Exception Ŭ���� 
//���� throw�� "Window.h"�Ʒ��� define�� ��ũ�η� �Ѵ�.
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:MyException(line, file),
	hr(hr)//hr �� HRESULT�� ����� ����ִ�
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

std::string Window::Exception::TraslateErrorCode(HRESULT hr) noexcept//���⼭ HRESULT������ ������ ����Ѵ�.
{
	char* pMsgBuf = nullptr;//�޼�������

	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |//�޼����� �޸𸮸� �ý��ۿ��� �Ҵ�
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,//�ü���κ��� ������ ������, �Է¹���
		nullptr, 
		hr, //������ȣ-->HRESULT
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),//�޼����� ��� LANG_NEUTRAL, SUBLANG_DEFAULT�� ���Ѱ�쿡�� �����ǿ��� ������ �ѱ�� ���´�.
		reinterpret_cast<LPSTR>(&pMsgBuf),//������ ���� ���� char* �� LPSTR�� ĳ�����ߴ�
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
	return Exception::hr;//���⼭ raw�� �� HRESULT�� ��ȯ�ϰ�, what���� �̸� ����Ѵ�.
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TraslateErrorCode(Exception::hr);//���⼭ Translate�� HRESULT�� ��ȯ�ϰ�, what���� �̸� ����Ѵ�.
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	//ť�� �޼����� ����������, dequeue�Ѵ�.
	//Peekmessage()�� GetMessage()�� �ٸ��� ������ ��������ʴ´�.
	//
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//quit�޼���(���α׷�����)�� üũ�Ѵ�.
		if (msg.message == WM_QUIT)
		{
			//quit�̸� wParam�� �����Ѵ�.
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	//���� message�� ���ٸ� �� optional�� �����Ѵ�.
	return {};
}