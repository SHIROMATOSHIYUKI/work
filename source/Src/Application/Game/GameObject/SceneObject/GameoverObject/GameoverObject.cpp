#include "GameoverObject.h"
#include"../../TextureDraw/TextureDraw.h"
#include"../../TextureDraw/TextureEasing/TextureEasing.h"
#include"../../TextureDraw/FadeInOut/FadeInOut.h"
#include"../../Character/Player/Player.h"

void GameoverObject::Init()
{
	SetJson("Data/Json/SceneObject/GameOver.json");
	// 2D画像描画クラス
	std::shared_ptr<TextureDraw> GameOverTex = std::make_shared<TextureDraw>();
	GameOverTex->Init(JsonS("texture", "gameover"));
	GameSystem::GetInstance().AddObject(GameOverTex);
	
	// イージング画像描画クラス
	std::shared_ptr<TextureEasing> GuideTex = std::make_shared<TextureEasing>();
	GuideTex->Init(JsonS("texture", "guide"), JsonF("easingSpeed"), "InOutSine", JsonV2("easingPos"), JsonR("easingRect"));
	GameSystem::GetInstance().AddObject(GuideTex);

	// フェードインクラス呼び出し
	std::shared_ptr<FadeInOut> fadeIn = std::make_shared<FadeInOut>();
	fadeIn->Init(true, JsonF("fadeInSpeed"));
	GameSystem::GetInstance().AddObject(fadeIn);
	
	// BGMの再生
	std::shared_ptr<KdSoundInstance> spSoundInstance = nullptr;
	spSoundInstance = GameAudio.Play(JsonS("sound", "dead"));
	spSoundInstance->SetVolume(JsonF("volume", "dead"));
}

void GameoverObject::Update()
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		// フェードアウト開始
		fadeOut = std::make_shared<FadeInOut>();
		fadeOut->Init(false, JsonF("fadeOutSpeed"));
		GameSystem::GetInstance().AddObject(fadeOut);
	}
	if (fadeOut) {
		if (fadeOut->GetEndFlg()) { GameInstance.RequestCangeScene("Title"); }
	}
}