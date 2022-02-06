#include "MyTimer.h"

//using namespace std::chrono;

//생성자를 부르면 시간을재기 시작한다.
MyTimer::MyTimer()
{
	this->last = std::chrono::steady_clock::now();
}

float MyTimer::Mark()
{
	//이전에 쟀던 시간을 old에 대입
	const auto old = last;
	//현재시간을 last에 대입
	last = std::chrono::steady_clock::now();
	//클래스를 생성했던 시간과 mark를 부를때의 차를 계산한다.
	const std::chrono::duration<float> frameTime = last - old;
	return frameTime.count();//그 차를 반환한다. 
}

float MyTimer::Peek() const
{
	//타이머의 peek과 같음
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}