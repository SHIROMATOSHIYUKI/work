#include"FadeInOut.h"

void FadeInOut::Init(bool fadeIn, float fadeSpeed)
{
	m_spTex = GameResourceFactory.GetTexture("Data/Textures/dark.png");
	m_fadeInFlg = fadeIn;
	if (m_fadeInFlg) { m_alpha = 1.0f; }
	else { m_alpha = 0.0f; }
	m_fadeSpeed = fadeSpeed;
}

void FadeInOut::StopFadeOutInit(float fadeSpeed, float stopAlpha, int stopTime)
{
	m_spTex = GameResourceFactory.GetTexture("Data/Textures/dark.png");
	m_fadeSpeed = fadeSpeed;
	m_stopAlpha = stopAlpha;	// �~�߂�^�C�~���O�̓����l
	m_stopTime = stopTime;		// �~�߂鎞��
	m_fadeInFlg = false;// �t�F�[�h�A�E�g�ɂ���
	m_alpha = 0.0f;		// �t�F�[�h�A�E�g�Ȃ̂ŃX�^�[�g�͓�����
}

void FadeInOut::Update()
{
	if (m_fadeInFlg)
	{
		FadeIn();
	}
	else
	{
		FadeOut();
	}
	// �����l���X�V
	m_Color = { 0,0,0,m_alpha };
}
void FadeInOut::Draw2D()
{
	SHADER->m_spriteShader.DrawTex(m_spTex.get(), (int)m_pos.x, (int)m_pos.y,1280,720, &m_rect, &m_Color);
}
// ���邭�Ȃ��ăC�����Ă���
void FadeInOut::FadeIn()
{
	// ���S�����ɂȂ�܂Ŗ��邭���Ă���
	// �����ɂȂ�����I���t���O��Ԃ�
	if (m_alpha >= 0.0f)
	{
		m_alpha -= m_fadeSpeed;
	}
	else if (m_alpha < 0.0f) { m_endFlg = true; }
}
// �Â��Ȃ��ăA�E�g���Ă���
void FadeInOut::FadeOut()
{
	// stopAlpha�ɒl�������Ă������~�t�F�[�h�A�E�g
	if(m_stopAlpha != 0){
		if (m_alpha <= m_stopAlpha){	// stopAlpha�ɒB����܂�
			m_alpha += m_fadeSpeed;
		}
		else {	// stopAlpha�ɒB�����珈���I���t���O�𗧂Ă� 
			m_endFlg = true;
		}
	}
	else {// stopAlpha�ɉ��������Ă��Ȃ��̂Œʏ�̃t�F�[�h�A�E�g
		// �^���ÂɂȂ�܂ňÂ����Ă����B
		// �^���ÂɂȂ�����I���t���O��Ԃ�
		if (m_alpha <= 1.0f)
		{
			m_alpha += m_fadeSpeed;
		}
		else
		{
			m_endFlg = true;
		}
	}
}