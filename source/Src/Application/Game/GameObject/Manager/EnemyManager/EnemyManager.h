#pragma once
#include "../../GameObject.h"
class EnemyBase;

class EnemyManager:public GameObject
{
public:
	EnemyManager() {}
	~EnemyManager() { Release(); }

	void Init();
	void Update();
	void CreateEnemy(Math::Vector3 pos, Math::Vector3 headRot, bool LastPageEnemy);	// 敵の呼び出し

	bool lastEnemyIsAlive() { return  lastEnemyFlg; }
	void LastEnemyCountReduce() { lastEnemyCount--; }
private:
	void Release();
	bool lastEnemyFlg = false;
	int lastEnemyCount = 0;

	std::list<std::weak_ptr<GameObject>> m_wpClearEnemy;	// このリストの敵全員倒すとクリア
};