#include"GameEventObject.h"
#include"../../TextureDraw/TextureDraw.h"
#include"../../TextureDraw/FadeInOut/FadeInOut.h"
#include"../../Manager/EnemyManager/EnemyManager.h"
#include"../../Character/Player/Player.h"

void GameEventObject::Init()
{
	SetJson("Data/Json/SceneObject/Game.json");
	// フェードインクラス呼び出し
	std::shared_ptr<FadeInOut> fadeIn = std::make_shared<FadeInOut>();
	fadeIn->Init(true, JsonF("fadeInSpeed"));
	GameSystem::GetInstance().AddObject(fadeIn);
}

void GameEventObject::Update()
{
	// クリア条件を達成したら
	if (GameEnemyManeger.get()->lastEnemyIsAlive())
	{
		ClearUpdate();
	}
	// プレイヤーが死亡したら
	if (GameInstance.GetPlayer().lock()->DeathFlg())
	{
		DeathUpdate();
	}
}


void GameEventObject::ClearUpdate()
{
	if (m_clearFirst)
	{
		m_fadeOutTex = std::make_shared<FadeInOut>();
		m_fadeOutTex->StopFadeOutInit(JsonF("firstFadeOut", "fadeSpeed"), JsonF("firstFadeOut", "stopAlpha"), JsonI("firstFadeOut", "stopTime"));
		GameSystem::GetInstance().AddObject(m_fadeOutTex);
		m_clearFirst = false;	// 一度だけ通る
	}

	if (!m_fadeOutTex) { return; }

	if (m_fadeOutTex->GetEndFlg()) {
		// カウントを初める
		if (m_halfCnt < JsonI("harfProc","halfCntMax")) { m_halfCnt++; }

		// 最初に一度だけ通る
		if (m_harfFirst) {
			// 音
			std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
			spSoundInstance = GameAudio.Play(JsonS("sound","clear"));
			spSoundInstance->SetVolume(JsonF("volume","clear"));
			// 画像
			std::shared_ptr<TextureDraw> CompleteTex = std::make_shared<TextureDraw>();
			CompleteTex->Init(JsonS("texture","complete"), JsonV2("harfProc","completePos"), JsonR("harfProc","completeRect"));
			GameSystem::GetInstance().AddObject(CompleteTex);
			m_harfFirst = false;
		}
	}

	// カウントが完了したら
	if (m_halfCnt >= JsonI("harfProc", "halfCntMax")) {
		// 2枚目のフェードアウト
		if (m_endOfCntFirst)
		{
			m_secondFadeOutTex = std::make_shared<FadeInOut>();
			m_secondFadeOutTex->Init(false, JsonF("secondFadeOut","fadeSpeed"));
			GameSystem::GetInstance().AddObject(m_secondFadeOutTex);
			m_endOfCntFirst = false;
		}
	}
	if (!m_secondFadeOutTex) { return; }

	// 2枚目のフェードアウト終了でリザルトへ移行
	if (m_secondFadeOutTex->GetEndFlg()) {
		GameInstance.RequestCangeScene("Result");
	}

}

void GameEventObject::DeathUpdate()
{
	// だんだん暗くしていく
	//darken(0.03f);
	if (m_deathFirst)
	{
		m_deathFadeOutTex = std::make_shared<FadeInOut>();
		m_deathFadeOutTex->Init(false,JsonF("deathUpdate","fadeOutSpeed"));
		GameSystem::GetInstance().AddObject(m_deathFadeOutTex);
		m_deathFirst = false;
	}
	// 処理が終わったらゲームオーバーへ移動
	if (m_deathFadeOutTex->GetEndFlg())
	{
		GameInstance.RequestCangeScene("Death");
	}
}
