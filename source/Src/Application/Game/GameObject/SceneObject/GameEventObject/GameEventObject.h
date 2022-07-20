#pragma once
#include"../SceneObject.h"

class GameEventObject :public SceneObject
{
public:
	GameEventObject(){}
	~GameEventObject(){}

	void Init() override;
	void Update() override;

private:
	
	void ClearUpdate();
	void DeathUpdate();

	std::shared_ptr<FadeInOut> m_fadeOutTex;		// clear���ŏ��̃t�F�[�h�A�E�g
	std::shared_ptr<FadeInOut> m_secondFadeOutTex;	// clear��2���ڂ̃t�F�[�h�A�E�g

	bool	m_clearFirst =true;		// �N���A�����Ƃ��ʂ�
	bool	m_harfFirst = true;		// �����s�����Ƃ��ɒʂ�
	bool	m_endOfCntFirst = true;	// �J�E���g�I����ʂ�

	int		m_halfCnt = 0;		// �����̃J�E���g


	std::shared_ptr<FadeInOut> m_deathFadeOutTex;	// ���񂾂Ƃ��p�t�F�[�h�A�E�g
	bool	m_deathFirst = true;	// ���S���ŏ��ɒʂ鏈��
};