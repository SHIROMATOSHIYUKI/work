#include "Gun.h"
#include "../../GameSystem.h"
#include"../Character/Player/Player.h"
#include "../Bullet/Bullet.h"
void Gun::Init()
{
	SetJson("Data/Json/Gun.json");
	m_modelWork.SetModel(GameResourceFactory.GetModelData(JsonS("model","gun")));	// 銃モデルの設定
	m_crossHairTex = GameResourceFactory.GetTexture(JsonS("texture", "crossHair"));		// クロスヘア画像の設定
	m_crossHairPos = JsonV2("crossHairPos");															// クロスヘアの座標セット
}

void Gun::Update()
{
	// レイ判定で弾丸の目的座標を取得
	SetBulletGoal();
	if (!m_canFiring)
	{
		m_canShotCount--;
		if (m_canShotCount <= 0)
		{
			m_canFiring = true;
		}
	}
}

void Gun::Draw2D()
{
	if (!m_crossHairTex) { return; }															// もしクロスヘア画像がnullだったら関数を終了
	std::shared_ptr<const KdCamera> spCamera = m_wpCamera.lock();								// KdCamera型の入れ物に継承先のFPSカメラ

	SHADER->m_spriteShader.DrawTex(m_crossHairTex.get(), (int)m_crossHairPos.x, (int)m_crossHairPos.y );	// 座標をセットして描画
}

void Gun::ShotBullet()
{
	if (m_canFiring)
	{
		std::shared_ptr<Bullet> spBullet = std::make_shared<Bullet>();			// 弾作成
		spBullet->SetRayHit(rayHit);
		spBullet->SetDir(m_mWorld.Translation(), m_rayHitPos, m_mWorld.Backward());

		spBullet->Init();

		const KdModelWork::Node* pNode = m_modelWork.FindNode("MuzzlePoint");	// モデルからノードを探す

		if (pNode)
		{
			spBullet->SetWorldMatrix(pNode->m_worldTransform * m_mWorld);		// マズルノードの行列と自分の行列を掛け合わせ対置から弾を発射
		}

		GameSystem::GetInstance().AddObject(spBullet);							// リストに追加
		m_canFiring = false;														// 一旦うてなくする
		m_canShotCount = JsonI("shotInterval");
		// サウンド
		std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
		spSoundInstance = GameAudio.Play3D(JsonS("sound","shoot"), GetPos());
		spSoundInstance->SetVolume(JsonF("volume","shoot"));
	}
}

void Gun::SetBulletGoal()		// レイ判定で弾を飛ばす座標を取得
{
	std::shared_ptr<KdCamera> spCamera = m_wpCamera.lock();										// KdCamera型の入れ物に継承先のFPSカメラ
	Math::Matrix cameraMatrix = spCamera->GetCameraMatrix();									// カメラ行列を取得
	// クロスヘアの2D座標を3D座標に変換
	Math::Vector3 worldCrossHairPos;															// クロスヘアの3D座標
	spCamera->ConvertScreenToWorldDetail(m_crossHairPos, worldCrossHairPos);
	// クロスヘア3D座標への方向を取得
	Math::Vector3 bulletAdvanceDir;																// カメラからクロスヘア3D座標の方向
	bulletAdvanceDir = worldCrossHairPos - spCamera->GetCameraMatrix().Translation();			// クロスヘアからカメラ座標までのベクトル取得
	bulletAdvanceDir.Normalize();																// 正規化で合計の値を1にして方向だけ取得　
	// オブジェクトノードを探し、レイ判定を行う
	const std::list<std::shared_ptr<GameObject>>& objects = GameSystem::GetInstance().GetObjects();		// ゲームobjectの住所一覧
	for (const std::shared_ptr<GameObject>& spObject : objects)											// ゲームobjectの数だけfor文を回る
	{
		if (spObject->GetClassID() != GameObject::eEnemy &&
			spObject->GetClassID() != GameObject::eStage) { continue; }
		RayInfo rayInfo(cameraMatrix.Translation(), bulletAdvanceDir, 1000.0f);							// レイの発射位置、方向、長さ
		BumpResult bumpResult;

		spObject->CheckCollisionRay(rayInfo, bumpResult);	// あたり判定呼び出し

		if (bumpResult.m_isHit)
		{
			m_rayHitPos = bumpResult.m_HitPos;				//　レイが当たった座標を渡す
			rayHit = true;
		}
		else
		{
			rayHit = false;
		}
	}
}