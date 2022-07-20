#pragma once
#include"../GameObject.h"

class Character : public GameObject
{
public:
	Character() {}
	~Character() {}

	// キャラクター専用のHPを減らす当たり判定
	// 今は弾の判定でしか使う予定がないのでレイ対メッシュ
	virtual bool CheckCollisionDamegeSphere(const RayInfo& info,DamegeResult& result);

protected:
	int m_Hp = 0;
private:

};