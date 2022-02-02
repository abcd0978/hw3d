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
			Press,//이벤트 종류
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public: 
		Event()//Event 생성자에 아무런 인자도 얺지 않으면 invalid한걸로 간주 
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
		//생성자 정의
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		//키 이벤트 virtual key 이벤트를 다룬다 F1,F2, control etc..
		bool KeyIsPressed(unsigned char keycode) const noexcept;
		Event ReadKey() noexcept;
		bool KetIsEmpty() const noexcept;
		void FlushKey() noexcept;
		//char 이벤트
		char Readchar() noexcept;
		bool CharIsEmpty() const noexcept;
		void Flushchar() noexcept;
		void Flush() noexcept;//여기있는 flush는 위의 flushkey도 부른다.
		//autorepeat 제어
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
	//bitset을 256개의 비트로 나타낸다는 뜻 
	//virtual key는 할당된것+할당안된것포함 키코드 0x01~0xFE로 총 254개가 있다
	static constexpr unsigned int bufferSize = 16u;//버퍼를 담는 queue사이즈
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;//256개의 key를 담고있고 key가 눌리면 bitset의 key번째 인덱스가 true가 된다
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;

};