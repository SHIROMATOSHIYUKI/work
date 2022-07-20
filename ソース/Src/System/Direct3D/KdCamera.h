#pragma once
class KdCamera
{
public:
	KdCamera() {}
	~KdCamera() {}	

	void SetProjectionMatrix(float fov, float maxRange = 2000, float minRange = 0.01f, float aspectRatio = D3D.WorkBackBuffer()->GetAspectRatio());
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj) { m_mProj = rProj; }

	inline const DirectX::SimpleMath::Matrix& GetCameraMatrix() const { return m_mCam; }	// カメラ行列取得
	inline const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return m_mView; }		// ビュー行列取得
	inline const DirectX::SimpleMath::Matrix& GetProjMatrix() const { return m_mProj; }		// 射影行列取得
	virtual void SetCameraMatrix(const DirectX::SimpleMath::Matrix& mCam);					// カメラ行列・ビュー行列セット
	void SetToShader() const;																// カメラ情報(ビュー・射影行列など)をシェーダへセット 

	void ConvertScreenToWorldDetail(const Math::Vector2& pos, Math::Vector3& result);		// スクリーン座標(2D座標)->ワールド座標(3D座標)に変換する

	const	DirectX::SimpleMath::Matrix& GetCamera()	{ return m_mCam; }					// いじれないカメラ
			DirectX::SimpleMath::Matrix& WorkCamera()	{ return m_mCam; }					// いじれるカメラ

protected:
	DirectX::SimpleMath::Matrix	m_mCam;		// カメラ行列
	DirectX::SimpleMath::Matrix	m_mView;	// ビュー行列	カメラの位置と方向
	DirectX::SimpleMath::Matrix	m_mProj;	// 射影行列
};
