#pragma once
#include"../SceneObject.h"

class TitleObject : public SceneObject
{
public:
	TitleObject() {}			// コンストラクター
	~TitleObject() {}			// デストラクター

	void Init() override;
	void Update() override;
private:
	void Release() {}			// 解放
};