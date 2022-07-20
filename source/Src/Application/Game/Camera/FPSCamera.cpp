#include "../GameSystem.h"
#include "FPSCamera.h"
#include"../GameObject/Gun/Gun.h"
const POINT FPSCamera::s_fixMousePos = { 640, 360 };
void FPSCamera::Init()
{
	SetCursorPos(s_fixMousePos.x, s_fixMousePos.y);		// マウスを固定位置に移動
	m_degAngle.y = -90 ;	// magicnumber 
}

void FPSCamera::Update()
{
		POINT nowPos;									// 現在位置の入れ物
		GetCursorPos(&nowPos);							// 現在位置を入力
		POINT mouseMove;								// マウスの移動量の入れ物

		mouseMove.x = nowPos.x - s_fixMousePos.x;		// :X座標が中心からどれだけ動いたかを取得
		mouseMove.y = nowPos.y - s_fixMousePos.y;		// :Y座標が中心からどれだけ動いたかを取得

		SetCursorPos(s_fixMousePos.x, s_fixMousePos.y);	// カーソルを中心に戻す 計算終了時
		
		m_degAngle.x += (float)(mouseMove.y * 0.25);				// カメラ回転処理
		m_degAngle.y += (float)(mouseMove.x * 0.25);				// カメラ回転処理

	m_degAngle.x = std::clamp(m_degAngle.x, m_minAngleX, m_maxAngleX);// 回転制御
}

const DirectX::SimpleMath::Matrix FPSCamera::GetRotationMatrix()// 三次元での回転の計算で用いる四元数の取得
{
	return DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(	//三次元での回転の計算で用いられる四元数
	DirectX::XMConvertToRadians(m_degAngle.y),					// ヨー	:上下の軸まわりの回転がヨーイング
	DirectX::XMConvertToRadians(m_degAngle.x),					// ピッチ	:左右の軸まわりの回転がピッチング 
	DirectX::XMConvertToRadians(m_degAngle.z));					// ロール	:前後の軸まわりの回転がローリング
}

const DirectX::SimpleMath::Matrix FPSCamera::GetRotationYMatrix()	// プレイヤーに渡す情報。
{
	return Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_degAngle.y));
}

void FPSCamera::SetCameraMatrix(const DirectX::SimpleMath::Matrix& mCam)	// FPSカメラのカメラ行列を設定
{
	DirectX::SimpleMath::Matrix trans;
	trans = trans.CreateTranslation(m_localPos);

	DirectX::SimpleMath::Matrix rotation;
	rotation = GetRotationMatrix();

	KdCamera::SetCameraMatrix(rotation * trans * mCam);
}