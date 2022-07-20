#pragma once
#include"../TextureDraw.h"
// �t�F�[�h�C���A�t�F�[�h�A�E�g�摜��`�悷��N���X
class FadeInOut :public TextureDraw
{
public:
	FadeInOut(){}
	~FadeInOut(){}

	void Init(
		bool fadeIn,
		float fadeSpeed
	);

	// �r���Ŏ~�߂����Ƃ���Init
	void StopFadeOutInit(
		float fadeSpeed,
		float stopAlpha,	// �~�߂�l
		int stopTime	// �~�߂鎞��
	);	

	void Update() override;
	void Draw2D() override;

	bool GetEndFlg() { return m_endFlg; }
private:
	// �t�F�[�h�C���ƃA�E�g�̏���
	void FadeIn();
	void FadeOut();

	Math::Color m_Color;// �F
	float m_alpha;		// �����l
	float m_fadeSpeed;	// �����̑��x
	bool m_fadeInFlg;	// �t�F�[�h�C�����A�E�g�����f���邽�߂̃t���O
	bool m_endFlg = false;		// �������I�����������擾����t���O

	// �r���Ŏ~�߂����Ƃ��Ɏg���ϐ�
	float 	m_stopAlpha = 0;	// �~�߂�^�C�~���O�̓����l
	int 	m_stopTime;		// �~�߂鎞��
};