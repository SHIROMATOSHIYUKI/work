#pragma once
#include"../GameObject.h"

class SceneObject : public GameObject
{
public:
	SceneObject() {}				// コンストラクター
	~SceneObject() {}				// デストラクター

protected:
	// フェードアウト
	std::shared_ptr<FadeInOut> fadeOut;
};