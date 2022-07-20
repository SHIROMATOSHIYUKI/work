#include"EnemyBullet.h"
#include "../Effect2D/Effect2D.h"

void EnemyBullet::Init()
{
	m_modelWork.SetModel(GameResourceFactory.GetModelData("Data/Models/Bullet/Bullet.gltf"));
	m_lifeSpan = 200;
	m_bumpSphereInfo.m_radius = 0.25f;

	// ランダム性を出す
	float randX = (float)((rand() % 10 - 5) * 0.005);
	float randY = (float)((rand() % 10 - 5) * 0.005);
	m_goalDir.x += randX;
	m_goalDir.y += randY;
	m_goalDir.Normalize();
	m_goalDir *= 5.0f;
	m_trans = Math::Matrix::CreateTranslation(m_goalDir);			// Vector3移動量を行列に

	m_trail.SetTexture(GameResourceFactory.GetTexture("Data/Textures/Trail.png"));
	m_trail.SetWidth(0.05f);															// サイズ調整
	m_trail.SetPattern(KdTrailPolygon::Trail_Pattern::eBillboard);
}

void EnemyBullet::Update()
{
	if (--m_lifeSpan <= 0) { m_isAlive = false; }
	
	if (!m_isStabbed) 
	{
		m_prevPos = m_mWorld.Translation();			// 移動する前の地点を覚える(判定用)
		m_mWorld *= m_trans;
		UpdateCollision();
	}

	m_trail.AddPoint(m_mWorld);
	if (m_trail.GetNumPoints() > 10)
	{
		m_trail.DelPoint_Back();
	}
}

void EnemyBullet::DrawEffect()
{
	SHADER->m_effectShader.DrawTrailPolygon(m_trail);
}

// 当たり判定
void EnemyBullet::UpdateCollision()
{
	// 最も近くにあったオブジェクト
	std::shared_ptr<GameObject> closestObject = nullptr;
	// 最も近くにあったオブジェクトへの距離
	float closestLength = 10000;
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		if (spObject->GetClassID() == GameObject::ePlayer ||
			spObject->GetClassID() == GameObject::eStage)
		{
			Math::Vector3 rayPos = m_prevPos;									// 発射位置
			Math::Vector3 rayDir = GetPos() - m_prevPos;						// 発射方向(前の地点から今の地点にめっしゅがあったら	
			// 一回の移動量
			float length = rayDir.Length();										// レイの長さ
			RayInfo rayInfo(rayPos, rayDir, length);
			HitResult result;

			if (spObject->CheckCollisionRay(rayInfo, result))
			{
				// 発射位置からヒット位置までのベクトル
				Math::Vector3 nowVector = m_prevPos - result.m_HitPos;
				
				// closestObject(最も近いオブジェクト)の中身が入っているとき
				if (closestObject)
				{
					float nowLength = nowVector.Length();
					// 最も近かったオブジェクトと距離を比較、現在の方が近かった場合更新
					if (closestLength > nowLength)
					{
						closestObject = spObject;
						closestLength = nowVector.Length();
					}
				}
				else
				{
					closestObject = spObject;
					closestLength = nowVector.Length();
				}
			}
		}
	}
	
	// 保存していたオブジェクトにヒットしたと伝える
	if (closestObject)
	{
		m_isStabbed = true;
		closestObject->onhit();
	}
}