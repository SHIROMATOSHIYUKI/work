#pragma once
#include"../TextureDraw.h"
// フェードイン、フェードアウト画像を描画するクラス
class FadeInOut :public TextureDraw
{
public:
	FadeInOut(){}
	~FadeInOut(){}

	void Init(
		bool fadeIn,
		float fadeSpeed
	);

	// 途中で止めたいときのInit
	void StopFadeOutInit(
		float fadeSpeed,
		float stopAlpha,	// 止める値
		int stopTime	// 止める時間
	);	

	void Update() override;
	void Draw2D() override;

	bool GetEndFlg() { return m_endFlg; }
private:
	// フェードインとアウトの処理
	void FadeIn();
	void FadeOut();

	Math::Color m_Color;// 色
	float m_alpha;		// 透明値
	float m_fadeSpeed;	// 処理の速度
	bool m_fadeInFlg;	// フェードインかアウトか判断するためのフラグ
	bool m_endFlg = false;		// 処理が終了したかを取得するフラグ

	// 途中で止めたいときに使う変数
	float 	m_stopAlpha = 0;	// 止めるタイミングの透明値
	int 	m_stopTime;		// 止める時間
};