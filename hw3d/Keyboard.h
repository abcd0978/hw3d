#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public :
		enum class Type
		{
			Press,//�̺�Ʈ ����
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public: 
		Event()//Event �����ڿ� �ƹ��� ���ڵ� �b�� ������ invalid�Ѱɷ� ���� 
			:
			type(Type::Invalid),
			code(0u)//unsigned int
		{}
		Event(Type type, unsigned char code) noexcept
			:type(type),
			code(code)
		{}
		bool IsPress() const noexcept
		{
			return type == Type::Press;
		}
		bool IsRelease() const noexcept
		{
			return type == Type::Release;
		}
	};
public:
		//������ ����
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		//Ű �̺�Ʈ virtual key �̺�Ʈ�� �ٷ�� F1,F2, control etc..
		bool KeyIsPressed(unsigned char keycode) const noexcept;
		Event ReadKey() noexcept;
		bool KetIsEmpty() const noexcept;
		void FlushKey() noexcept;
		//char �̺�Ʈ
		char Readchar() noexcept;
		bool CharIsEmpty() const noexcept;
		void Flushchar() noexcept;
		void Flush() noexcept;//�����ִ� flush�� ���� flushkey�� �θ���.
		//autorepeat ����
		void EnableAutorepeat() noexcept;
		void DisableAutorepeat() noexcept;
		bool AutorepeatIsEnabled() const noexcept;
private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycoed) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	//bitset�� 256���� ��Ʈ�� ��Ÿ���ٴ� �� 
	//virtual key�� �Ҵ�Ȱ�+�Ҵ�ȵȰ����� Ű�ڵ� 0x01~0xFE�� �� 254���� �ִ�
	static constexpr unsigned int bufferSize = 16u;//���۸� ��� queue������
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;//256���� key�� ����ְ� key�� ������ bitset�� key��° �ε����� true�� �ȴ�
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;

};