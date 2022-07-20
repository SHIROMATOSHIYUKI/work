#include "KdCamera.h"

void KdCamera::SetProjectionMatrix(float fov, float maxRange, float minRange, float aspectRatio)	// 射影行列の設定
{
	m_mProj = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(fov), aspectRatio, minRange, maxRange);
}
void KdCamera::SetCameraMatrix(const DirectX::SimpleMath::Matrix& mCam)								// カメラ行列・ビュー行列セット
{
	m_mCam = mCam;				// カメラ行列セット
	m_mView = m_mCam.Invert();	// カメラ行列からビュー行列を算出
}
void KdCamera::ConvertScreenToWorldDetail(const Math::Vector2& pos, Math::Vector3& result)			// スクリーン座標(2D座標)->ワールド座標(3D座標)に変換する
{
	Math::Viewport vp;												// ビューポートを取得する
	D3D.GetViewport(vp);

	Math::Matrix trans = Math::Matrix::CreateTranslation(			// 射影座標系に変換
		pos.x / (vp.width * 0.5f),pos.y / (vp.height * 0.5f), 0);	

	Math::Matrix convertMat =										// 2D->3D変換用行列
		trans * GetProjMatrix().Invert() * GetViewMatrix().Invert();	

	convertMat._41 /= convertMat._44;								// x奥行情報を元に戻す
	convertMat._42 /= convertMat._44;								// y奥行情報を元に戻す
	convertMat._43 /= convertMat._44;								// z奥行情報を元に戻す

	result = convertMat.Translation();
}
void KdCamera::SetToShader() const																	// カメラ情報(ビュー・射影行列など)をシェーダへセット
{
	SHADER->m_cb7_Camera.Work().CamPos = m_mCam.Translation();	// カメラ座標をセット	
	SHADER->m_cb7_Camera.Work().mView = m_mView;				// ビュー行列をセット
	SHADER->m_cb7_Camera.Work().mProj = m_mProj;				// 射影行列をセット
	SHADER->m_cb7_Camera.Write();								// カメラ情報(ビュー行列、射影行列)を、シェーダの定数バッファへ書き込む
}