#include "EnemyManager.h"
#include"../../Character/Enemy/Turret/Turret.h"
#include "../../Character/Enemy/EnemyBase.h"
#include "../../../GameSystem.h"
#include"../../GameObject.h"
void EnemyManager::Init()
{
	SetJson("Data/Json/Manager/EnemyManager.json");
	// 地上の敵
	{CreateEnemy(JsonV3("fast1","pos"), JsonV3("fast1", "headRot"), false); }

	{CreateEnemy(JsonV3("second1", "pos"), JsonV3("second1", "headRot"), false);}
	{CreateEnemy(JsonV3("second2","pos"), JsonV3("second2", "headRot"), false); }

	{CreateEnemy(JsonV3("third1", "pos"), JsonV3("third1", "headRot"), false); }
	// 坂の上の敵2対
	{CreateEnemy(JsonV3("slope1", "pos"), JsonV3("slope1", "headRot"), false);}
	{CreateEnemy(JsonV3("slope2", "pos"), JsonV3("slope2", "headRot"), false);}
	// 遺跡一階の敵
	{CreateEnemy(JsonV3("castle1", "pos"), JsonV3("castle1", "headRot"), false);}
	{CreateEnemy(JsonV3("castle2", "pos"), JsonV3("castle2", "headRot"), false);}
	{CreateEnemy(JsonV3("castle3", "pos"), JsonV3("castle3", "headRot"), false);}
	// 遺跡から打ち下ろす敵
	{CreateEnemy(JsonV3("downhill", "pos"), JsonV3("downhill", "headRot"), false);}
	{CreateEnemy(JsonV3("downhil2", "pos"), JsonV3("downhil2", "headRot"), false);}
	{CreateEnemy(JsonV3("downhil3", "pos"), JsonV3("downhil3", "headRot"), false);}
	{CreateEnemy(JsonV3("downhil4", "pos"), JsonV3("downhil4", "headRot"), false); }
	
	// ラストステージの敵
	// 地上の敵
	{CreateEnemy(JsonV3("lastGround1", "pos"), JsonV3("lastGround1", "headRot"), true); }
	{CreateEnemy(JsonV3("lastGround2", "pos"), JsonV3("lastGround2", "headRot"), true); }
	// 柱上の敵
	{CreateEnemy(JsonV3("lastPillar1", "pos"), JsonV3("lastPillar1", "headRot"), true); }
	{CreateEnemy(JsonV3("lastPillar2", "pos"), JsonV3("lastPillar2", "headRot"), true); }
	{CreateEnemy(JsonV3("lastPillar3", "pos"), JsonV3("lastPillar3", "headRot"), true); }
	{CreateEnemy(JsonV3("lastPillar4", "pos"), JsonV3("lastPillar4", "headRot"), true); }
}


void EnemyManager::CreateEnemy(Math::Vector3 pos, Math::Vector3 spornPos, bool LastPageEnemy)
{
	std::shared_ptr<Turret> spEnemy = std::make_shared<Turret>();
	spEnemy->SetPos(pos);
	spEnemy->SetHeadRot(spornPos);
	spEnemy->Init();
	GameInstance.AddObject(spEnemy);

	if (LastPageEnemy)
	{
		spEnemy->SetClearEnemy(true);
		lastEnemyCount++;
		m_wpClearEnemy.push_back(spEnemy);
	}
}

void EnemyManager::Update()
{
	// クリアエネミーリストを回し、期限ぎれの場合消去する
	for (auto itr = m_wpClearEnemy.begin(); itr != m_wpClearEnemy.end(); ++itr)
	{
		if (itr->expired()){m_wpClearEnemy.erase(itr);}
	}

	if (lastEnemyCount <= 0)
	{
		m_wpClearEnemy.clear();
		lastEnemyFlg = true;
	}

	// クリアエネミーリストに敵がいなかった場合
	//if (m_wpClearEnemy.size() == 0)
	//{lastEnemyFlg = true;}
}

void EnemyManager::Release()
{
}