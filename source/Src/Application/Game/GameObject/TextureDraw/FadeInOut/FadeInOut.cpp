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
	m_stopAlpha = stopAlpha;	// 止めるタイミングの透明値
	m_stopTime = stopTime;		// 止める時間
	m_fadeInFlg = false;// フェードアウトにする
	m_alpha = 0.0f;		// フェードアウトなのでスタートは透明に
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
	// 透明値を更新
	m_Color = { 0,0,0,m_alpha };
}
void FadeInOut::Draw2D()
{
	SHADER->m_spriteShader.DrawTex(m_spTex.get(), (int)m_pos.x, (int)m_pos.y,1280,720, &m_rect, &m_Color);
}
// 明るくなってインしていく
void FadeInOut::FadeIn()
{
	// 完全透明になるまで明るくしていく
	// 透明になったら終了フラグを返す
	if (m_alpha >= 0.0f)
	{
		m_alpha -= m_fadeSpeed;
	}
	else if (m_alpha < 0.0f) { m_endFlg = true; }
}
// 暗くなってアウトしていく
void FadeInOut::FadeOut()
{
	// stopAlphaに値が入っていたら停止フェードアウト
	if(m_stopAlpha != 0){
		if (m_alpha <= m_stopAlpha){	// stopAlphaに達するまで
			m_alpha += m_fadeSpeed;
		}
		else {	// stopAlphaに達したら処理終了フラグを立てる 
			m_endFlg = true;
		}
	}
	else {// stopAlphaに何も入っていないので通常のフェードアウト
		// 真っ暗になるまで暗くしていく。
		// 真っ暗になったら終了フラグを返す
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