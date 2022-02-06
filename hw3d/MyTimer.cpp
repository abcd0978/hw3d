#include "MyTimer.h"

//using namespace std::chrono;

//�����ڸ� �θ��� �ð������ �����Ѵ�.
MyTimer::MyTimer()
{
	this->last = std::chrono::steady_clock::now();
}

float MyTimer::Mark()
{
	//������ ��� �ð��� old�� ����
	const auto old = last;
	//����ð��� last�� ����
	last = std::chrono::steady_clock::now();
	//Ŭ������ �����ߴ� �ð��� mark�� �θ����� ���� ����Ѵ�.
	const std::chrono::duration<float> frameTime = last - old;
	return frameTime.count();//�� ���� ��ȯ�Ѵ�. 
}

float MyTimer::Peek() const
{
	//Ÿ�̸��� peek�� ����
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}