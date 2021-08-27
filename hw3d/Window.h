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

//���� throw ��ũ�� 
#define CHWND_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,hr)
//last������ throw�Ѵ�.
#define CHWND_LAST_EXCEPT() Window::Exception(__LINE__,__FILE__,GetLastError());