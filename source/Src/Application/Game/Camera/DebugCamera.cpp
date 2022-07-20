#include"DebugCamera.h"

#include"../../main.h"

void DebugCamera::Init()
{
	SetProjectionMatrix(60.0f);	// 射影行列

	SetLocalPos(m_viewDistance);
}

void DebugCamera::Update()
{
	// マウスの移動量を取得
	POINT nowPos;
	GetCursorPos(&nowPos);

	POINT mouseMove;

	mouseMove.x = nowPos.x - m_prevMousePos.x;
	mouseMove.y = nowPos.y - m_prevMousePos.y;

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		// カメラを回転させる処理
		m_degAngle.x += mouseMove.y * 0.25;
		m_degAngle.y += mouseMove.x * 0.25;
		// 回転制御
	m_degAngle.x = std::clamp(m_degAngle.x, m_minAngleX, m_maxAngleX);
	}

	int wheelVal = APP.m_window.GetMouseWheelVal();

	if (wheelVal != 0)
	{
		m_viewDistance.z += wheelVal * 0.01f;

		SetLocalPos(m_viewDistance);
	}

	if (GetAsyncKeyState(VK_MBUTTON))
	{
		Math::Vector3 vMove;

		vMove.x = mouseMove.x * 0.05f;
		vMove.y = -mouseMove.y * 0.05f;

		vMove = vMove.TransformNormal(vMove, GetRotationMatrix());

		m_mWorld *= Math::Matrix::CreateTranslation(vMove);
	}

	// 次のフレームで使うための準備
	m_prevMousePos = nowPos;

}

void DebugCamera::SetCameraMatrix(const DirectX::SimpleMath::Matrix& mWorld)
{
	FPSCamera::SetCameraMatrix(m_mWorld * mWorld);
}
