#pragma once
#include "Window.h"
#include "MyTimer.h"

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
	MyTimer timer;
};