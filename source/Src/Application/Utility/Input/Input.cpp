#include"Input.h"
void InputController::Update()
{
	for (std::pair<const std::string, InputKey>& key: m_inputKeys)
	{
		key.second.Update();
	}
	for (std::pair<const std::string, InputAxis>& inputAxis : m_inputAxis)
	{
		inputAxis.second.Update();
	}
}

const Math::Vector2& InputController::GetAxis(const std::string& axisName) const
{
	auto foundItr = m_inputAxis.find(axisName);

#ifdef _DEBUG

	if (foundItr == m_inputAxis.end())	// 配列の中にない
	{
		assert(0 && "登録されていないキーの検索です");

		return Math::Vector2::Zero;
	}

#endif

	return foundItr->second.GetAxis();
}

char InputController::GetKeyState(const std::string& keyName) const
{
	auto foundItr = m_inputKeys.find(keyName);
	
#ifdef _DEBUG

	if (foundItr == m_inputKeys.end())	// 配列の中にない
	{
		assert(0 && "登録されていないキーの検索です");

		return eKeyFree;
	}

#endif

	return foundItr->second.GetKeyState();
}

void InputKey::Update()
{
	if (GetAsyncKeyState(m_keyCode))
	{
			if (m_keyState & eKeyHold)	// キーホールドが既に立っていた(2f目以降)
			{
				// プレスフラグ(押した瞬間フラグ)をOFFにする
				m_keyState &= ~eKeyHold;		// 1101をかけることによってHoldの2番目だけを0にできる
			}
			else
			{
				m_keyState |= eKeyPress | eKeyHold;	// 必ず0011になる
				m_keyState &= ~eKeyRelease;
			}
		}
	else
	{
			// 離された瞬間
			if (m_keyState & eKeyHold)
			{
				m_keyState |= eKeyRelease;
			}
			else
			{
				m_keyState &= ~eKeyRelease;
			}
			m_keyState &= ~eKeyPress;
			m_keyState &= ~eKeyHold;
	}
}

void InputAxis::Update()
{
	for (InputKey& key : m_inputAxisKeys)
	{
		key.Update();
	}

	m_axis = Math::Vector2::Zero;

	if (m_inputAxisKeys[eUp].GetKeyState())		{ m_axis.y += 1.0f; }
	if (m_inputAxisKeys[eDown].GetKeyState())	{ m_axis.y -= 1.0f; }
	if (m_inputAxisKeys[eRight].GetKeyState())	{ m_axis.x += 1.0f; }
	if (m_inputAxisKeys[eLeft].GetKeyState())	{ m_axis.x -= 1.0f; }
}

void InputAxis::SetKeyCode(UINT up, UINT right, UINT down, UINT left)
{
 	m_inputAxisKeys[0].SetKeyCode(up);
	m_inputAxisKeys[1].SetKeyCode(right);
	m_inputAxisKeys[2].SetKeyCode(down);
	m_inputAxisKeys[3].SetKeyCode(left);
}