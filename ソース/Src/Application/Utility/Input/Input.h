#pragma once

enum KeyState
{
	eKeyFree	= 0x0,
	eKeyPress	= 0x1,
	eKeyHold	= 0x2,
	eKeyRelease = 0x4
};

class InputKey
{
public:
	InputKey() {}
	~InputKey(){}

	void Update();

	// 自身が管理するキーの状態を返す関数
	char GetKeyState()const { return m_keyState; }

	void SetKeyCode(UINT code) { m_keyCode = code; }

private:
	char m_keyState = eKeyFree;
	UINT m_keyCode = 0;

};

class InputAxis
{
public:
	void Update();

	const Math::Vector2& GetAxis() const  { return m_axis; }
	void SetKeyCode(UINT up, UINT right, UINT down, UINT left);
private:
	enum AxisKeys
	{
		eUp,
		eRight,
		eDown,
		eLeft,
		eMax
	};

	Math::Vector2 m_axis;

	InputKey m_inputAxisKeys[eMax];
};


class InputController
{
public:
	InputController() { 
		m_inputKeys["Jump"].SetKeyCode(VK_SPACE);
		m_inputKeys["Firing"].SetKeyCode(VK_LBUTTON);
		m_inputKeys["End"].SetKeyCode(VK_ESCAPE);
		m_inputKeys["Debug"].SetKeyCode(VK_TAB);
		// 軸の登録
		m_inputAxis["Move"].SetKeyCode('W', 'D', 'S', 'A');
	}
	~InputController(){}

	void Update();
	const Math::Vector2& GetAxis(const std::string& axisName) const;
	char GetKeyState(const std::string& keyName) const;
private:

	std::unordered_map<std::string, InputKey>  m_inputKeys;
	std::unordered_map<std::string, InputAxis> m_inputAxis;
};