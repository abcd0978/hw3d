#pragma once
#include "Keyboard.h"
//키 이벤트 virtual key 이벤트를 다룬다 F1,F2, control etc..
bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return keystates[keycode];//bitset(256개의 키보드 키)의 key(누른키의인덱스)번째가 true인지 false인지 리턴
}
Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keybuffer.size() > 0u)//queue사이즈가 0보다 크면,
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;//pop해서 가지고온다
	}
	else
	{
		return Keyboard::Event();//queue사이즈가 0보다 작다면
		//default 생성자를 부른다(invaild함)
	}
}
bool Keyboard::KetIsEmpty() const noexcept
{
	return keybuffer.empty();//queue의 isempty
}
void Keyboard::FlushKey() noexcept
{
	keybuffer = std::queue<Event>();//queue를 Flush하는 방법: 그냥 새로 선언해서 대입한다.
}
//char 이벤트
char Keyboard::Readchar() noexcept
{
	if (charbuffer.size() > 0u)//queue사이즈가 0보다 크면
	{
		unsigned char charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;//queue에 front에 있는거 복사해서 리턴하고 pop함
	}
	else
	{
		return 0;
	}
}
bool Keyboard::CharIsEmpty() const noexcept
{
	return charbuffer.empty();
}
void Keyboard::Flushchar() noexcept
{
	charbuffer = std::queue<char>();
}
void Keyboard::Flush() noexcept
{
	Flushchar();
	FlushKey();
}
//autorepeat 제어
void Keyboard::EnableAutorepeat() noexcept
{
	Keyboard::autorepeatEnabled = true;
}
void Keyboard::DisableAutorepeat() noexcept
{
	Keyboard::autorepeatEnabled = false;
}
bool Keyboard::AutorepeatIsEnabled() const noexcept
{
	return autorepeatEnabled;
}
//"Window.cpp가 사용하는 거"
//wParam이 unsigned char로 캐스팅 되어 들어온다.
void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	keystates[keycode] = true;//키가 눌릴시에 true로 변경
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	//Press 타입으로 Event를 push한다.
	TrimBuffer(keybuffer);
}
void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	keystates[keycode] = false;//키를 뗄시에 false로 변경
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	//Relase 타입으로 Event를 push한다.
	TrimBuffer(keybuffer);
}
void Keyboard::OnChar(char character) noexcept
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}
void Keyboard::ClearState() noexcept
{
	keystates.reset();//bitset을 보두 0으로 한다.
}
template<typename T>//Event와 char 두개의 타입에서 사용하므로 탬플릿사용
static void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	//헤더에서 정의한 bufferSize = 16보다 queue의 크기가 커지면 강제로 pop시킨다.
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}