#pragma once
#include"../GameObject.h"

// ↓GameObjectを継承させてください。
class StageMap : public GameObject
{
public:
	StageMap() {}			// コンストラクター
	~StageMap() {}			// デストラクター

	void Init() override;	// 初期化

	const GameObject::ClassID GetClassID() const override
	{
		return GameObject::eStage;
	}
	void onhit() override;
	bool CheckCollisionBump(const SphereInfo& info, BumpResult& result) override;

private:
	void Release();			// 解放
};