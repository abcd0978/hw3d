#pragma once
#include "Window.h"

class App
{
public:
	App();
	//애플리케이션이 시작하는 함수
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
};