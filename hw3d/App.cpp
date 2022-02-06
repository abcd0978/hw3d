#include "App.h"

App::App()
	:wnd(800, 600, "game")
{}

//Go�Լ������� ����Ǵ� ���α׷������� �޼����� ó���Ѵ�.
int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());
}