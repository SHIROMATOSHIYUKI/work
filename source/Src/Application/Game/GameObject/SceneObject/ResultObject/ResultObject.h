#pragma once
#include"../SceneObject.h"

class ResultObject : public SceneObject
{
public:
	ResultObject() {}	// コンストラクター
	~ResultObject() {}	// デストラクター

	void Init() override;
	void Update() override;

private:
	void Release() {}	// 解放
};