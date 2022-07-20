#include "Bullet.h"
#include "Application/Game/GameSystem.h"
#include "../Effect2D/Effect2D.h"
#include "../Character/Character.h"

void Bullet::Init()
{
	m_modelWork.SetModel(GameResourceFactory.GetModelData("Data/Models/Bullet/Bullet.gltf"));
	m_lifeSpan = 2000;
	m_damegeSphereInfo.m_radius = 0.25f;

	// レイ判定がヒットしてるか確認。
	if (rayHit) { m_goalDir = m_goalPos - m_gunPos; }	// ture :ゴールから発射位置のベクトルを入力
	float randX = (float)((rand() % 10 - 5) * 0.002);
	float randY = (float)((rand() % 10 - 5) * 0.002);
	m_goalDir.x += randX;
	m_goalDir.y += randY;
	m_goalDir.Normalize();
	m_goalDir *= 1;
	m_trans = Math::Matrix::CreateTranslation(m_goalDir);			// Vector3移動量を行列に

	m_trail.SetTexture(GameResourceFactory.GetTexture("Data/Textures/Trail.png"));	
	m_trail.SetWidth((float)0.1);														// サイズ調整
	m_trail.SetPattern(KdTrailPolygon::Trail_Pattern::eBillboard);					// 常にカメラの方向を自動で向く
}

void Bullet::Update()
{
	if (--m_lifeSpan <= 0) { m_isAlive = false; }

	if (!m_isStabbed)
	{
		m_prevPos = m_mWorld.Translation();			// 移動する前の地点を覚える(判定用)
		m_mWorld *= m_trans;
		UpdateCollision();							// 当たり判定。座標更新すべて終わってから
	}
	
	m_trail.AddPoint(m_mWorld);
	if (m_trail.GetNumPoints() > 60)
	{m_trail.DelPoint_Back();}
}

void Bullet::DrawEffect()
{
	SHADER->m_effectShader.DrawTrailPolygon(m_trail);
}

void Bullet::UpdateCollision()
{
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() != GameObject::eEnemy) { continue; }

		// レイとダメージの設定
		Math::Vector3 rayPos = m_prevPos;				// レイ開始位置
		Math::Vector3 rayDir = GetPos() - m_prevPos;	// レイ
		// 一回の移動量
		float length = rayDir.Length();					// レイの長さ

		RayInfo rayInfo(rayPos, rayDir, length);
		DamegeResult damegeResult;
		damegeResult.m_Damege = 1;

		// 設定したレイとダメージを引数にレイ判定を呼び出す
		bool result = spObject->CheckCollisionDamegeRay(rayInfo, damegeResult);
			
		if (result)
		{
			std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
			spSoundInstance = GameAudio.Play3D("Data/Audio/impact.wav", GetPos());
			spSoundInstance->SetVolume(10);
			m_isAlive = false;																// フラグを下げる
			// 爆発の呼び出し
			//std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();				// 爆発矢を作る
			//spEffect->Init();
			//spEffect->SetAnimation(5, 5);
			//spEffect->SetPos(m_prevPos);
			//spEffect->SetTexture(GameResourceFactory.GetTexture("Data/Textures/Explosion.png"));
			//GameInstance.AddObject(spEffect);											// 管理できるところに入れておく
		}	
	}

	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())				//  objectリストを全部回す
	{
		if (spObject->GetClassID() != GameObject::eStage) { continue; }		// 地面との判定

		// レイを使って地面との当たり判定
		Math::Vector3 rayPos = m_prevPos;									// 発射位置
		Math::Vector3 rayDir = GetPos() - m_prevPos;						// 発射方向(前の地点から今の地点にめっしゅがあったら	
		// 一回の移動量
		float length = rayDir.Length();										// レイの長さ

		// その引数を持つコンストラクタが通る
		RayInfo rayInfo(rayPos, rayDir, length);
		BumpResult result;

		if (spObject->CheckCollisionRay(rayInfo,result))
		{
			m_isStabbed = true;												// 消す
						// 爆発の呼び出し
			//std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();	// 爆発矢を作る
			//spEffect->Init();
			//spEffect->SetAnimation(5, 5);
			//spEffect->SetPos(GetPos());
			//spEffect->SetTexture(GameResourceFactory.GetTexture("Data/Textures/Explosion.png"));
			//GameInstance.AddObject(spEffect);						// 管理できるところに入れておく
		}
	}
}