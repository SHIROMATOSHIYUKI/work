#pragma once

#include "../GameObject.h"
class FPSCamera;	

class Gun : public GameObject
{
public:
	Gun(){}
	~Gun() {}

	void SetCamera(std::weak_ptr<KdCamera> pCamera){ m_wpCamera = pCamera; }

	void Init()		override;					// 初期化
	void Update()	override;					// 更新
	void Draw2D()	override;					// クロスヘア描画

	const Math::Vector3 GetRayHitPos() const { return m_rayHitPos; }
	void SetWorld(Math::Matrix pos) { m_mWorld = pos; }
	void ShotBullet();							// 弾の発射判定
private:
	// 弾発射
	bool			m_canFiring = true;			// 弾が発射できるかどうか
	int				m_canShotCount = 0;

	// 弾を飛ばす座標を取得
	void SetBulletGoal();						// 弾のゴールを設定
	float			m_hitDistance = 0.0f;		// レイがHITした際の本体との距離
	Math::Vector3	m_rayHitPos;				// レイが当たった座標
	Math::Vector3	m_rayDir;					// レイ座標に向かう角度
	bool rayHit = false;

	std::weak_ptr<KdCamera>		m_wpCamera;		// FPSCameraの中のKdCameraの部分を受けとる
	std::shared_ptr<KdTexture>	m_crossHairTex;	// スコープ用テクスチャ
	Math::Vector2				m_crossHairPos;	// クロスヘアの座標
};