#include "Player.h"
#include "../../Gun/Gun.h"
#include "../../Effect2D/Effect2D.h"
#include"../../Manager/EnemyManager/EnemyManager.h"

#include"../../../../Utility/Easing/Easing.h"
#include"../../../../Utility/DebugObject/DebugObject.h"
#include "../../../GameSystem.h"
#include "../../../Camera/DebugCamera.h"

void Player::ImGuiProcess()
{
	ImGui::Text("Hp ; %d\n", m_Hp);
}

void Player::Init()
{
	// Jsonファイルの読み込み
	SetJson("Data/Json/Character/Player.json");
	// テクスチャセット/
	m_damageTex		= GameResourceFactory.GetTexture(JsonS("texture","damage"));	// ダメージ画像の設定
	m_dyingWholeTex = GameResourceFactory.GetTexture(JsonS("texture", "damage"));	// 瀕死時画像の設定
	m_dyingFrameTex = GameResourceFactory.GetTexture(JsonS("texture", "dyingFrame"));// ダメージ画像の設定

	// カメラの作成と情報を入力
	m_spCamera = std::make_shared<FPSCamera>();
	GameSystem::GetInstance().SetCamera( m_spCamera );			// GameSystemにもカメラをシェアさせる
	m_spCamera->Init();											// カメラ初期化
	m_spCamera->SetClampAngleX(JsonF("camera", "minAng"), JsonF("camera", "maxAng"));					// 上下カメラの可動域制限値
	m_spCamera->SetProjectionMatrix(JsonF("camera", "viewRange"));						// 視界の設定
	m_spCamera->SetLocalPos({JsonV3("camera","pos") });	// 顔の位置にカメラをセット
	
	// 体力
	m_Hp = JsonI("hp");
	// 座標
	SetPos({JsonV3("pos",JsonS("posName"))});
	// 球情報:衝突用
	m_bumpSphereInfo.m_radius = JsonF("bumpSphere", "radius");
	m_bumpSphereInfo.m_pos.y = JsonF("bumpSphere", "incrPosY");
	// 球情報:ダメージ用
	damageSphere.Radius = JsonF("damageSphere", "radius");
	
	// 銃関連
	GunInit();
	m_gunPos.Translation({JsonV3("gunPos")});
	
	// デバッグオブジェクトのインスタンス化
	m_spDebugObject = std::make_shared<DebugObject>();			
}

void Player::Update()
{
	// 体力が0になったら死亡フラグを立てる
	if (m_Hp <= 0){m_deathFlg = true;}
	if (m_deathFlg){DeathMovement();}
	// 重力
	m_verticalMovement += JsonF("gravity");
	// 座標更新
	if (!m_deathFlg) { UpdateCIE(); }
	// ダメージ表現処理処理
	DamageRep();
	// HP表現処理。clearした時は処理を行わない
	HpRep();

	// カメラ更新
	UpdateCamera();
	// 銃更新
	UpdateGun();
	// ダメージ用球判定の更新
	damageSphere.Center = GetPos();
	damageSphere.Center.y += JsonF("damageSphere", "incrPosY");
	
	// デバッグ更新
	UpdateDebug();
}

void Player::Draw2D()
{	
	// クリア時は処理を通らないようにする
	if (GameEnemyManeger.get()->lastEnemyIsAlive()){return;}
	Math::Rectangle rect = JsonR("fullScreenRect");
	SHADER->m_spriteShader.DrawTex(m_damageTex.get(), 0, 0, &rect, &m_damageColor);
	SHADER->m_spriteShader.DrawTex(m_dyingFrameTex.get(), 0, 0, &rect, &m_dyingFrameColor);
	SHADER->m_spriteShader.DrawTex(m_dyingWholeTex.get(), 0, 0, &rect, &m_dyingWholeColor);
}

void Player::Release()
{
	m_spCamera.reset(); // 参照カウンタ減らす
}
void Player::UpdateCIE()
{
	m_prevPos = GetPos();			// 移動する前の位置を覚える
	Math::Vector3 vMove;			// 移動ベクトル
	UpdateMove(vMove);				// 移動の更新
	m_worldPos.y -= m_verticalMovement;	// 重力
	UpdateCollision();					// 当たり判定
}

void Player::UpdateCamera()
{
	if (!m_spCamera) { return; }
	m_spCamera->Update();
	Math::Matrix trans = Math::Matrix::CreateTranslation(m_worldPos);
	// キャラクターから見たカメラの位置にプレイヤーの位置を合成したもの
	m_spCamera->SetCameraMatrix(trans);
}

void Player::UpdateGun()
{
	// 銃の移動
	Math::Matrix cameraMatrix = m_spCamera->GetCameraMatrix();
	m_spGun->SetWorld(m_gunPos * cameraMatrix);
	// 銃の発射
	if (GameInput.GetKeyState("Firing") & eKeyHold)
	{
		m_spGun->ShotBullet();
	}
}

void Player::UpdateDebug()
{
	if (!m_spDebugObject) { return; }
	//// debugLine：銃のマズルとレイ判定Hit座標
	//Math::Vector3 Camera = m_spCamera->GetCameraMatrix().Translation();
	//Math::Vector3 RayHit = m_spGun->GetRayHitPos();
	//const KdModelWork::Node* pNode = m_spGun->GetModel().FindNode("MuzzlePoint");
	//if (pNode)
	//{
	//	Math::Matrix muzzleMatrix = m_spGun->GetWorldMatrix();
	//	m_spDebugObject->AddDebugLine(muzzleMatrix.Translation(), RayHit);
	//}

	//m_spDebugObject->AddDebugSphere(damageSphere.Center, damageSphere.Radius);
	//m_spDebugObject->AddDebugSphere(GetPos() + m_bumpSphereInfo.m_pos , m_bumpSphereInfo.m_radius);
}

// エネミーバレットで呼び出す用関数
void Player::onhit()
{
	m_Hp -= m_json["enemyBulletDamage"].int_value();
	// 半透明に真っ赤な画像を画面全体に描画
	m_damageAlpha = JsonF("damageAlpha");
}

void Player::UpdateMove(Math::Vector3& dstMove)
{
	float moveSpd = JsonF("moveSpeed");
	DirectX::SimpleMath::Vector3 moveVec;

	const Math::Vector2& inputAxis = GameInput.GetAxis("Move");

	moveVec.x = inputAxis.x;
	moveVec.z = inputAxis.y;	// zだけど2なのでy表記

	moveVec.Normalize();	// 正規化：方向を保った状態で長さを１にする
	moveVec *= moveSpd;	

	if (m_spCamera)// カメラの回転を加味してベクトルを変換する
	{
		moveVec = moveVec.TransformNormal(moveVec, m_spCamera->GetRotationYMatrix());
	}
	m_worldPos += moveVec;// 座標の更新
	dstMove = moveVec;
}

void Player::UpdateCollision()
{
	// ①for文でゲームオブジェクトリストをすべて回す
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())
	{
		// ②設定されたクラスIDがEnemyだった場合判定を行う
		if (spObject->GetClassID() == GameObject::eEnemy)
		{
			SphereInfo sphereInfo(GetPos() + m_bumpSphereInfo.m_pos, m_bumpSphereInfo.m_radius);
			BumpResult bumpResult;

			// 自分と当たる対象に呼び出してもらう
			bool result = spObject->CheckCollisionBump(
				sphereInfo,
				bumpResult);
			if (result)
			{
				// 押し戻す処理
				m_worldPos += bumpResult.m_pushVec;
			}
		}
		else
		{		// ステージマップ横方向の判定
			if (spObject->GetClassID() != GameObject::eStage) { continue; }

			SphereInfo sphereInfo(GetPos() + m_bumpSphereInfo.m_pos, m_bumpSphereInfo.m_radius);
			BumpResult bumpResult;

			// 自分と当たる対象に呼び出してもらう
			bool result = spObject->CheckCollisionBump(
				sphereInfo,
				bumpResult);
			if (result)
			{
				// 押し戻す処理
				m_worldPos += bumpResult.m_pushVec;
			}
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	for (const std::shared_ptr<GameObject>& spObject : GameSystem::GetInstance().GetObjects())			// 地面(StageMap)の検索
	{
		if (spObject->GetClassID() != GameObject::eStage) { continue; }	// ステージオブジェクトじゃなかったら処理を飛ばす
		
		Math::Vector3 rayPos = m_prevPos;	// ひとつ前の座標を渡す					
		rayPos.y += JsonF("limitOfStepHeight");	// 歩いて移動できる地面の限界の段差
		RayInfo rayInfo(rayPos, Math::Vector3(0.0f, -1.0f, 0.0f), m_verticalMovement + JsonF("limitOfStepHeight"));

		BumpResult bumpResult;
		spObject->CheckCollisionRay(rayInfo, bumpResult);		// 相手に当たり判定を呼ばせる

		if (bumpResult.m_isHit)
		{
			m_worldPos += bumpResult.m_pushVec;					// 押し戻す処理
			m_verticalMovement = 0;
			m_canJump = true;
		}
	}
}

bool Player::CheckCollisionRay(const RayInfo& info, HitResult& result)
{
	float distance = 0;
	if (damageSphere.Intersects(info.m_pos, info.m_dir, distance))
	{
		if (distance <= info.m_range)
		{
			result.m_HitPos = info.m_pos + info.m_dir * distance;
			return true;
		}
	}
	return false;
}
void Player::GunInit()
{
	m_spGun  = std::make_shared<Gun>();
	m_spGun->Init();
	m_spGun->SetCamera(m_spCamera);
	GameSystem::GetInstance().AddObject(m_spGun);
}

void Player::DamageRep()
{
	m_damageColor = { 1,1,1,m_damageAlpha };
	if (m_damageAlpha > 0) { m_damageAlpha -= JsonF("damageAlphaDec"); }
}

void Player::HpRep()
{
	// クリア時は処理を通らないようにする
	if (GameEnemyManeger.get()->lastEnemyIsAlive()){return;}
	// カラーの透明値を常に更新
	m_dyingFrameColor = { 1,1,1,m_dyingFrameAlpha };
	m_dyingWholeColor = { 1,1,1,m_dyingWholeAlpha };
	// HPが15以下で血を点滅
	if (m_Hp <= JsonI("halfHp")&& m_Hp > JsonI("dyingHp")){
		m_dyingFrameProg += JsonF("halfFlashing");
	}
	else if (m_Hp <= JsonI("dyingHp")) 	// 5以下で早く描画　画面を赤色にする
	{ 
		// 点滅速度
		m_dyingFrameProg += JsonF("dyingFlashing");
		// 画面を赤く
		m_dyingWholeAlpha = JsonF("dyingFill");
	}
	m_dyingFrameAlpha = easeInOutSine(m_dyingFrameProg, 1.0f);
}

void Player::DeathMovement()
{
	// 現在のカメラの座標を入れる。
	if (m_deathMomentFlg)
	{
		m_deathCameraX = m_spCamera->GetPos().x;
		m_deathCameraY = m_spCamera->GetPos().y;
		m_deathCameraZ = m_spCamera->GetPos().z;
		m_deathMomentFlg = false;
	}
	// イージングチートで座標に値を足しこみ、倒れているのを表現
	{
		m_deathCameraProgY += JsonF("deathCameraMove");
		m_deathCameraY -= easeOutBounce(m_deathCameraProgY,1);
	}
	
	m_spCamera->SetLocalPos(Math::Vector3(m_deathCameraX, m_deathCameraY, m_deathCameraZ));
}
