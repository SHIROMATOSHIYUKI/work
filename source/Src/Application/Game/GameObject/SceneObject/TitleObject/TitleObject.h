#pragma once
#include"../SceneObject.h"

class TitleObject : public SceneObject
{
public:
	TitleObject() {}			// �R���X�g���N�^�[
	~TitleObject() {}			// �f�X�g���N�^�[

	void Init() override;
	void Update() override;
private:
	void Release() {}			// ���
};