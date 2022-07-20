#pragma once
#include"../SceneObject.h"

class GameoverObject : public SceneObject
{
public:
	GameoverObject() {}		// コンストラクター
	~GameoverObject() {}	// デストラクター

	void Init() override;
	void Update() override;

private:
	void Release() {}		// 解放
};