#pragma once

class FPSCamera : public KdCamera
{
public:
	FPSCamera() {}
	~FPSCamera() {}
	static const POINT s_fixMousePos;// マウスカーソルを留める画面中央の座標を持った位置
	
	void Init();
	virtual void Update();

	const DirectX::SimpleMath::Matrix GetRotationMatrix();
	const DirectX::SimpleMath::Matrix GetRotationYMatrix();
	const Math::Vector3 GetPos()const {return m_localPos;}
	const Math::Vector3& GetRotationAngles() const { return m_degAngle; }
	const float GetRotationYAngle() const { return 0; }
	// 視線用のローカル座標の指定
	void SetLocalPos(DirectX::SimpleMath::Vector3& pos) { m_localPos = pos; }
	// SetCameraMatrix関数をFPSCameraで上書き(厳密にはちがう)
	virtual void SetCameraMatrix(const DirectX::SimpleMath::Matrix& mCam) override ;
	// カメラのX軸の制限をかける関数
	void SetClampAngleX(float minAng, float maxAng){ m_minAngleX = minAng; m_maxAngleX = maxAng; }
protected:
	// 視線用のローカル座標
	//(キャラクターの原点から指定の位置へ移動する座標ベクトル)
	DirectX::SimpleMath::Vector3 m_localPos;
	// 各軸の回転情報
	DirectX::SimpleMath::Vector3 m_degAngle;

	float m_minAngleX = -FLT_MAX; // 上向きの制限
	float m_maxAngleX = FLT_MAX; // 下向きの制限
};