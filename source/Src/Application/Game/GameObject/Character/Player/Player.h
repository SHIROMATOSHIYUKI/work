#pragma once
#include"../Character.h"

class FPSCamera;
class DebugCamera;
class Gun;

class Player : public Character
{
public:
	Player() {}						// コンストラクタ
	~Player() { Release(); }		// デストラクタ

	void Init() override;			// 初期化
	void Update() override;			// 更新
	void Draw2D() override;			// 2D描画
	void ImGuiProcess() override;	// デバッグウインドウ

	void UpdateCIE();	// 座標(coordinates)更新
	void UpdateCamera();// カメラ更新
	void UpdateGun();	// 銃更新
	void UpdateDebug();	// デバック更新

	std::weak_ptr<const GameObject>	m_wpTarget;	// 追いかけるターゲット
	// 場所の取得 overridしてem_worldPosを渡す
	const Math::Vector3 GetPos() const override { return m_worldPos; }	
	// 球情報の取得
	const SphereInfo GetSphereInfo() const { return m_bumpSphereInfo; }	
	// 「プレイヤー」を示すクラスIDを返す
	const GameObject::ClassID GetClassID() const override{return GameObject::ePlayer;}

	const std::shared_ptr<FPSCamera> GetCamera() const { return m_spCamera; }
	std::shared_ptr<FPSCamera> WorkCamera() { return m_spCamera; }
	
	// 死んだかどうか
	const bool DeathFlg() { return m_deathFlg; }
	
	// 座標のセット
	void SetPos(const Math::Vector3& pos) override
	{
		m_worldPos = pos;
		m_prevPos = pos;
	}

	//===============
	// 敵の弾との判定
	//===============
	// 打たれた判定　レイ対球
	//bool CheckCollisionDamegeSphere(const RayInfo& info, DamegeResult& result) override;
	bool CheckCollisionRay(const RayInfo& info, HitResult& result) override;
	void onhit() override;

	DirectX::BoundingSphere damageSphere;	// ダメージ判定用球情報

private:

	std::shared_ptr<FPSCamera>		m_spCamera = nullptr;	
	std::shared_ptr<DebugCamera>	m_debugCamera = nullptr;	

	std::shared_ptr<Gun>			m_spGun = nullptr;

	void Release();										// 解放

	void UpdateMove(Math::Vector3& dstMove);			// 移動
	void UpdateCollision();								// 当たり判定統合

	bool m_canJump = true;		// ジャンプできるかどうか
	Math::Vector3 m_worldPos;	// ワールド行列を構成する座標ベクトル
	Math::Vector3 m_worldRot;	// ワールド行列を構成する回転ベクトル
	Math::Vector3 m_prevPos;	// 1f前の座標

	float	m_verticalMovement = 0.0f;	// 垂直方向の移動量 対義語は(水平:Horizontal)

	void GunInit();		// 銃のセット
	Math::Matrix m_gunPos;// 銃の座標
	//=============
	// ダメージ表現
	//=============
	void DamageRep();	// 表現(representation)の略
	std::shared_ptr<KdTexture>	m_damageTex;	// ダメージ用テクスチャ
	Math::Color m_damageColor = { 1,1,1,m_damageAlpha };
	float m_damageAlpha = 0;

	//=======
	// HP表現
	//=======
	void HpRep();	// 表現(representation)の略
	// 瀕死時の枠画像
	std::shared_ptr<KdTexture>	m_dyingFrameTex;
	Math::Color m_dyingFrameColor = { 1,1,1,m_dyingFrameAlpha };
	float m_dyingFrameAlpha = 0;
	float m_dyingFrameProg = 0;
	// 瀕死時に全体にかかる画像
	std::shared_ptr<KdTexture>  m_dyingWholeTex;		
	Math::Color m_dyingWholeColor = { 1,1,1,m_dyingWholeAlpha };
	float m_dyingWholeAlpha = 0;
	
	//=================
	// 死んだときの動き
	//=================
	void DeathMovement();
	// カメラの値
	float m_deathCameraX;
	float m_deathCameraY;
	float m_deathCameraZ;
	// カメラの動き
	float m_deathCameraMoveX;
	float m_deathCameraMoveY;
	float m_deathCameraMoveZ;
	// カメラの変化量
	float m_deathCameraProgX = 0;
	float m_deathCameraProgY = 0;
	float m_deathCameraProgZ = 0;
	// 死亡フラグ
	bool m_deathFlg = false;
	// 死んだ瞬間
	bool m_deathMomentFlg = true;
};