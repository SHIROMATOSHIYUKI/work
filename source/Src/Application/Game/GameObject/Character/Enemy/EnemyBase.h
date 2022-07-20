#pragma once
#include"../Character.h"

class Player;

class EnemyBase : public Character
{
public:
	EnemyBase() {}
	~EnemyBase() {}
	// 「エネミー」を示すクラスIDを返す
	const GameObject::ClassID GetClassID() const override{return GameObject::eEnemy;}		

	bool CheckCollisionDamegeRay(const RayInfo& info, DamegeResult& result) override;

	void SetClearEnemy(bool ClearEnemy) { m_clearEnemy = ClearEnemy; }
protected:
	std::weak_ptr<Player>	m_wpTarget;		// ターゲット

	bool m_clearEnemy = false;
private:

};