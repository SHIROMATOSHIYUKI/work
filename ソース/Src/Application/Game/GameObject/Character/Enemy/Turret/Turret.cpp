#include"Turret.h"
#include "../../../EnemyBullet/EnemyBullet.h"
#include"../../Player/Player.h"
#include "../../../Manager/EnemyManager/EnemyManager.h"
#include "../../../Effect2D/Effect2D.h"
#include"../../../../../Utility/DebugObject/DebugObject.h"

void Turret::ImGuiProcess()
{
	//Math::Vector3 turretPos = m_mWorld.Translation();
	//ImGui::Text("Position [x:%2f][y:%2f][z:%2f]\n", turretPos.x, turretPos.y, turretPos.z);
	ImGui::Text("m_headRot [x:%2f][y:%2f][z:%2f]\n", m_headRot.x, m_headRot.y, m_headRot.z);
}
void Turret::UpdateDebug()
{
	if (!m_spDebugObject) { return; }
	
	// マズルポイントから弾の飛ぶ方向に線
	//Math::Matrix fastPos = m_pMuzzlePoint->m_worldTransform * m_mWorld;

	//Math::Vector3 lastPos = m_pMuzzlePoint->m_worldTransform.Backward() * 30;
	//lastPos += fastPos.Translation();
	//m_spDebugObject->AddDebugLine(fastPos.Translation(), lastPos);

}
void Turret::Init()
{
	// Jsonファイルの読み込み
	SetJson("Data/Json/Character/Enemy/Turret.json");

	// テクスチャ、モデル、ノードの読み込み
	m_modelWork.SetModel(GameResourceFactory.GetModelData(JsonS("model","turret")));
	m_pMuzzlePoint	= m_modelWork.FindWorkNode("MuzzlePoint");
	m_pHead			= m_modelWork.FindWorkNode("Head");

	// 球情報のセット
	m_bumpSphereInfo.m_radius = JsonF("bumpSphere", "radius");
	m_bumpSphereInfo.m_pos.y = JsonF("bumpSphere", "incrPosY");

	fovSphereInfo.m_radius = JsonF("fovSphere","radius");
	fovSphereInfo.m_pos.y = JsonF("fovSphere", "incrPosY");

	// ステータス(Hp、座標)をセット
	m_Hp = JsonI("hp");
	m_fovAngle = JsonF("fovAngle");	// 視野判定の角度

	Math::Matrix trans;		// 移動
	trans = Math::Matrix::CreateTranslation(m_worldPos);
	m_mWorld = trans;
	m_reloadCount =JsonI("reloadTime");
	//　アクションを「探索」状態にする
	m_spActionState = std::make_shared<ActionSearch>();
	m_warningCount = JsonI("warningCount");
	// デバッグオブジェクトのインスタンス化
	m_spDebugObject = std::make_shared<DebugObject>();	
}

void Turret::Update()
{
	// Hpが0になったら死亡ステートに移行
	if (m_Hp <= 0)
	{
		m_spActionState = std::make_shared<ActionDeath>();
	}
	// m_wpTargetがnullならターゲットを取得
	if (m_wpTarget.expired())
	{
		m_wpTarget = GameInstance.WorkPlayer();
		m_spTarget = m_wpTarget.lock();
	}

	// 頭の回転
	if (m_pHead)
	{
		Math::Vector3 nodePos = m_pHead->m_localTransform.Translation();
		m_pHead->m_localTransform = Math::Matrix::CreateFromYawPitchRoll
		(DirectX::XMConvertToRadians(m_headRot.y),DirectX::XMConvertToRadians(m_headRot.x), 0);

		m_pHead->m_localTransform.Translation(nodePos);
	}
	m_modelWork.CalcNodeMatrices();

	// 視野判定
	FovCollision();
	// 角度と球、視界良好判定、どちらもhitの時にfovOllHit判定
	if ( (bool)(m_angleHit * m_sphereHit * m_visiblePlayerFlg) == true)
	{m_fovOllHit = true;}
	 else
	{ m_fovOllHit = false; }

	UpdateDebug();
	if (m_spActionState) { m_spActionState->Update(*this); }
}

void Turret::Draw2D()
{
	if (m_spActionState) { m_spActionState->Draw2D(*this); }
}


//=================
// ステートパターン
//=================

//=====
// 探索
//=====
int pastHp;
void Turret::ActionSearch::Update(Turret& owner)
{
	// ダメージを受けていたらHeadRotateで追跡
	if (pastHp > owner.m_Hp) { owner.HeadRotate(); }

	if (owner.m_fovOllHit)	// 発見したら
	{
		owner.m_spActionState = std::make_shared<ActionChase>();	// 追跡
		std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
		spSoundInstance = GameAudio.Play3D(owner.JsonS("sound","discovery"), owner.GetPos());
		spSoundInstance->SetVolume(owner.JsonF("volume","discovery"));
	}
	pastHp = owner.m_Hp;
}
//=====
// 追跡
//=====
void Turret::ActionChase::Update(Turret& owner)
{
	owner.HeadRotate();	// 追跡
	// プレイヤーの方を向いたら

	if ((owner.betweenAngY <= owner.JsonF("uprightUpperLimit")) && owner.betweenAngY >= owner.JsonF("uprightLowerLimit"))
	{
		owner.m_spActionState = std::make_shared<ActionWarning>();	// 警告に遷移
	}
	if (!owner.m_fovOllHit)	// 見失ったら
	{
		owner.m_spActionState = std::make_shared<ActionMiss>();		// 見失う
	}
}
//=====
// 警告
//=====
void Turret::ActionWarning::Update(Turret& owner)
{
	owner.HeadRotate();

	owner.m_warningCount--;	// 時間経過
	if (owner.m_warningCount <= 0)
	{
		owner.m_warningCount = owner.JsonI("warningCount");
		owner.m_spActionState = std::make_shared<ActionAttack>();	// 攻撃に遷移
	}
}
void Turret::ActionWarning::Draw2D(Turret& owner)
{
	std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();				// 爆発矢を作る
	spEffect->Init();
	spEffect->SetLifeSpan(owner.JsonI("warningDrawTime"));
	Math::Vector3 pos = owner.GetPos();
	pos.y += owner.JsonF("warningIncrPosY");
	spEffect->SetPos(pos);
	spEffect->SetTexture(GameResourceFactory.GetTexture(owner.JsonS("texture", "warning")));
	GameInstance.AddObject(spEffect);
}
//=====間隔
// 攻撃
//=====
void Turret::ActionAttack::Update(Turret& owner)
{
	owner.HeadRotate();
	--owner.m_shotCount;
	if (owner.m_shotCount <= 0)
	{
		owner.ShotBullet();
		// 射撃音
		std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
		spSoundInstance = GameAudio.Play3D(owner.JsonS("sound", "shoot"), owner.GetPos());
		spSoundInstance->SetVolume(owner.JsonF("volume","attack"));

		owner.m_shotCount = owner.JsonI("shotInterval");
		owner.m_shotBreakCount++;	// ブレイクカウントをプラス
	}

	if (owner.m_shotBreakCount >= owner.JsonI("numberOfBullets"))// 一度の攻撃で発砲する段数
	{
		owner.m_shotBreakCount = 0;
		owner.m_spActionState = std::make_shared<ActionReload>();
	}
}
//=======
// 再装填
//=======
void Turret::ActionReload::Update(Turret& owner)
{
	// リロード音の再生(1度だけ)
	if (owner.m_reloadSoundFlg)	
	{
		std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
		spSoundInstance = GameAudio.Play3D(owner.JsonS("sound", "reload"), owner.GetPos());
		spSoundInstance->SetVolume(owner.JsonF("volume","reload"));
		owner.m_reloadSoundFlg = false;
	}

	// 一定時間経過したら「再装填」から「警告」に戻る
	owner.m_reloadCount--;
	if (owner.m_reloadCount <= 0)
	{
		owner.m_reloadSoundFlg = true;
		owner.m_reloadCount = owner.JsonI("reloadTime");
		owner.m_spActionState = std::make_shared<ActionChase>();
	}
}
//=========
// 見失う
//=========
void Turret::ActionMiss::Update(Turret& owner)
{
	if (owner.m_fovOllHit)	// 発見したら
	{
		owner.m_spActionState = std::make_shared<ActionChase>();	// 追跡
		std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
		spSoundInstance = GameAudio.Play3D(owner.JsonS("sound", "discovery"), owner.GetPos());
		spSoundInstance->SetVolume(owner.JsonF("volume", "discovery"));
	}
}
void Turret::ActionMiss::Draw2D(Turret& owner)
{
	std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();				// 爆発矢を作る
	spEffect->Init();
	spEffect->SetLifeSpan(owner.JsonI("discoveryVolume"));
	Math::Vector3 pos = owner.GetPos();
	pos.y += owner.JsonF("discoveryIncrPosY");
	spEffect->SetPos(pos);
	spEffect->SetTexture(GameResourceFactory.GetTexture(owner.JsonS("texture","question")));
	GameInstance.AddObject(spEffect);
}
//=========
// 死亡
//=========

void Turret::ActionDeath::Update(Turret& owner)
{
	// ①爆発音と爆発テクスチャを一度だけ描画
	// ②頭を下に向かせる
	// ③煙を立たせる

	// ①爆発音とエネミーマネージャーで死んだことを伝える
	if (owner.DeathMoment)
	{

		std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
		spSoundInstance = GameAudio.Play3D(owner.JsonS("sound", "death"), owner.GetPos());
		spSoundInstance->SetVolume(owner.JsonF("volume", "death"));


		if(owner.m_clearEnemy)
		{ 
			std::weak_ptr<EnemyManager> wpEnemyManager = GameInstance.WorkEnemyManeger();
			std::shared_ptr<EnemyManager> spEnemyManager = wpEnemyManager.lock();
			spEnemyManager.get()->LastEnemyCountReduce();
		}

		owner.DeathMoment = false;
	}
	// ②頭を下に向かせる
	owner.m_headRot.x = owner.JsonF("deathHeadRotX");
	
	// ③煙を立たせる
	if (owner.SmokeCnt++ >= owner.JsonI("smokeInterval"))
	{
		float randX = (float)((rand() % owner.JsonI("smokeIncrPosMax") - owner.JsonI("smokeIncrPosMin")) * owner.JsonF("smokeIncrAdj"));
		float randZ = (float)((rand() % owner.JsonI("smokeIncrPosMax") - owner.JsonI("smokeIncrPosMin")) * owner.JsonF("smokeIncrAdj"));
		Math::Vector3 movePos = { randX,owner.JsonF("smokeIncrPosY"),randZ };
		std::shared_ptr<Effect2D> spEffect = std::make_shared<Effect2D>();
		spEffect->SetAnimation(owner.JsonI("smokeAnimationsplitX"), owner.JsonI("smokeAnimationsplitY"), owner.JsonF("smokeAnimationSpeed"), false);
		spEffect->SetPos(owner.GetPos());
		spEffect->SetLifeSpan(owner.JsonI("somkeLifeSpan"));
		spEffect->SetMove(movePos);
		spEffect->SetSize(owner.JsonV3("smokeIncrSize"));
		spEffect->SetTexture(GameResourceFactory.GetTexture(owner.JsonS("texture","explosion")), 0.0f, 7.0f, 7.0f);
		GameInstance.AddObject(spEffect);
		owner.SmokeCnt = 0;
	}

	//DeathFlgを立て、死亡したことを伝える
	owner.DeathFlg = true;
}

//===============================
// ステートパターンの中で使う関数
//===============================

//=========
// 視野判定
//=========
// 視野判定統合
void Turret::FovCollision()
{
	FovAngle();
	FovSphere();
	FovObject();
}
// 視野角度判定
void Turret::FovAngle()
{
	// 自分の座標、自分から敵の方向、判定距離の作成
	// 自分の向いている方向ベクトル
	Math::Vector3 nowDir = m_pMuzzlePoint->m_worldTransform.Backward();
	// ターゲットに向かう方向ベクトル
	Math::Vector3 targetDir = GetPos() - m_spTarget->GetPos();
	// 2つのベクトルを正規化
	nowDir.Normalize();
	targetDir.Normalize();

	//============================
	// 2つの方向の間の角度を求める
	//============================

	// ①2辺の「内積」
	float dot = targetDir.Dot(nowDir);
	dot = std::clamp(dot, JsonF("dotClampMin"), JsonF("dotClampMax"));	// dotを-1.0f～1.0fの値になるようにする

	// ②2つに①をacos(逆余弦)して2辺の角度を求める
	float betweenRadian = acos(dot);
	m_betweenDegree = DirectX::XMConvertToDegrees(betweenRadian);

	//==========================================
	//ターゲットとの角度次第で判定する
	//==========================================
	if (m_betweenDegree >= m_fovAngle)
	{
		m_angleHit = true;
	}
	else
	{
		m_angleHit = false;
	}
}
// 視野球判定
void Turret::FovSphere()
{
	if (m_wpTarget.expired()) { return; }	// ターゲットがいなかったら実行しない

	// ①タレットとプレイヤーの距離と互いの円判定の距離を比較
	Math::Vector3 selfPos	= GetPos() + fovSphereInfo.m_pos;
	Math::Vector3 targetPos = m_spTarget->GetPos();
	
	// 2点間の距離を求める
	Math::Vector3 betweenVec = selfPos - targetPos;
	float distanceSqr = betweenVec.LengthSquared();
	
	// 判定結果を入れる
	if (distanceSqr <= (fovSphereInfo.m_radius * fovSphereInfo.m_radius))
	{
		m_sphereHit = true;
	}
	else
	{
		m_sphereHit = false;
	}
}
// プレイヤーとの間にオブジェクトがあるかどうか
void Turret::FovObject()
{
	// ①タレットからプレイヤーの方向を作成
	// ②タレットからプレイヤーまでの長さを作成
	// ③座標、①、②　を引数にレイ情報を作成
	// ④メンバーがステージマップだった時にレイ判定呼び出し③を引数

	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())			// 地面(StageMap)の検索
	{
		if (spObject->GetClassID() != GameObject::eStage) { continue; }	// ステージオブジェクトじゃなかったら処理を飛ばす
		Math::Vector3 targetDir = m_spTarget->GetPos()- GetPos();
		float targetLength = targetDir.Length();
		targetDir.Normalize();
		Math::Vector3 rayPos = GetPos();
		rayPos.y += JsonF("rayIncrPosY");
		RayInfo fovRayInfo(rayPos, targetDir, targetLength);
		bool result = false;
		spObject->CheckCollisionRay(fovRayInfo, result);		// 相手に当たり判定を呼ばせる

		if(result){ m_visiblePlayerFlg = false;}	// 当たっていたら見えていない
		else{ m_visiblePlayerFlg = true;}			// 当たっていなければ見えている
	}
}

// 敵の方向へ向かうための角度計算
void Turret::HeadRotate()
{
	if (m_wpTarget.expired()) { return; }	// ターゲットがいなかったら実行しない
	if (!m_pHead) { return; }
	
	Math::Vector3 nowDir = m_pHead->m_localTransform.Backward();
	Math::Vector3 targetDirY = m_spTarget->GetPos() - m_worldPos;	// 敵へ向かう方向ベクトル
	Math::Vector3 targetDirX = m_spTarget->GetPos() - m_worldPos;	// 敵へ向かう方向ベクトル
	nowDir.Normalize();		// 正規化(角度のみを取得)
	targetDirY.Normalize();	// 正規化(角度のみを取得)
	targetDirX.Normalize();
	float betweenAngY = GetBetweenAngY(nowDir, targetDirY);
	float betweenAngX = GetBetweenAngX(nowDir, targetDirX);

	float rotateAngY = std::clamp(betweenAngY, JsonF("rotateAngYClampMin"), JsonF("rotateAngYClampMax"));	// -2から2の中で制限される
	float rotateAngX = std::clamp(betweenAngX, JsonF("rotateAngYClampMin"), JsonF("rotateAngYClampMax"));	//!!!マジックナンバー

	m_headRot.y += rotateAngY;
	m_headRot.x += rotateAngX;
}
float Turret::GetBetweenAngY(const Math::Vector3& dir1, const Math::Vector3& dir2)
{
	// 今向いている方向のワールド角度を求める
	float nowAngY = atan2(dir1.x, dir1.z);			// 真上からみて2Dで角度を取得
	nowAngY = DirectX::XMConvertToDegrees(nowAngY);		// デグリー変換
	// ターゲットへ向くワールド角度を求める
	float targetAngY = atan2(dir2.x, dir2.z);
	targetAngY = DirectX::XMConvertToDegrees(targetAngY);	// デグリー変換

	betweenAngY = targetAngY - nowAngY;	// 2間の角度を求める

	if (betweenAngY > 180) { betweenAngY -= 360; }	// 角度を-180~180で保つ//!!!マジックナンバー
	else if (betweenAngY < -180) { betweenAngY += 360; }	

	return betweenAngY;
}

float Turret::GetBetweenAngX(const Math::Vector3& dir1, const Math::Vector3& dir2)
{
	// 今向いている方向のワールド角度を求める
	float nowAngX = atan2(1.0f, dir1.y); // 真上からみて2Dで角度を取得
	nowAngX = DirectX::XMConvertToDegrees(nowAngX); // デグリー変換

	// ターゲットへ向くワールド角度を求める
	float targetAngX = atan2(1.0f, dir2.y);
	targetAngX = DirectX::XMConvertToDegrees(targetAngX);	// デグリー変換

	float betweenAngX = targetAngX - nowAngX;	// 2間の角度を求める

	return betweenAngX;
}
// 弾発射
void Turret::ShotBullet()
{
	if (!m_pMuzzlePoint) { return; }

	std::shared_ptr<EnemyBullet> spBullet = std::make_shared<EnemyBullet>();
	spBullet->SetDir(m_pMuzzlePoint->m_worldTransform.Backward());
	spBullet->Init();
	spBullet->SetWorldMatrix(m_pMuzzlePoint->m_worldTransform * m_mWorld);
	GameSystem::GetInstance().AddObject(spBullet);
}
// 球VSメッシュ
//bool Turret::CheckCollisionDamegeSphere(const SphereInfo& info, DamegeResult& result)
//{
//	for (UINT i = 0; i < m_modelWork.GetDataNodes().size(); ++i)	// StageMapのメッシュ達戸当たり判定
//	{
//		const KdModelData::Node dataNode = m_modelWork.GetDataNodes()[i];// 該当のノードを取得
//		if (!dataNode.m_spMesh) { continue; }	// メッシュでなかったら判定しない
//		// ノードの現在の位置や回転情報					↓変更可能データ
//		const KdModelWork::Node workNode = m_modelWork.GetNodes()[i];
//
//		// 最終的にすべての当たった面から差し戻されるベクトル
//		// 押し戻された後の座標
//		Math::Vector3 localPushedPos = {};
//
//		if (KdSphereToMesh(info.m_pos, info.m_radius,
//			*(dataNode.m_spMesh.get()), workNode.m_worldTransform * m_mWorld, localPushedPos))
//		{
//			m_Hp -= 1;
//			result.m_isHit = true;
//		}
//	}
//
//	return result.m_isHit;
//}