#pragma once
#include"../SceneObject.h"

class ResultObject : public SceneObject
{
public:
	ResultObject() {}	// �R���X�g���N�^�[
	~ResultObject() {}	// �f�X�g���N�^�[

	void Init() override;
	void Update() override;

private:
	void Release() {}	// ���
};