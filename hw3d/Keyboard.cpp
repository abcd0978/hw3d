#pragma once
#include "Keyboard.h"
//Ű �̺�Ʈ virtual key �̺�Ʈ�� �ٷ�� F1,F2, control etc..
bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return keystates[keycode];//bitset(256���� Ű���� Ű)�� key(����Ű���ε���)��°�� true���� false���� ����
}
Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keybuffer.size() > 0u)//queue����� 0���� ũ��,
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;//pop�ؼ� ������´�
	}
	else
	{
		return Keyboard::Event();//queue����� 0���� �۴ٸ�
		//default �����ڸ� �θ���(invaild��)
	}
}
bool Keyboard::KetIsEmpty() const noexcept
{
	return keybuffer.empty();//queue�� isempty
}
void Keyboard::FlushKey() noexcept
{
	keybuffer = std::queue<Event>();//queue�� Flush�ϴ� ���: �׳� ���� �����ؼ� �����Ѵ�.
}
//char �̺�Ʈ
char Keyboard::Readchar() noexcept
{
	if (charbuffer.size() > 0u)//queue����� 0���� ũ��
	{
		unsigned char charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;//queue�� front�� �ִ°� �����ؼ� �����ϰ� pop��
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
//autorepeat ����
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
//"Window.cpp�� ����ϴ� ��"
//wParam�� unsigned char�� ĳ���� �Ǿ� ���´�.
void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	keystates[keycode] = true;//Ű�� �����ÿ� true�� ����
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	//Press Ÿ������ Event�� push�Ѵ�.
	TrimBuffer(keybuffer);
}
void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	keystates[keycode] = false;//Ű�� ���ÿ� false�� ����
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	//Relase Ÿ������ Event�� push�Ѵ�.
	TrimBuffer(keybuffer);
}
void Keyboard::OnChar(char character) noexcept
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}
void Keyboard::ClearState() noexcept
{
	keystates.reset();//bitset�� ���� 0���� �Ѵ�.
}
template<typename T>//Event�� char �ΰ��� Ÿ�Կ��� ����ϹǷ� ���ø����
static void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	//������� ������ bufferSize = 16���� queue�� ũ�Ⱑ Ŀ���� ������ pop��Ų��.
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}