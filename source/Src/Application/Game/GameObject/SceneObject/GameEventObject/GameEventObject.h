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

	std::shared_ptr<FadeInOut> m_fadeOutTex;		// clear時最初のフェードアウト
	std::shared_ptr<FadeInOut> m_secondFadeOutTex;	// clear時2枚目のフェードアウト

	bool	m_clearFirst =true;		// クリアしたとき通る
	bool	m_harfFirst = true;		// 半分行ったときに通る
	bool	m_endOfCntFirst = true;	// カウント終了後通る

	int		m_halfCnt = 0;		// 半分のカウント


	std::shared_ptr<FadeInOut> m_deathFadeOutTex;	// 死んだとき用フェードアウト
	bool	m_deathFirst = true;	// 死亡時最初に通る処理
};