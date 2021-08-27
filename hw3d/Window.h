#pragma once
#include "directWin.h"
#include "MyException.h"

class Window
{
public:
	class Exception : public MyException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TraslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	//싱글턴
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;//exception이 없다. 예외처리 안한다는뜻
		~WindowClass();//소멸자
		WindowClass(const WindowClass&) = delete;//이생성자는 생성되지 않도록한다
		WindowClass& operator=(const WindowClass&) = delete;//이 연산자는 생성되지 않도록 한다. 정의를 금지함 C++11키워드
		static constexpr const char* wndClassName = "Direct 3D Engine Window";//클래스네임
		static WindowClass wndClass;
		HINSTANCE hInst;//hInstance
		//window class에서 구현하는 메소드는 생성자 소멸자 getter밖에 없다
	};
public:
	Window(int width, int height, const char* name) noexcept;//너비,높이 창이름
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

//에러 throw 매크로 
#define CHWND_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,hr)
//last에러를 throw한다.
#define CHWND_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError());