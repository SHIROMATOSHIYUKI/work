#pragma once
#include"../GameObject.h"

class SceneObject : public GameObject
{
public:
	SceneObject() {}				// �R���X�g���N�^�[
	~SceneObject() {}				// �f�X�g���N�^�[

protected:
	// �t�F�[�h�A�E�g
	std::shared_ptr<FadeInOut> fadeOut;
};