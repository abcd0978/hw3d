#pragma once
#include "Window.h"

class App
{
public:
	App();
	//���ø����̼��� �����ϴ� �Լ�
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
};